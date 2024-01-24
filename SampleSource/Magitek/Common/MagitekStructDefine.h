// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UObject/Object.h"
#include "CoreMinimal.h"
#include "MagitekEnumDefine.h"
#include "Engine/DataTable.h"
#include "Frameworks/Snap/SnapMap/SnapMapModuleDatabase.h"
#include "Builders/SnapMap/SnapMapAsset.h"
#include "MagitekStructDefine.generated.h"

class UGemFunctionBase;
class UGemFunctionParameter;

class UAchievementFunctionParameter;
class UAchievementFunctionParameterBase;

// 공통 정보
USTRUCT(BlueprintType)
struct FCommonInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MainName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> Icon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText MainDesc;
};

USTRUCT(BlueprintType)
struct FActorArray
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<class AActor*> ActorArray;
};

USTRUCT(BlueprintType)
struct FAccountInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Nickname;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Google;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Guest;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Apple;
};


USTRUCT(BlueprintType)
struct FPostItem
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
};

USTRUCT(BlueprintType)
struct FPostInfo 
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PostID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReceive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPostItem> PostItemList;
};

USTRUCT(BlueprintType)
struct FGemInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Name;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	TSubclassOf<UGemFunctionBase> Function;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGemType Type;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta=(EditCondition="Type==EGemType::Passive"))
	EGemPassiveTargetType PassiveTargetType;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<int32> LevelByExp;	
};

UCLASS(Blueprintable)
class UGemDataBase : public UObject
{
public:
	GENERATED_BODY()

	// DB ID
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 ID;

	// Table ID
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FName ItemID;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 Level;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	int32 Exp;
	
	UPROPERTY(BlueprintReadWrite, meta=(ExposeOnSpawn))
	FGemInfo Data;
};

USTRUCT(BlueprintType)
struct FNeedResourceInfo
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTypes Type;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RowType = "/Script/Magitek.ItemETC", EditCondition = "Type == EItemTypes::Etc", EditConditionHides=true) )
	FDataTableRowHandle Item;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RowType = "/Script/Magitek.GemInfo", EditCondition = "Type == EItemTypes::Gem", EditConditionHides=true) )
	FDataTableRowHandle Gem;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Count;	
};

USTRUCT(BlueprintType)
struct FGemCombinInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FNeedResourceInfo> NeedResource;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RowType = "/Script/Magitek.GemInfo") )
	FDataTableRowHandle  ResultGem;
};

USTRUCT(BlueprintType)
struct FintWithIcon
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Value = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<class UTexture2D> Icon;
};

// 캐릭터의 속성, 스텟, 등등 수치화가 가능한 것들
// 마이너스도 있음!
USTRUCT(BlueprintType)
struct FAbilityInfo
{
	GENERATED_USTRUCT_BODY()

	// 물리 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MeleeAttackPower = 0;

	// 마법 공격
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagicAttackPower = 0;

	// 물리 방어
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MeleeDefence = 0;

	// 마법 방어
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MagicDefence = 0;
	
	// 움직임 속도
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 MovementSpeed = 0;

	// 크리티컬 데미지 증가략
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalPower = 0;

	// 크리티컬 확률 증가량
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 CriticalRate = 0;
	
	// 체력
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 HP = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Cost = 0;
};

#pragma region Achievement
// 업적 획득 조건 정보
USTRUCT(BlueprintType)
struct FAchievementConditionParameterBase
{
	GENERATED_USTRUCT_BODY()
};

USTRUCT(BlueprintType)
struct FAchievementFunctionMainStageClear : public FAchievementConditionParameterBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConditionClearStage;
};

USTRUCT(BlueprintType)
struct FAchievementFunctionKillMonster : public FAchievementConditionParameterBase
{
	GENERATED_USTRUCT_BODY()
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (RowType = "/Game/Blueprints/DataTable/ST_Monster.ST_Monster"))
	FDataTableRowHandle MonsterID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConditionKillCount;
};

USTRUCT(BlueprintType)
struct FAchievementFunctionGetSpecificGem : public FAchievementConditionParameterBase
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SpecificGemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ConditionGetGemCount;
};


USTRUCT(BlueprintType)
struct FAchievementInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UTexture2D> Icon;

	// 이 업적을 얻음에 따라 증가될 어빌리티, 직접 보는게 직관적
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FAbilityInfo AbilityInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EAchivementConditionType AchivementConditionType;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AchivementConditionType==EAchivementConditionType::StageClear", EditConditionHides = true))
	FAchievementFunctionMainStageClear MainStageClearParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AchivementConditionType==EAchivementConditionType::KillSpecificMonster", EditConditionHides = true))
	FAchievementFunctionKillMonster KillSpecificMonsterParameter;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AchivementConditionType==EAchivementConditionType::KillMonster", EditConditionHides = true))
	FAchievementFunctionKillMonster KillMonsterParameter;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "AchivementConditionType==EAchivementConditionType::GetGem", EditConditionHides = true))
	FAchievementFunctionGetSpecificGem SpecificGemParameter;

};
#pragma endregion Achievement

