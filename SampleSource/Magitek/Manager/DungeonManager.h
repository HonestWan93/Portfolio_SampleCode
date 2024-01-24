// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekStructDefine.h"
#include "GameFramework/Actor.h"
#include "DungeonManager.generated.h"

UCLASS()
class MAGITEK_API ADungeonManager : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonManager();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	//각 던전 등록
	UFUNCTION(BlueprintCallable)
	void RegisterDungeon();

	//각 던전에 맞는 던전 룸 등록, 던전 인포 DT에서 사용되는 태그
	UFUNCTION(BlueprintCallable)
	void RegisterDungeonRoom();

	UFUNCTION(BlueprintPure)
	TArray<class ADungeon*> GetDungeonList();

	UFUNCTION(BlueprintPure)
	TArray<AActor*> GetDuegoneRoomList();

	UFUNCTION(BlueprintCallable)
	void GetDungeonRoomSnapsDirection(class ADungeonRoomBase* DungeonRoom, TMap<EDirection, USnapConnectionComponent*>& OutDungeonRoomSnapsDirection);

public:

	UPROPERTY(BlueprintReadWrite)
	FName DungeonStageTag;

private:

	UPROPERTY()
	TArray<class ADungeon*> DungeonList;

	UPROPERTY()
	TArray<AActor*> DuegoneRoomList;

};
