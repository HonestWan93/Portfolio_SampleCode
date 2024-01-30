// Fill out your copyright notice in the Description page of Project Settings.


#include "MagitekGameInstance.h"
#include "Manager/GameDataManager.h"
#include "MagitekStructDefine.h"
#include "Kismet/GameplayStatics.h"


void UMagitekGameInstance::Init()
{
	Super::Init(); 
	UGameDataManager::ValidCheckInstance();

	InitOption();
}

void UMagitekGameInstance::AsyncLevelLoad(const FString& LevelDir, const FString& LevelName) const
{
	LoadPackageAsync(LevelDir + LevelName,
	                 FLoadPackageAsyncDelegate::CreateLambda(
		                 [=](const FName& PackageName, UPackage* LoadedPackage, EAsyncLoadingResult::Type Result)
		                 {
			                 if (Result == EAsyncLoadingResult::Succeeded)
			                 {
				                 UGameplayStatics::OpenLevel(this, FName(LevelName));
			                 }
		                 })
	                 , 0, PKG_ContainsMap);
}


void UMagitekGameInstance::SetAccountInfo(const struct FAccountInfo& InAccountInfo)
{
	AccountInfo = InAccountInfo;


	if (SaveGameBase == nullptr)
	{
		SaveGameBase = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	}

	SaveGameBase->UserNickname = AccountInfo.Guest;

	SaveBase();
}

FAccountInfo UMagitekGameInstance::GetAccountInfo()
{
	return AccountInfo;
}

void UMagitekGameInstance::InitOption()
{

	FString saveSlotName = "SaveBase";
	int32 slotIndex = 0;
	
	SaveGameBase = Cast<USaveGameBase>(UGameplayStatics::LoadGameFromSlot(saveSlotName, slotIndex));

	if (SaveGameBase == nullptr)
	{
		SaveGameBase = Cast<USaveGameBase>(UGameplayStatics::CreateSaveGameObject(USaveGameBase::StaticClass()));
	}

	if (SaveGameBase->UserNickname.IsEmpty() == false)
	{
		AccountInfo.Guest = SaveGameBase->UserNickname;
	}
}

//유저 기본 정보 저장
void UMagitekGameInstance::SaveBase()
{
	if (SaveGameBase == nullptr)
	{
		return;
	}

	FString saveSlotName = "SaveBase";
	int32 slotIndex = 0;

	if (UGameplayStatics::SaveGameToSlot(SaveGameBase, saveSlotName, slotIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("SaveBase Success"));
	}
}