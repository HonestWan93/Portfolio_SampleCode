// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "GameSave/SaveSystemComponentBase.h"
#include "MainGameModeBase.generated.h"

/**
 * 
 */
UCLASS()
class MAGITEK_API AMainGameModeBase : public AGameMode
{
	GENERATED_BODY()

public:

	AMainGameModeBase();

public:

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USaveSystemComponentBase* SaveSystemComponent;
	
};
