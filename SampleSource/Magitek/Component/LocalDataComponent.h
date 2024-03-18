// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekStructDefine.h"
#include "SaveMagitekStructDefine.h"
#include "Components/ActorComponent.h"
#include "LocalDataComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyUpdatePostData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnNotifyFirstAchievementData);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnNotifyUpdateMyAchievementData, FAchievementHistory, Achievement);

UCLASS(BlueprintType, Blueprintable, ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGITEK_API ULocalDataComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	ULocalDataComponent();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frameBlueprintReadWrite, VisibleAnywhere
	//virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnNotifyUpdatePostData OnNotifyUpdatePostData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnNotifyFirstAchievementData OnNotifyFirstAchievementData;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnNotifyUpdateMyAchievementData OnNotifyUpdateMyAchievementData;

#pragma region Post
	void AddPostInfo(const struct FPostInfo& PostInfo);

	UFUNCTION(BlueprintCallable)
	void RemovePlayerPostInfo(int32 PostID, bool& bRemoveSuccess);

	UFUNCTION(BlueprintCallable)
	void UpdateLocalPostData(const FString& PostStringData);

	UFUNCTION(Blueprintpure)
	TMap<int32, struct FPostInfo> GetPlayerPostMap();

	UFUNCTION(BlueprintCallable)
	void FindPostInfo(int32 PostId, FPostInfo& PostInfo, bool& bFind);

	UFUNCTION(BlueprintCallable)
	void AddOpenedPostID(int32 PostId);

	UFUNCTION(Blueprintpure)
	TArray<struct FPostInfo> GetPlayerNewPostInfoList();

	UFUNCTION(BlueprintCallable)
	void ClearPlayerNewPostInfoList();

	UFUNCTION(Blueprintpure)
	void GetPlayerPostList(TArray<struct FPostInfo>& PlayerPostInfoList);

	UFUNCTION(Blueprintpure)
	void GetOpenedPostInfoList(TArray<struct FPostInfo>& OpenedPostInfoList);

	UFUNCTION(Blueprintpure)
	bool IsOpenedPost(int32 PostID);

	UFUNCTION(BlueprintCallable)
	void UpdatePostNoItemListOpen(bool& bNeedLocalSave);

	UFUNCTION(Blueprintpure)
	bool IsNotOpenedPostItem();

#pragma endregion

#pragma region Item
	
	UFUNCTION(Blueprintpure)
	int32 GetCachedItemCount(int32 ItemID);

	UFUNCTION(BlueprintCallable)
	void AddItemCount(int32 ItemID, int32 ItemCount);

	UFUNCTION(BlueprintCallable)
	void AddItemCountFromPost(const FPostInfo& PostInfo);

#pragma endregion

#pragma region Achievement

	UFUNCTION(BlueprintCallable)
	void AddFirstAchievementUser(int AchievementId, const FString& UserNickname);

	UFUNCTION(Blueprintpure)
	TMap<int32, FString> GetCachedFirstAchievement();

	UFUNCTION(BlueprintCallable)
	void AddMyAchievement(const FAchievementHistory& Achievement);

	UFUNCTION(Blueprintpure)
	TMap<int32, FAchievementHistory>& GetCachedMyAchievement();

	UFUNCTION(BlueprintCallable)
	void SetCachedMyAchievement(const TMap<int32, FAchievementHistory>& InCachedMyAchievement);

#pragma endregion

#pragma region IAP
	UFUNCTION(BlueprintCallable) 
	void AddIAPLog(int32 ShopID, int32 Timestamp)
	{
		if ( CachedIAP.Contains(ShopID) == false )
		{
			CachedIAP.Add(ShopID, TArray<FIAPLog>());
		}

		FIAPLog Log;
		Log.ShopID = ShopID;
		Log.Timestamp = Timestamp;
		
		CachedIAP[ShopID].EmplaceAt(0, Log);
	}

	UFUNCTION(Blueprintpure)
	bool GetIAPLogs(int32 ShopID, TArray<FIAPLog>& OutLogs)
	{
		TArray<FIAPLog>* Logs = CachedIAP.Find(ShopID);

		if ( Logs != nullptr )
		{
			OutLogs = *Logs;
			return true;
		}
		return false;
	}
#pragma endregion 	
private:

#pragma region Post
	
	//우편함 ID, 우편함 정보, 전체 정보
	TMap<int32, struct FPostInfo> PlayerPostMap;

	//우편함 ID, 우편함 정보, 새로 받은 우편함 
	TArray<struct FPostInfo> PlayerNewPostInfoList;

#pragma endregion

#pragma region Item

	// 아이템 카운트 맵핑
	TMap<int32, int32> CachedItemCountMap;

#pragma endregion

#pragma region Achievement

	// 업적 최초 달성자 캐싱
	TMap<int32, FString> CachedFirstAchievement;

	// 내가 클리어한 업적 데이터 캐싱
	TMap<int32, FAchievementHistory> CachedMyAchievement; 

#pragma endregion

#pragma region IAP
	TMap<int32, TArray<FIAPLog>> CachedIAP;
#pragma endregion 	
};
