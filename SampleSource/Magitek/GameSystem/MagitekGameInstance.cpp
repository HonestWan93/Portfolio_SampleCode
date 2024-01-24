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


	if (SaveGameOption == nullptr)
	{
		SaveGameOption = Cast<USaveGameOption>(UGameplayStatics::CreateSaveGameObject(USaveGameOption::StaticClass()));
	}

	SaveGameOption->UserNickname = AccountInfo.Guest;

	SaveOption();
}

FAccountInfo UMagitekGameInstance::GetAccountInfo()
{
	return AccountInfo;
}

void UMagitekGameInstance::InitOption()
{

	FString saveSlotName = "SaveOption";
	int32 slotIndex = 0;
	
	SaveGameOption = Cast<USaveGameOption>(UGameplayStatics::LoadGameFromSlot(saveSlotName, slotIndex));

	if (SaveGameOption == nullptr)
	{
		SaveGameOption = Cast<USaveGameOption>(UGameplayStatics::CreateSaveGameObject(USaveGameOption::StaticClass()));
	}

	if (SaveGameOption->UserNickname.IsEmpty() == false)
	{
		AccountInfo.Guest = SaveGameOption->UserNickname;
	}
}

//옵션 정보 저장
void UMagitekGameInstance::SaveOption()
{
	if (SaveGameOption == nullptr)
	{
		return;
	}

	FString saveSlotName = "SaveOption";
	int32 slotIndex = 0;

	if (UGameplayStatics::SaveGameToSlot(SaveGameOption, saveSlotName, slotIndex))
	{
		UE_LOG(LogTemp, Log, TEXT("SaveOption Success"));
	}
}

void UMagitekGameInstance::SetOptionInfo(const FOptionInfo& OptionInfo)
{
	if (SaveGameOption == nullptr)
	{
		return;
	}

	SaveGameOption->OptionInfo = OptionInfo;
}

FOptionInfo UMagitekGameInstance::GetOptionInfo()
{
	if (SaveGameOption == nullptr)
	{
		return FOptionInfo();
	}

	return SaveGameOption->OptionInfo;
}