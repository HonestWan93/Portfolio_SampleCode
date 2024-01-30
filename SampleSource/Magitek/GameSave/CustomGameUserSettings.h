// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

UCLASS()
class MAGITEK_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UCustomGameUserSettings();

	virtual void SetToDefaults() override;

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	static UCustomGameUserSettings* GetCustomGameUserSettings();

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetMasterVolume(float Value);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetBGMVolume(float Value);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetEffectVolume(float Value);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetUIVolume(float Value);

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	float GetMasterVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	float GetBGMVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	float GetEffectVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	float GetUIVolume() const;

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetMuteMasterVolume(bool bMute);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetMuteBGMVolume(bool bMute);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetMuteEffectVolume(bool bMute);

	UFUNCTION(BlueprintCallable, Category = CustomGameUserSettings)
	void SetMuteUIVolume(bool bMute);

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	bool IsMuteMasterVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	bool IsMuteBGMVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	bool IsMuteEffectVolume() const;

	UFUNCTION(BlueprintPure, Category = CustomGameUserSettings)
	bool IsMuteUIVolume() const;

protected:

	UPROPERTY(config)
	float MasterVolume;

	UPROPERTY(config)
	float BGMVolume;

	UPROPERTY(config)
	float EffectVolume;

	UPROPERTY(config)
	float UIVolume;

	UPROPERTY(config)
	bool bMuteMasterVolume;

	UPROPERTY(config)
	bool bMuteBGMVolume;

	UPROPERTY(config)
	bool bMuteEffectVolume;

	UPROPERTY(config)
	bool bMuteUIVolume;
};
