// Fill out your copyright notice in the Description page of Project Settings.


#include "MagitekFunctionLibrary.h"
#include "Manager/GameDataManager.h"

UGameDataManager* UMagitekFunctionLibrary::GetGameDataManager()
{
	return UGameDataManager::GetInstance();
}