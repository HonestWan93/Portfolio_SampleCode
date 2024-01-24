// Fill out your copyright notice in the Description page of Project Settings.


#include "GameSave/SaveGameBase.h"
#include "GameSave/SaveFunctionLibrary.h"


void USaveGameLocal::SetSavePlayerOpenedPostInfoList(const TArray<FPostInfo>& InPlayerOpenedPostInfoList)
{
	PlayerOpenedPostInfoListV1 = USaveFunctionLibrary::ConvertPostInfoListToSaveStructArray(InPlayerOpenedPostInfoList);
}

void USaveGameLocal::SetSaveUserNickname(const FString& InUserNickname)
{
	UserNickname = InUserNickname;
}