// Fill out your copyright notice in the Description page of Project Settings.


#include "Dungeon/DungeonRoomBase.h"
#include "Actor/DoorBase.h"
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

	for (auto SnapConnectionActor : SnapConnectionActors)
	{
		if (SnapConnectionActor->ConnectionComponent->ConnectionState != ESnapConnectionState::Door)
		{
			continue;
		}

		DoorSnapConnectionComponents.Add(SnapConnectionActor->ConnectionComponent);
	}	
}

TArray<class USnapConnectionComponent*> ADungeonRoomBase::GetDoorSnapConnectionComponents() const
{
	return DoorSnapConnectionComponents;
}

TArray<class ADungeonRoomBase*> ADungeonRoomBase::GetNeighboringRooms() const
{
	return NeighboringRooms;
}

TArray<class ADoorBase*> ADungeonRoomBase::GetDungeonDoors() const
{
	return DungeonDoors;
}

void ADungeonRoomBase::RegisterNeighboringRooms()
{
	for (auto chunk : Chunks)
	{
		TArray<AActor*> loadedChunkActors = chunk->GetLoadedChunkActors();
		for (auto loadedChunkActor : loadedChunkActors)
		{
			ADungeonRoomBase* room = Cast<ADungeonRoomBase>(loadedChunkActor);
			if (room != nullptr)
			{
				NeighboringRooms.Add(Cast<ADungeonRoomBase>(room));
			}
		}	
	}
}

void ADungeonRoomBase::RegisterRoomDoors()
{
	for (auto snapConnectionActor : SnapConnectionActors)
	{
		for (auto spawnedInstances : snapConnectionActor->GetSpawnedInstances())
		{
			ADoorBase* doorBase = Cast<ADoorBase>(spawnedInstances);
			if (doorBase != nullptr)
			{
				DungeonDoors.Add(doorBase);
			}
		}
	}
}

void ADungeonRoomBase::FindConnectedNeighboringRoom(class USnapConnectionComponent* InSnapConnectionComponent, class ADungeonRoomBase*& OutNeighboringRoom)
{
	//별로 좋지 않는 방법이지만 던전 플러그인 스냅션이 서로에 대한 연결 관련 정보를 가지고 있지 않다.
	//순회해서 찾아야한다..
	for (auto neighboringRoom : NeighboringRooms)
	{
		for (auto snapConnection : neighboringRoom->GetDoorSnapConnectionComponents())
		{
			//위치 값 비교
			if(InSnapConnectionComponent->GetComponentLocation().Equals(snapConnection->GetComponentLocation(), 0.1f))
			{
				OutNeighboringRoom = neighboringRoom;
				return;
			}
		}
	}
}

void ADungeonRoomBase::SetRoomClear(bool Value)
{
	bRoomClear = Value;
}

bool ADungeonRoomBase::GetRoomClear() const
{
	return bRoomClear;
}

void ADungeonRoomBase::OpenDungeonDoors()
{
	for (auto dungeonDoor : DungeonDoors)
	{
		dungeonDoor->OpenDoor();
	}
}

void ADungeonRoomBase::OpenNeighboringDungeonDoors()
{
	for (auto neighboringRoom : NeighboringRooms)
	{
		neighboringRoom->OpenDungeonDoors();
	}
}

void ADungeonRoomBase::CloseDungeonDoors()
{
	for (auto dungeonDoor : DungeonDoors)
	{
		dungeonDoor->CloseDoor();
	}
}

void ADungeonRoomBase::CloseNeighboringDungeonDoors()
{
	for (auto neighboringRoom : NeighboringRooms)
	{
		neighboringRoom->CloseDungeonDoors();
	}
}