#pragma region Item

//기타 아이템 정보
USTRUCT(BlueprintType)
struct FItemETC : public FCommonInfo
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Tier;
};

#pragma endregion Item

#pragma region Option
USTRUCT(BlueprintType)
struct FOptionInfo
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float MasterVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float BGMVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float EffectVolume = 1.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float UIVolume = 1.0f;
};

#pragma endregion Option

#pragma region Shop
USTRUCT(BlueprintType)
struct FSalesListInfo
{
	GENERATED_USTRUCT_BODY()
public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle Item;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int Count = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EItemTypes ItemType = EItemTypes::None;
};

USTRUCT(BlueprintType)
struct FShopInfo : public FCommonInfo
{
	GENERATED_USTRUCT_BODY()

public:

	// 맨 위로 올려 보낼 우선 순위
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int SortOrder = -1;

	// 판매 여부
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bSales = true;

	// 구매 화폐 타입
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	EGameCurrencyTypes GameCurrencyTypes = EGameCurrencyTypes::None;

#pragma region ShopCash

	// IOS SKU ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Cash", EditConditionHides = true))
	FString IOSSKUID = FString();

	// Android SKU ID
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Cash", EditConditionHides = true))
	FString AndroidSKUID = FString();

	// 현금 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Cash", EditConditionHides = true))
	int32 CashPrice = 0;

	// 현금 할인
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Cash", EditConditionHides = true))
	int32 CashDiscount = 0;

#pragma endregion ShopCash

	// 골드 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Gold", EditConditionHides = true))
	int32 GoldPrice = 0;

	// 골드 할인 (100퍼센트 100000, 10퍼센트 10000, 1퍼센트 1000, 0.1퍼센트 100, 0.01퍼센트 10)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::Gold", EditConditionHides = true))
	int32 GoldDiscount = 0;

	// 크리스탈 가격
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::CashCrystal", EditConditionHides = true))
	int32 CrystalPrice = 0;

	// 크리스탈 할인 (100퍼센트 100000, 10퍼센트 10000, 1퍼센트 1000, 0.1퍼센트 100, 0.01퍼센트 10)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "GameCurrencyTypes==EGameCurrencyTypes::CashCrystal", EditConditionHides = true))
	int32 CrystalDiscount = 0;

	// 구매 시간 기간 타입 (Month, Week, Day, None)
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	ETimePeriodTypes PurchasableTimePeriodType = ETimePeriodTypes::None;
	
	// 달 구매 가능 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PurchasableTimePeriodType==ETimePeriodTypes::Month", EditConditionHides = true))
	int32 MonthPurchasableCount = 0;

	// 주 구매 가능 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PurchasableTimePeriodType==ETimePeriodTypes::Week", EditConditionHides = true))
	int32 WeekPurchasableCount = 0;

	// 하루 구매 횟수
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PurchasableTimePeriodType==ETimePeriodTypes::Day", EditConditionHides = true))
	int32 DayPurchasableCount = 0;

	// 최대 구매 가능 (값이 0일 경우 무제한)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, meta = (EditCondition = "PurchasableTimePeriodType==ETimePeriodTypes::None", EditConditionHides = true))
	int32 MaximumPurchasableCount = 0;

	// 상품에 포함된 판매 목록 배열
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSalesListInfo> SalesListInfoArray;

	// 판매 종료일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 SalesEndTime = 0;

	// 할인 종료일
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 DiscountEndTime = 0;
};

#pragma endregion Shop

#pragma region Dungeon

// 던전 빌드 시 필요한 에셋 정보
USTRUCT(BlueprintType)
struct FDungeonBuildInfo : public FTableRowBase
{
	GENERATED_USTRUCT_BODY()

public:

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USnapMapModuleDatabase* SnapMapModuleDatabase;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	USnapMapAsset* SnapMapFlowGraph;

};

// 던전 정보
USTRUCT(BlueprintType)
struct FDungeonInfo : public FCommonInfo
{
	GENERATED_BODY()

	//빌드할 던전 테이블
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle DungeonBuildInfo;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TSoftObjectPtr<UDataTable> StageTable;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FName TagName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FDataTableRowHandle> Monsters;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FDataTableRowHandle BossMonster;

	// 착용된 젬이 얻을 경험치
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 Exp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FSalesListInfo> Rewards;
};

#pragma endregion Dungeon


#pragma region Development

USTRUCT(BlueprintType)
struct FServerInfo : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText ServerName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString IPAddress;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Desc;
};

#pragma endregion