// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "MagitekStructDefine.h"
#include "SaveMagitekStructDefine.h"
#include "SaveGameBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGITEK_API USaveGameBase : public USaveGame
{
	GENERATED_BODY()

public:

	USaveGameBase()
	{

	}

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString UserNickname;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FString PlatformUID;

};

//로컬 게임 저장용
UCLASS()
class MAGITEK_API USaveGameLocal : public USaveGameBase
{
	GENERATED_BODY()

public:

	USaveGameLocal()
	{

	}

	void SetSavePlayerOpenedPostInfoList(const TArray<FPostInfo>& InPlayerOpenedPostInfoList);

	void SetSaveUserNickname(const FString& InUserNickname);

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<FPostInfoV1> PlayerOpenedPostInfoListV1;
};
