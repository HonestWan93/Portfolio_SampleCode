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
void ULocalDataComponent::RemovePlayerPostInfo(int32 PostID, bool& bRemoveSuccess)
{
	// 제거 할 우편이 없다
	if (PlayerPostMap.Contains(PostID) == false)
	{
		bRemoveSuccess = false;
		return;
	}

	PlayerPostMap.Remove(PostID);
	bRemoveSuccess = true;
}

// 서버로부터 받은 우편 정보 업데이트
void ULocalDataComponent::UpdateLocalPostData(const FString& PostStringData)
{
	FBlueprintJsonObject PostListJsonObject = UBlueprintJsonLibrary::Conv_StringToJsonObject(PostStringData);

	if (UBlueprintJsonLibrary::JsonHasField(PostListJsonObject, "posts") == false)
	{
		return;
	}

	FBlueprintJsonValue PostsJsonValue = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostListJsonObject, "posts");
	TArray<FBlueprintJsonValue> PostList = UBlueprintJsonLibrary::Conv_JsonValueToArray(PostsJsonValue);

	for (auto PostJsonValue : PostList)
	{
		//우편함 정보
		FPostInfo TempPostInfo;
		FBlueprintJsonObject PostJsonObject = UBlueprintJsonLibrary::Conv_JsonValueToObject(PostJsonValue);
		
		//우편함에 아래 체크 값들은 기본적으로 있어야 함. 없으면 넘기자!
		if (UBlueprintJsonLibrary::JsonHasField(PostJsonObject, "id") == false && UBlueprintJsonLibrary::JsonHasField(PostJsonObject, "title") == false 
			&& UBlueprintJsonLibrary::JsonHasField(PostJsonObject, "description") == false && UBlueprintJsonLibrary::JsonHasField(PostJsonObject, "endTimestamp") == false)
		{
			continue;
		}

		TempPostInfo.PostID = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "id"));	
		TempPostInfo.Title = UBlueprintJsonLibrary::Conv_JsonValueToString(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "title"));
		TempPostInfo.Description = UBlueprintJsonLibrary::Conv_JsonValueToString(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "description"));
		TempPostInfo.EndTimestamp = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "endTimestamp"));
		TempPostInfo.HasItem = (bool)UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "hasItem"));

		auto HistoryID = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "postHistories.id");		
		TempPostInfo.IsReceive = HistoryID.Value.IsValid() && HistoryID.Value->IsNull() == false;
		
		TempPostInfo.PostItemList.Empty();

		//필드에 아이템 정보가 없다면 패스 하지만 우편함에 추가
		if (UBlueprintJsonLibrary::JsonHasField(PostJsonObject, "items") == false)
		{
			AddPostInfo(TempPostInfo);
			continue;
		}

		FBlueprintJsonValue PostItemListJsonValue = UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostJsonObject, "items");
		TArray<FBlueprintJsonValue> PostItemListJsonValueArray = UBlueprintJsonLibrary::Conv_JsonValueToArray(PostItemListJsonValue);

		//아이템이 없으면 패스 하고 우편함에 추가
		if (PostItemListJsonValueArray.Num() == 0)
		{
			AddPostInfo(TempPostInfo);
			continue;
		}

		for (auto PostItemJsonValue : PostItemListJsonValueArray)
		{
			FPostItem TempPostItem;
			FBlueprintJsonObject PostItemObject = UBlueprintJsonLibrary::Conv_JsonValueToObject(PostItemJsonValue);

			if (UBlueprintJsonLibrary::JsonHasField(PostItemObject, "type") == false )
				continue;

			auto Type = UBlueprintJsonLibrary::Conv_JsonValueToString(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostItemObject, "type"));
			
			if ( Type == "etc")
				TempPostItem.Type = EItemTypes::Etc;
			else if ( Type == "gold")
				TempPostItem.Type = EItemTypes::Gold;
			else if ( Type == "gem")
				TempPostItem.Type = EItemTypes::Gem;
			else if ( Type == "crystal")
				TempPostItem.Type = EItemTypes::Crystal;
			else if ( Type == "buff")
				TempPostItem.Type = EItemTypes::Buff;
			else
				TempPostItem.Type = EItemTypes::None;
			
			if ( TempPostItem.Type == EItemTypes::Etc || TempPostItem.Type == EItemTypes::Gem)
			{
				if  (UBlueprintJsonLibrary::JsonHasField(PostItemObject, "itemID") == false || UBlueprintJsonLibrary::JsonHasField(PostItemObject, "count") == false )
					continue;
				
				TempPostItem.ItemCount = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostItemObject, "count"));
				TempPostItem.ItemID = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostItemObject, "itemID"));
			}
			else if (TempPostItem.Type == EItemTypes::Gold || TempPostItem.Type == EItemTypes::Crystal)
			{
				if (UBlueprintJsonLibrary::JsonHasField(PostItemObject, "count") == false )
					continue;
				
				TempPostItem.ItemCount = UBlueprintJsonLibrary::Conv_JsonValueToInteger(UBlueprintJsonLibrary::Conv_JsonObjectToJsonValue(PostItemObject, "count"));
			}

			TempPostInfo.PostItemList.Add(TempPostItem);
		}

		AddPostInfo(TempPostInfo);
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

	TArray<int32> KeyArray;
	PlayerPostMap.GetKeys(KeyArray);

	for (auto key : KeyArray)
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

bool ULocalDataComponent::IsOpenedPost(int32 PostID)
{
	if (PlayerPostMap.Contains(PostID) == false)
	{
		return false;
	}

	if (PlayerPostMap[PostID].IsReceive == false)
	{
		return false;
	}

	return true;
}

void ULocalDataComponent::UpdatePostNoItemListOpen(bool& bNeedLocalSave)
{
	bNeedLocalSave = false;

	TArray<int32> PostIDList;
	PlayerPostMap.GetKeys(PostIDList);
	for (auto PostID : PostIDList)
	{
		if (PlayerPostMap[PostID].HasItem == false && PlayerPostMap[PostID].IsReceive == false)
		{
			bNeedLocalSave = true;
			PlayerPostMap[PostID].IsReceive = true;
		}
	}
}

bool ULocalDataComponent::IsNotOpenedPostItem()
{
	TArray<int32> PostIDList;
	PlayerPostMap.GetKeys(PostIDList);
	for (auto PostID : PostIDList)
	{
		if (PlayerPostMap[PostID].HasItem == true && PlayerPostMap[PostID].IsReceive == false)
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
	TArray<FPostItem> PostItemList;
	PostItemList = PostInfo.PostItemList;
	for (auto Item : PostItemList)
	{
		AddItemCount(Item.ItemID, Item.ItemCount);
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


void ULocalDataComponent::AddMyAchievement(const FAchievementHistory& AchievementHistory)
{
	if (CachedMyAchievement.Contains(AchievementHistory.AchievementID))
	{
		return;
	}

	CachedMyAchievement.Add(AchievementHistory.AchievementID, AchievementHistory);
	OnNotifyUpdateMyAchievementData.Broadcast(AchievementHistory);
}


TMap<int32, FAchievementHistory>& ULocalDataComponent::GetCachedMyAchievement()
{
	return CachedMyAchievement;
}

void ULocalDataComponent::SetCachedMyAchievement(const TMap<int32, FAchievementHistory>& InCachedMyAchievement)
{
	CachedMyAchievement = InCachedMyAchievement;
}