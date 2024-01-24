// Fill out your copyright notice in the Description page of Project Settings.


#include "Component/LocalDataComponent.h"
#include "BlueprintJsonLibrary.h"
// Sets default values for this component's properties
ULocalDataComponent::ULocalDataComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	PlayerPostMap.Empty();
	// ...
}


// Called when the game starts
void ULocalDataComponent::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


//// Called every frame
//void ULocalDataComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
//{
//	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
//
//	// ...
//}

#pragma region Post
// 플레이어 우편 정보 및 새로운 우편 정보 추가
void ULocalDataComponent::AddPostInfo(const struct FPostInfo& PostInfo)
{
	if (PlayerPostMap.Contains(PostInfo.PostID) == true)
	{
		return;
	}

	PlayerPostMap.Add(PostInfo.PostID, PostInfo);
	PlayerNewPostInfoList.Add(PostInfo);	
}

// 프레이어 우편 정보 제거
void ULocalDataComponent::RemovePlayerPostInfo(int32 postID, bool& bRemoveSuccess)
{
	// 제거 할 우편이 없다
	if (PlayerPostMap.Contains(postID) == false)
	{
		bRemoveSuccess = false;
		return;
	}

	PlayerPostMap.Remove(postID);
	bRemoveSuccess = true;
}

// 서버로부터 받은 우편 정보 업데이트
void ULocalDataComponent::UpdateLocalPostData(const FString& PostStringData)
{
	FBlueprintJsonObject postListJsonObject = UBlueprintJsonLibrary::Conv_StringToJsonObject(PostStringData);

	if (UBlueprintJsonLibrary::JsonHasField(postListJsonObject, "posts") == false)
	{
		return;
	}

	FBlueprintJsonValue postsJsonValue = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postListJsonObject, "posts");
	TArray<FBlueprintJsonValue> postList = UBlueprintJsonLibrary::Conv_JsonValueToArray(postsJsonValue);

	for (auto postJsonValue : postList)
	{
		//우편함 정보
		FPostInfo tempPostInfo;
		FBlueprintJsonObject postJsonObject = UBlueprintJsonLibrary::Conv_JsonValueToObject(postJsonValue);
		
		//우편함에 아래 체크 값들은 기본적으로 있어야 함. 없으면 넘기자!
		if (UBlueprintJsonLibrary::JsonHasField(postJsonObject, "id") == false && UBlueprintJsonLibrary::JsonHasField(postJsonObject, "title") == false 
			&& UBlueprintJsonLibrary::JsonHasField(postJsonObject, "description") == false && UBlueprintJsonLibrary::JsonHasField(postJsonObject, "endTimestamp") == false)
		{
			continue;
		}

		tempPostInfo.postID = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "id"));	
		tempPostInfo.Title = UBlueprintJsonLibrary::Conv_JsonValueToString(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "title"));
		tempPostInfo.Description = UBlueprintJsonLibrary::Conv_JsonValueToString(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "description"));
		tempPostInfo.EndTimestamp = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "endTimestamp"));
		tempPostInfo.HasItem = (bool)UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "hasItem"));

		auto historyID = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "postHistories.id");		
		tempPostInfo.IsReceive = historyID.Value.IsValid() && historyID.Value->IsNull() == false;
		
		tempPostInfo.postItemList.Empty();

		//필드에 아이템 정보가 없다면 패스 하지만 우편함에 추가
		if (UBlueprintJsonLibrary::JsonHasField(postJsonObject, "items") == false)
		{
			AddPostInfo(tempPostInfo);
			continue;
		}

		FBlueprintJsonValue postItemListJsonValue = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postJsonObject, "items");
		TArray<FBlueprintJsonValue> postItemListJsonValueArray = UBlueprintJsonLibrary::Conv_JsonValueToArray(postItemListJsonValue);

		//아이템이 없으면 패스 하고 우편함에 추가
		if (postItemListJsonValueArray.Num() == 0)
		{
			AddPostInfo(tempPostInfo);
			continue;
		}

		for (auto postItemJsonValue : postItemListJsonValueArray)
		{
			FPostItem tempPostItem;
			FBlueprintJsonObject postItemObject = UBlueprintJsonLibrary::Conv_JsonValueToObject(postItemJsonValue);
			
			if (UBlueprintJsonLibrary::JsonHasField(postItemObject, "itemID") == false && UBlueprintJsonLibrary::JsonHasField(postItemObject, "count") == false )
			{
				continue;
			}

			tempPostItem.ItemID = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postItemObject, "itemID"));
			tempPostItem.ItemCount = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(postItemObject, "count"));

			tempPostInfo.postItemList.Add(tempPostItem);
		}

		AddPostInfo(tempPostInfo);
	}

	OnNotifyUpdatePostData.Broadcast();
}

TMap<int32, struct FPostInfo> ULocalDataComponent::GetPlayerPostMap()
{
	return PlayerPostMap;
}

