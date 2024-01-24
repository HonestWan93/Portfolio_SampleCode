// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekStructDefine.h"
#include "GameFramework/Actor.h"
#include "DungeonRoomBase.generated.h"

class ASnapConnectionActor;
class USnapConnectionComponent;
class UDungeonStreamingChunk;

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
	// Called every frame
	virtual void Tick(float DeltaTime) override;

public:
	// 던전 태그들 덮어 씌우기
	UFUNCTION(BlueprintCallable, BlueprintImplementableEvent)
	void CoverDungeonStageTagInActor();

	UFUNCTION(BlueprintPure)
	TArray<class USnapConnectionComponent*> GetSnapConnectionComponentList();

	UFUNCTION(BlueprintPure)
	TArray<class ADungeonRoomBase*> GetNeighboringRooms();

	UFUNCTION(BlueprintCallable)
	void ConvertChunksToDungeonRooms(const UClass* ActorClass);

	UFUNCTION(BlueprintCallable)
	void FindConnectedNeighboringRoom(class USnapConnectionComponent* InSnapConnectionComponent, class ADungeonRoomBase*& OutNeighboringRoom);

public:

	//방문 여부
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bVisited = false;

	//연결 지점 리스트
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<class ASnapConnectionActor*> SnapConnectionActorList;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TSet<UDungeonStreamingChunk*> Chunks;

private:

	UPROPERTY()
	TArray<class USnapConnectionComponent*> SnapConnectionComponentList;

	//이웃 던전 방
	UPROPERTY()
	TArray<class ADungeonRoomBase*> NeighboringRooms;
};
