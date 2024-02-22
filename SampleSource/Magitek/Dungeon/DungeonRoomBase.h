// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekStructDefine.h"
#include "GameFramework/Actor.h"
#include "DungeonRoomBase.generated.h"

class ASnapConnectionActor;
class USnapConnectionComponent;
class UDungeonStreamingChunk;
class ADoorBase;

UCLASS()
class MAGITEK_API ADungeonRoomBase : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ADungeonRoomBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintPure)
	TArray<class USnapConnectionComponent*> GetDoorSnapConnectionComponents() const;

	UFUNCTION(BlueprintPure)
	TArray<class ADungeonRoomBase*> GetNeighboringRooms() const;

	UFUNCTION(BlueprintPure)
	TArray<class ADoorBase*> GetDungeonDoors() const;

	UFUNCTION(BlueprintCallable)
	void RegisterNeighboringRooms();

	UFUNCTION(BlueprintCallable)
	void RegisterRoomDoors();

	UFUNCTION(BlueprintCallable)
	void FindConnectedNeighboringRoom(class USnapConnectionComponent* InSnapConnectionComponent, class ADungeonRoomBase*& OutNeighboringRoom);

	UFUNCTION(BlueprintCallable)
	void SetRoomClear(bool Value);

	UFUNCTION(BlueprintPure)
	bool GetRoomClear() const;

	UFUNCTION(BlueprintCallable)
	void OpenDungeonDoors();

	UFUNCTION(BlueprintCallable)
	void OpenNeighboringDungeonDoors();

	UFUNCTION(BlueprintCallable)
	void CloseDungeonDoors();

	UFUNCTION(BlueprintCallable)
	void CloseNeighboringDungeonDoors();

public:

	//연결 지점 리스트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ASnapConnectionActor*> SnapConnectionActors;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<UDungeonStreamingChunk*> Chunks;

	// 클리어 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bRoomClear = false;

private:	

	UPROPERTY()
	TArray<class USnapConnectionComponent*> DoorSnapConnectionComponents;

	//이웃 던전 방
	UPROPERTY()
	TArray<class ADungeonRoomBase*> NeighboringRooms;

	//던전 방 안에 문들
	UPROPERTY()
	TArray<class ADoorBase*> DungeonDoors;
};
