// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/DungeonRoomBase.h"
#include "Frameworks/Snap/Lib/Connection/SnapConnectionComponent.h"
#include "Frameworks/Snap/Lib/Connection/SnapConnectionActor.h"
#include "Frameworks/LevelStreaming/DungeonLevelStreamingModel.h"



// Sets default values
ADungeonRoomBase::ADungeonRoomBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

// Called when the game starts or when spawned
void ADungeonRoomBase::BeginPlay()
{
	Super::BeginPlay();

	for (auto snapConnectionActor : SnapConnectionActorList)
	{
		SnapConnectionComponentList.Add(snapConnectionActor->ConnectionComponent);
	}
	
}

// Called every frame
void ADungeonRoomBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

TArray<class USnapConnectionComponent*> ADungeonRoomBase::GetSnapConnectionComponentList()
{
	return SnapConnectionComponentList;
}

TArray<class ADungeonRoomBase*> ADungeonRoomBase::GetNeighboringRooms()
{
	return NeighboringRooms;
}

void ADungeonRoomBase::ConvertChunksToDungeonRooms(const UClass* ActorClass)
{
	for (auto chunk : Chunks)
	{
		AActor* room = chunk->GetLoadedChunkActorOfType(ActorClass);
		if (room != nullptr)
		{
			NeighboringRooms.Add(Cast<ADungeonRoomBase>(room));
		}
	}
}

void ADungeonRoomBase::FindConnectedNeighboringRoom(class USnapConnectionComponent* InSnapConnectionComponent, class ADungeonRoomBase*& OutNeighboringRoom)
{
	// 던전 플러그인 스냅션이 서로에 대한 연결 관련 정보를 가지고 있지 않다.
	for (auto neighboringRoom : NeighboringRooms)
	{
		for (auto snapConnection : neighboringRoom->GetSnapConnectionComponentList())
		{			
			if (snapConnection->ConnectionState != ESnapConnectionState::Door)
			{
				continue;
			}

			//위치 값 비교
			if(InSnapConnectionComponent->GetComponentLocation().Equals(snapConnection->GetComponentLocation(), 0.1f))
			{
				OutNeighboringRoom = neighboringRoom;
				return;
			}
		}
	}
}