// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameUserSettings.h"
#include "CustomGameUserSettings.generated.h"

/**
 * 
 */
UCLASS()
class MAGITEK_API UCustomGameUserSettings : public UGameUserSettings
{
	GENERATED_BODY()

public:
	UCustomGameUserSettings();

	virtual void SetToDefaults() override;

	UFUNCTION(BlueprintCallable)
	static UCustomGameUserSettings* GetCustomGameUserSettings();
};
