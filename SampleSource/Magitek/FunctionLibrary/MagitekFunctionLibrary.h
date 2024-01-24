// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "MagitekFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS()
class MAGITEK_API UMagitekFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsDevelopment()
	{
#if UE_BUILD_DEVELOPMENT
		return true;
#else
		return false;
#endif
	}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	static bool IsEditor()
	{
#if WITH_EDITOR
		return true;
#else
		return false;
#endif
	}

	UFUNCTION(BlueprintPure)
	static class UGameDataManager* GetGameDataManager();
};
