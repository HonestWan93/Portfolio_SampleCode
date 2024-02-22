// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/DungeonManager.h"
#include "Kismet/GameplayStatics.h"
#include "Core/Utils/DungeonModelHelper.h"
#include "Core/Dungeon.h"
#include "Dungeon/DungeonRoomBase.h"
#include "Camera/CameraComponent.h"
#include "Frameworks/Snap/Lib/Connection/SnapConnectionComponent.h"

// Sets default values
ADungeonManager::ADungeonManager()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

}

// Called when the game starts or when spawned
void ADungeonManager::BeginPlay()
{
	Super::BeginPlay();

	RegisterDungeons();
}

//각 던전 등록
void ADungeonManager::RegisterDungeons()
{
	TArray<AActor*> outActors;
	outActors.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADungeon::StaticClass(), outActors);
	for (auto dungeonActor : outActors)
	{
		ADungeon* dungeon = Cast<ADungeon>(dungeonActor);
		Dungeons.Add(dungeon);
	}
}

//각 던전에 맞는 방들 등록
void ADungeonManager::RegisterDungeonRooms()
{
	DuegoneRooms.Empty();

	TArray<AActor*> outActors;
	outActors.Empty();

	// 해당 던전에 Room 등록
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADungeonRoomBase::StaticClass(), outActors);
	for (auto roomActor : outActors)
	{
		ADungeonRoomBase* dungeonRoom = Cast<ADungeonRoomBase>(roomActor);
		DuegoneRooms.Add(dungeonRoom);
	}

	for (auto duegoneRoom : DuegoneRooms)
	{
		// 던전 이웃 방들 등록
		duegoneRoom->RegisterNeighboringRooms();

		//던전 방에 문들 등록
		duegoneRoom->RegisterRoomDoors();
	}
}

TArray<class ADungeon*> ADungeonManager::GetDungeons() const
{
	return Dungeons;
}

TArray<ADungeonRoomBase*> ADungeonManager::GetDuegoneRooms() const
{
	return DuegoneRooms;
}

void ADungeonManager::GetDungeonRoomSnapsDirection(ADungeonRoomBase* DungeonRoom, TMap<EDirection, USnapConnectionComponent*>& OutDungeonRoomSnapsDirection) const
{
	//우선 첫 스타트 맵은 회전이 안되게 막자
    TMap<EDirection, USnapConnectionComponent*> dungeonRoomSnapsDirection;
    for (auto snapConncetion : DungeonRoom->GetDoorSnapConnectionComponents())
    {
        //door 일 경우에만
        if (snapConncetion->ConnectionState == ESnapConnectionState::Door)
        {
			//월드 기준
			// 앞을 보고 있으니까 이 스냅션은 앞에 있다
			FVector ForwardVector = snapConncetion->GetForwardVector();			
			if (ForwardVector.Equals(FVector::ForwardVector,0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Up, snapConncetion);
			}
			// 뒤를 보고 있으니까 이 스냅션은 뒤에 있다.
			else if (ForwardVector.Equals(FVector::BackwardVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Down, snapConncetion);
			}
			// 오른쪽을 보고 있으니까 이 스냅션은 오른쪽에 있다.
			else if (ForwardVector.Equals(FVector::RightVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Right, snapConncetion);
			}
			// 왼쪽을 보고 있으니까 이 스냅션은 왼쪽에 있다.
			else if (ForwardVector.Equals(FVector::LeftVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Left, snapConncetion);
			}
        }
    }

	OutDungeonRoomSnapsDirection = dungeonRoomSnapsDirection;
}