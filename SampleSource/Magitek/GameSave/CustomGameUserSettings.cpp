// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameUserSettings.h"

UCustomGameUserSettings::UCustomGameUserSettings()
{

}

void UCustomGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();
	FrameRateLimit = 60.0f;	
}

UCustomGameUserSettings* UCustomGameUserSettings::GetCustomGameUserSettings()
{
	return Cast<UCustomGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}