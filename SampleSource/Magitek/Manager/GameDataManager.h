// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DataTable.h"
#include "GameDataManager.generated.h"

/**
 * 
 */

UCLASS(Config = Game)
class MAGITEK_API UGameDataManagerConfig : public UObject
{
public:
	GENERATED_BODY()

	UPROPERTY(Config)
	FSoftClassPath ClassPath;
};


UCLASS(Blueprintable, BlueprintType)
class MAGITEK_API UGameDataManager : public UObject
{
	GENERATED_BODY()
public:
#pragma region DataTable GetFunction

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetItemETCDataTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetStageDataTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetGemTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetAchievementInfoTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetDungeonInfoTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetShopInfoTable();

	UFUNCTION(BlueprintPure, Category = "DataTable")
	UDataTable* GetDungeonBuildInfoTable();

#pragma endregion

public:

#pragma region Static Function

	static void ValidCheckInstance();

	static UGameDataManager* GetInstance();

#pragma endregion
public:
#pragma region DataTable

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> ItemETCTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> StageTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> GemTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> AchievementInfoTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> GetDungeonInfoTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> GetShopInfoTableRef;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "DataTable")
	TObjectPtr<UDataTable> GetDungeonBuildInfoTableRef;

#pragma endregion
};
