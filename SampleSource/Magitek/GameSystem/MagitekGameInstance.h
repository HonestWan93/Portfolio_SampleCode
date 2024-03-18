// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSave/SaveGameBase.h"
#include "Engine/GameInstance.h"
#include "Kismet/BlueprintPlatformLibrary.h"
#include "MagitekGameInstance.generated.h"

/**
 * 
 */

UCLASS()
class MAGITEK_API UMagitekGameInstance : public UPlatformGameInstance
{
	GENERATED_BODY()

public:	

	virtual void Init() override;

	UFUNCTION(BlueprintCallable)
	void AsyncLevelLoad(const FString& LevelDir, const FString& LevelName) const;

	UFUNCTION(BlueprintCallable)
	void SetAccountInfo(const struct FAccountInfo& InAccountInfo);

	UFUNCTION(BlueprintPure)
	FAccountInfo GetAccountInfo();

	UFUNCTION(BlueprintPure)
	FString GetAccountUID();

	UFUNCTION(BlueprintCallable)
	void SaveBase();

private:

	struct FAccountInfo AccountInfo;

	UPROPERTY()
	USaveGameBase* SaveGameBase;
private:

	UFUNCTION(BlueprintCallable)
	void InitOption();
};
