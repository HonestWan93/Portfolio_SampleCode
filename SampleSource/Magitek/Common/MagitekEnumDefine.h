// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekEnumDefine.generated.h"

UENUM(BlueprintType)
enum class EAdsType : uint8
{
	None,
	//배너
	BannerUnit,
	//전면 광고
	InterstitialUnit,
	//보상 광고
	RewardedVideoAdUnit
};

UENUM(BlueprintType)
enum class EAchivementConditionType : uint8
{
	// 스테이지 클리어
	StageClear,

	// 특정 ID의 젬을 얻으면?
	GetGem,

	// 메인 스테이지의 몬스터 처리 수에 따른 업적, 특정 몬스터 처리 수에 따른 업적
	KillMonster,
	
	// 메인 스테이지의 몬스터 처리 수에 따른 업적, 특정 몬스터 처리 수에 따른 업적
	KillSpecificMonster,

	// 한방이 최고 (잡몹, 보스몹, 던전 몹 등등?)
	OnePunch,

	// 운 좋은 녀석 (극한의 조합 확률?을 뚫었다)
	LuckyGuy
};

UENUM()
enum class EGemType : uint8
{
	Attack,
	Passive,	
};

UENUM()
enum class EGemPassiveTargetType : uint8
{
	All,
	Attack,
};

UENUM(BlueprintType)
enum class EItemTypes : uint8
{
	// 없음
	None,
	// 기타 아이템
	Etc,
	// 젬
	Gem,
	// 골드
	Gold,
	// 크리스탈(현금으로 구매한 재화)
	Crystal
};

UENUM(BlueprintType)
enum class EAbilityTypes : uint8
{
	None,
	MeleeAttackPower,
	MagicAttackPower,
	MeleeDefence,
	MagicDefence,
	MovementSpeed,
	CriticalPower,
	CriticalRate,
	HP,
	Cost,
};

//게임 화폐 타입
UENUM(BlueprintType)
enum class EGameCurrencyTypes : uint8
{
	None,
	// 캐쉬
	Cash,
	// 크리스탈
	CashCrystal,
	// 골드
	Gold	
};

// 시간 기간 타입
UENUM(BlueprintType)
enum class ETimePeriodTypes : uint8
{
	None,
	Year,
	Month,
	Week,
	Day,
	Hour
};

// 던전 룸 타입
UENUM(BlueprintType)
enum class EDungeonRoomTypes : uint8
{
	None,
	// 보스
	BossRoom,
	// 미니 보스
	MiniBossRoom,
	// 보너스
	BonusRoom,
	// 통로
	Corridor,
	// 몬스터 스폰 룸
	MonsterSpawnRoom,
	// 시작 룸
	StartRoom
};

UENUM(BlueprintType)
enum class EDirection : uint8
{
	Unknown,
	//상
	Up,
	//하
	Down,
	//좌
	Left,
	//우
	Right
};