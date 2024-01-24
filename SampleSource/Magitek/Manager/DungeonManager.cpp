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

	RegisterDungeon();
	//RegisterDungeonRoom();
}

//각 던전 등록
void ADungeonManager::RegisterDungeon()
{
	TArray<AActor*> outActors;
	outActors.Empty();

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADungeon::StaticClass(), outActors);
	for (auto dungeonActor : outActors)
	{
		ADungeon* dungeon = Cast<ADungeon>(dungeonActor);
		DungeonList.Add(dungeon);
	}
}

//각 던전에 맞는 던전 룸 등록
void ADungeonManager::RegisterDungeonRoom()
{
	TArray<AActor*> outActors;
	outActors.Empty();

	// 해당 던전에 Room 등록
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ADungeonRoomBase::StaticClass(), outActors);
	for (auto roomActor : outActors)
	{
		ADungeonRoomBase* dungeonRoom = Cast<ADungeonRoomBase>(roomActor);
		dungeonRoom->CoverDungeonStageTagInActor();
		DuegoneRoomList.Add(dungeonRoom);
	}
}

TArray<class ADungeon*> ADungeonManager::GetDungeonList()
{
	return DungeonList;
}

TArray<AActor*> ADungeonManager::GetDuegoneRoomList()
{
	return DuegoneRoomList;
}

void ADungeonManager::GetDungeonRoomSnapsDirection(ADungeonRoomBase* DungeonRoom, TMap<EDirection, USnapConnectionComponent*>& OutDungeonRoomSnapsDirection)
{
    TMap<EDirection, USnapConnectionComponent*> dungeonRoomSnapsDirection;
	
    for (auto snapConncetion : DungeonRoom->GetSnapConnectionComponentList())
    {
        //door 일 경우에만
        if (snapConncetion->ConnectionState == ESnapConnectionState::Door)
        {
			//월드 기준
			// 앞을 보고 있으니까 이 스냅션은 앞에 있다
			FVector forwardVector = snapConncetion->GetForwardVector();
			
			if (forwardVector.Equals(FVector::ForwardVector,0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Up, snapConncetion);
			}
			// 뒤를 보고 있으니까 이 스냅션은 뒤에 있다.
			else if (forwardVector.Equals(FVector::BackwardVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Down, snapConncetion);
			}
			// 오른쪽을 보고 있으니까 이 스냅션은 오른쪽에 있다.
			else if (forwardVector.Equals(FVector::RightVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Right, snapConncetion);
			}
			// 왼쪽을 보고 있으니까 이 스냅션은 왼쪽에 있다.
			else if (forwardVector.Equals(FVector::LeftVector, 0.1f))
			{
				dungeonRoomSnapsDirection.Add(EDirection::Left, snapConncetion);
			}
        }
    }

	OutDungeonRoomSnapsDirection = dungeonRoomSnapsDirection;
}