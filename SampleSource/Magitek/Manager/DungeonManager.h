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

	//던전들 등록
	UFUNCTION(BlueprintCallable)
	void RegisterDungeons();

	//던전에 맞는 던전 방들 등록
	UFUNCTION(BlueprintCallable)
	void RegisterDungeonRooms();

	UFUNCTION(BlueprintPure)
	TArray<class ADungeon*> GetDungeons() const;

	UFUNCTION(BlueprintPure)
	TArray<ADungeonRoomBase*> GetDuegoneRooms() const;

	UFUNCTION(BlueprintCallable)
	void GetDungeonRoomSnapsDirection(class ADungeonRoomBase* DungeonRoom, TMap<EDirection, USnapConnectionComponent*>& OutDungeonRoomSnapsDirection) const;

public:

	UPROPERTY(BlueprintReadWrite)
	FName DungeonStageTag;

private:

	UPROPERTY()
	TArray<class ADungeon*> Dungeons;

	UPROPERTY()
	TArray<ADungeonRoomBase*> DuegoneRooms;

};
