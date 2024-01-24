// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameSave/SaveGameBase.h"
#include "Components/ActorComponent.h"
#include "SaveSystemComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MAGITEK_API USaveSystemComponentBase : public UActorComponent
{
	GENERATED_BODY()

public:	
	// Sets default values for this component's properties
	USaveSystemComponentBase();

protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UFUNCTION(BlueprintCallable)
	void SaveLocalData(AMainPlayerControllerBase* MainPlayerController);

	UFUNCTION(BlueprintCallable)
	void LoadLocalGameData(AMainPlayerControllerBase* MainPlayerController);

	UFUNCTION(BlueprintCallable)
	void SetSavePlayerPostInfoList(const TArray<struct FPostInfo>& OpenedPostInfoList);

	UFUNCTION(BlueprintCallable)
	void SetSaveUserNickname(const FString& InSaveUserNickname);

public:
	// Save game data file name
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	FString SaveSlotName;

	// Save game data file index
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	int32 SaveIndex;

	// Save game USaveGame
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "SaveData")
	USaveGameLocal* SaveGameLocal;
};
