// Fill out your copyright notice in the Description page of Project Settings.


#include "CustomGameUserSettings.h"

UCustomGameUserSettings::UCustomGameUserSettings()
{

}

void UCustomGameUserSettings::SetToDefaults()
{
	Super::SetToDefaults();

	FrameRateLimit = 60.0f;	
	MasterVolume = 1.0f;
	BGMVolume = 1.0f;
	EffectVolume = 1.0f;
	UIVolume = 1.0f;

	bMuteMasterVolume = false;
	bMuteBGMVolume = false;
	bMuteEffectVolume = false;
	bMuteUIVolume = false;
}

UCustomGameUserSettings* UCustomGameUserSettings::GetCustomGameUserSettings()
{
	return Cast<UCustomGameUserSettings>(UGameUserSettings::GetGameUserSettings());
}

#pragma region SoundOption

void UCustomGameUserSettings::SetMasterVolume(float Value)
{
	MasterVolume = Value;
}

void UCustomGameUserSettings::SetBGMVolume(float Value)
{
	BGMVolume = Value;
}

void UCustomGameUserSettings::SetEffectVolume(float Value)
{
	EffectVolume = Value;
}

void UCustomGameUserSettings::SetUIVolume(float Value)
{
	UIVolume = Value;
}

float UCustomGameUserSettings::GetMasterVolume() const
{
	return MasterVolume;
}

float UCustomGameUserSettings::GetBGMVolume() const
{
	return BGMVolume;
}

float UCustomGameUserSettings::GetEffectVolume() const
{
	return EffectVolume;
}

float UCustomGameUserSettings::GetUIVolume() const
{
	return UIVolume;
}

void UCustomGameUserSettings::SetMuteMasterVolume(bool bMute)
{
	bMuteMasterVolume = bMute;
}

void UCustomGameUserSettings::SetMuteBGMVolume(bool bMute)
{
	bMuteBGMVolume = bMute;
}

void UCustomGameUserSettings::SetMuteEffectVolume(bool bMute)
{
	bMuteEffectVolume = bMute;
}

void UCustomGameUserSettings::SetMuteUIVolume(bool bMute)
{
	bMuteUIVolume = bMute;
}

bool UCustomGameUserSettings::IsMuteMasterVolume() const
{
	return bMuteMasterVolume;
}

bool UCustomGameUserSettings::IsMuteBGMVolume() const
{
	return bMuteBGMVolume;
}

bool UCustomGameUserSettings::IsMuteEffectVolume() const
{
	return bMuteEffectVolume;
}

bool UCustomGameUserSettings::IsMuteUIVolume() const
{
	return bMuteUIVolume;
}

#pragma endregion SoundOption