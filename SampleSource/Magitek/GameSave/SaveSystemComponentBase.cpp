// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSave/SaveSystemComponentBase.h"
#include "Component/LocalDataComponent.h"
#include "MagitekGameInstance.h"
#include "MainPlayerControllerBase.h"
#include "Kismet/GameplayStatics.h"
#include "GameSave/SaveFunctionLibrary.h"

//파일 위치 찾기 위해 사용하는 헤더
#include "Misc/Paths.h"
#include "HAL/FileManagerGeneric.h"
//#include "HAL/PlatformFilemanager.h"

// Sets default values for this component's properties
USaveSystemComponentBase::USaveSystemComponentBase()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = false;
	// 임시 세이브 슬롯 이름 및 인덱스
	SaveSlotName = "save1";
	SaveIndex = 0;
	// ...
}


// Called when the game starts
void USaveSystemComponentBase::BeginPlay()
{
	Super::BeginPlay();

	// ...
	
}


// Called every frame
void USaveSystemComponentBase::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	// ...
}

// 세이브 로컬 데이타 
void USaveSystemComponentBase::SaveLocalData(AMainPlayerControllerBase* MainPlayerController)
{
	if (MainPlayerController == nullptr)
	{
		return;
	}

	if (MainPlayerController->GetLocalDataComponent() == nullptr)
	{
		return;
	}

	TArray<struct FPostInfo> openedPostInfoList;
	openedPostInfoList.Empty();

	ULocalDataComponent* localDataComponent = MainPlayerController->GetLocalDataComponent();

	//오픈한 우편 정보 리스트 가져오기
	localDataComponent->GetOpenedPostInfoList(openedPostInfoList);

	//없으면 세이브 생성
	if (SaveGameLocal == nullptr)
	{
		SaveGameLocal = Cast<USaveGameLocal>(UGameplayStatics::CreateSaveGameObject(USaveGameLocal::StaticClass()));
	}

	//플레이어가 오픈한 우편 정보
	SetSavePlayerPostInfoList(openedPostInfoList);

	UMagitekGameInstance* magitekGameInstance = Cast<UMagitekGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (magitekGameInstance != nullptr)
	{
		SetSaveUserNickname(magitekGameInstance->GetAccountInfo().Nickname);
	}

	//세이브
	if (UGameplayStatics::SaveGameToSlot(SaveGameLocal, SaveSlotName, SaveIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("Save Success"));
	}
}

// 세이브 데이타 로드
void USaveSystemComponentBase::LoadLocalGameData(AMainPlayerControllerBase* MainPlayerController)
{
	if (MainPlayerController == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("MainPlayerController Nullptr"));
		return;
	}

	if (MainPlayerController->GetLocalDataComponent() == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("GetLocalDataComponent nullptr"));
		return;
	}
	
	SaveGameLocal = Cast<USaveGameLocal>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, SaveIndex));

	if (SaveGameLocal == nullptr)
	{
		SaveGameLocal = Cast<USaveGameLocal>(UGameplayStatics::CreateSaveGameObject(USaveGameLocal::StaticClass()));
	}	

	//이름이 다르면 파일을 삭제하자
	UMagitekGameInstance* magitekGameInstance = Cast<UMagitekGameInstance>(UGameplayStatics::GetGameInstance(GetWorld()));
	if (magitekGameInstance != nullptr)
	{
		if (magitekGameInstance->GetAccountInfo().Nickname.Equals(SaveGameLocal->UserNickname) == false)
		{	
			FString directory = FPaths::ProjectSavedDir()+"SaveGames/";
			FString fileName = "save1.sav";
			FString completePath = directory + fileName;

			if (FPaths::ValidatePath(completePath) && FPaths::FileExists(completePath))
			{
				IFileManager& fileManager = IFileManager::Get(); FileManager.Delete(*completePath);
			}

			return;
		}
	}

	ULocalDataComponent* localDataComponent = MainPlayerController->GetLocalDataComponent();

	const TArray<FPostInfo>& playerOpenedPostInfoList = USaveFunctionLibrary::ConvertSavePostInfoListToRuntimeStructArray(SaveGameLocal->PlayerOpenedPostInfoListV1);
	for (auto playerOpenedPostInfo : playerOpenedPostInfoList)
	{
		localDataComponent->AddOpenedPostID(playerOpenedPostInfo.PostID);
		localDataComponent->AddPostInfo(playerOpenedPostInfo);
	}

	UE_LOG(LogTemp, Log, TEXT("LoadGameData Success"));
}

// 세이브 게임에 PostInfoList Set 
void USaveSystemComponentBase::SetSavePlayerPostInfoList(const TArray<struct FPostInfo>& OpenedPostInfoList)
{
	if (SaveGameLocal != nullptr)
	{
		SaveGameLocal->SetSavePlayerOpenedPostInfoList(OpenedPostInfoList);
	}
}

void USaveSystemComponentBase::SetSaveUserNickname(const FString& InSaveUserNickname)
{
	if (SaveGameLocal != nullptr)
	{
		SaveGameLocal->SetSaveUserNickname(InSaveUserNickname);
	}
}
