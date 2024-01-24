// Fill out your copyright notice in the Description page of Project Settings.


#include "Manager/GameDataManager.h"


// 게임 데이터 매니저 싱글턴 인스턴스
static TObjectPtr<UGameDataManager> GGameDataManager = nullptr;

template<typename ClassType>
void LoadGameClass(FSoftClassPath& ClassName, TSubclassOf<ClassType>& GameClassRef)
{
	if (GameClassRef == nullptr)
	{
		GameClassRef = LoadClass<ClassType>(nullptr, *ClassName.ToString());

		// try redirects
		if (GameClassRef == nullptr)
		{
			ClassName.FixupCoreRedirects();
			GameClassRef = LoadClass<ClassType>(nullptr, *ClassName.ToString());
		}

		if (GameClassRef == nullptr)
		{
			GameClassRef = ClassType::StaticClass();
			UE_LOG(LogTemp, Error, TEXT("Failed to load '%s', falling back to '%s'"), *ClassName.ToString(), *GameClassRef->GetName());
		}
	}
}

// 싱글턴 인스턴스 유효성 검사
void UGameDataManager::ValidCheckInstance()
{
	// Vaild 확인
	if (GGameDataManager->IsValidLowLevel())
	{
		return;
	}

	UE_LOG(LogTemp, Log, TEXT("[%s] GameDataManager is not valid"), *FString(__FUNCTION__));

	GGameDataManager = nullptr;

	// 다시 생성
	GetInstance();
}

UGameDataManager* UGameDataManager::GetInstance()
{
	if (GGameDataManager == nullptr)
	{
		UE_LOG(LogTemp, Log, TEXT("[%s] GameDataManager is NULL!"), *FString(__FUNCTION__));

		static const FName name(TEXT("GDM"));
		GGameDataManager = FindObject<UGameDataManager>(GetTransientPackage(), *name.ToString());
		if (GGameDataManager == nullptr)
		{	
			UGameDataManagerConfig* config = NewObject<UGameDataManagerConfig>();
			FSoftClassPath softClassPath = config->ClassPath;

			FString classPath = softClassPath.ToString();

			if (softClassPath.IsValid() == false)
			{
				UE_LOG(LogTemp, Error, TEXT("[%s] Invalid GameDataManager ClassPath!"), *FString(__FUNCTION__));
				return nullptr;
			}

			// 게임 데이터 매니저 클래스 로드하기
			TSubclassOf<UGameDataManager> classRef;
			LoadGameClass<UGameDataManager>(softClassPath, classRef);
			if (classRef == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("[%s] Failed to Load GameDataManager Class! '%s'"), *FString(__FUNCTION__), *classPath);
				return nullptr;
			}

			// 게임 데이터 매니저 오브젝트 생성하기
			GGameDataManager = NewObject<UGameDataManager>(GetTransientPackage(), classRef, name);
			if (GGameDataManager == nullptr)
			{
				UE_LOG(LogTemp, Error, TEXT("[%s] Failed to create GameDataManager!"), *FString(__FUNCTION__));
				return nullptr;
			}

			// 최상위 root에 붙인다.
			GGameDataManager->AddToRoot();

			UE_LOG(LogTemp, Log, TEXT("[%s] GameDataManager was created!"), *FString(__FUNCTION__));
		}
	}

	return GGameDataManager;
}

UDataTable* UGameDataManager::GetItemETCDataTable()
{
	return ItemETCTableRef;
}


UDataTable* UGameDataManager::GetStageDataTable()
{
	return StageTableRef;
}


UDataTable* UGameDataManager::GetGemTable()
{
	return GemTableRef;
}

UDataTable* UGameDataManager::GetAchievementInfoTable()
{
	return AchievementInfoTableRef;
}

UDataTable* UGameDataManager::GetDungeonInfoTable()
{
	return GetDungeonInfoTableRef;
}

UDataTable* UGameDataManager::GetShopInfoTable()
{
	return GetShopInfoTableRef;
}

UDataTable* UGameDataManager::GetDungeonBuildInfoTable()
{
	return GetDungeonBuildInfoTableRef;
}