void ULocalDataComponent::FindPostInfo(int32 PostId, FPostInfo& PostInfo, bool& bFind)
{
	if (PlayerPostMap.Contains(PostId) == false)
	{
		bFind = false;
		return;
	}

	PostInfo = PlayerPostMap[PostId];
	bFind = true;
}

void ULocalDataComponent::AddOpenedPostID(int32 PostId)
{
	// 0은 아무값도 아닌 디폴트 값 방어코드
	if (PostId == 0)
	{
		return;
	}

	if (PlayerPostMap.Contains(PostId) == false)
	{
		return;
	}	

	PlayerPostMap[PostId].IsReceive = true;
}

TArray<struct FPostInfo> ULocalDataComponent::GetPlayerNewPostInfoList()
{
	return PlayerNewPostInfoList;
}

void ULocalDataComponent::ClearPlayerNewPostInfoList()
{
	PlayerNewPostInfoList.Empty();
}

void ULocalDataComponent::GetPlayerPostList(TArray<struct FPostInfo>& PlayerPostInfoList)
{
	PlayerPostInfoList.Empty();

	TArray<int32> keyArray;
	PlayerPostMap.GetKeys(keyArray);

	for (auto key : keyArray)
	{
		//방어 코드
		if (PlayerPostMap.Contains(key) == false)
		{
			continue;
		}

		PlayerPostInfoList.Add(PlayerPostMap[key]);
	}
}

void ULocalDataComponent::GetOpenedPostInfoList(TArray<struct FPostInfo>& OpenedPostInfoList)
{
	OpenedPostInfoList.Empty();

	for (auto PlayerPost : PlayerPostMap)
	{
		//방어 코드 열지 않았다
		if (PlayerPost.Value.IsReceive == false)
		{
			continue;
		}

		OpenedPostInfoList.Add(PlayerPost.Value);
	}
}

bool ULocalDataComponent::IsOpenedPost(int32 postID)
{
	if (PlayerPostMap.Contains(postID) == false)
	{
		return false;
	}

	if (PlayerPostMap[postID].IsReceive == false)
	{
		return false;
	}

	return true;
}

void ULocalDataComponent::UpdatePostNoItemListOpen(bool& bNeedLocalSave)
{
	bNeedLocalSave = false;

	TArray<int32> postIDList;
	PlayerPostMap.GetKeys(postIDList);
	for (auto postID : postIDList)
	{
		if (PlayerPostMap[postID].HasItem == false && PlayerPostMap[postID].IsReceive == false)
		{
			bNeedLocalSave = true;
			PlayerPostMap[postID].IsReceive = true;
		}
	}
}

bool ULocalDataComponent::IsNotOpenedPostItem()
{
	TArray<int32> postIDList;
	PlayerPostMap.GetKeys(postIDList);
	for (auto postID : postIDList)
	{
		if (PlayerPostMap[postID].HasItem == true && PlayerPostMap[postID].IsReceive == false)
		{
			return true;
		}
	}

	return false;
}

#pragma endregion

// 아이템 카운트 Get
int32 ULocalDataComponent::GetCachedItemCount(int32 ItemID)
{
	if (CachedItemCountMap.Contains(ItemID))
	{
		return 0;
	}

	return CachedItemCountMap[ItemID];
}

// 아이템 카운트 증가
void ULocalDataComponent::AddItemCount(int32 ItemID, int32 ItemCount)
{
	if (CachedItemCountMap.Contains(ItemID) == false)
	{
		CachedItemCountMap.Add(ItemID, ItemCount);
	}

	CachedItemCountMap[ItemID] += ItemCount;
}

// 우편 정보에 포함된 아이템
void ULocalDataComponent::AddItemCountFromPost(const FPostInfo& PostInfo)
{
	TArray<FPostItem> postItemList;
	postItemList = PostInfo.PostItemList;
	for (auto item : postItemList)
	{
		AddItemCount(item.ItemID, item.ItemCount);
	}
}

void ULocalDataComponent::AddFirstAchievementUser(int AchievementId, const FString& UserNickname)
{
	if (CachedFirstAchievement.Contains(AchievementId))
	{
		return;
	}

	CachedFirstAchievement.Add(AchievementId, UserNickname);
}

TMap<int32, FString> ULocalDataComponent::GetCachedFirstAchievement()
{
	return CachedFirstAchievement;
}


void ULocalDataComponent::AddMyAchievemen(int AchievementId)
{
	if (CachedMyAchievement.Contains(AchievementId))
	{
		return;
	}

	CachedMyAchievement.Add(AchievementId);
	OnNotifyUpdateMyAchievementData.Broadcast(AchievementId);
}


TArray<int32>& ULocalDataComponent::GetCachedMyAchievement()
{
	return CachedMyAchievement;
}

void ULocalDataComponent::SetCachedMyAchievement(const TArray<int32>& InCachedMyAchievement)
{
	CachedMyAchievement = InCachedMyAchievement;
}