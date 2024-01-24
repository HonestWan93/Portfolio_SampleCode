// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MagitekStructDefine.h"
#include "SaveMagitekStructDefine.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "SaveFunctionLibrary.generated.h"

/**
 * 
 */
UCLASS(Blueprintable, BlueprintType)
class MAGITEK_API USaveFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:

#pragma region Post
	UFUNCTION(BlueprintPure)
	static TArray<FPostInfoV1> ConvertPostInfoListToSaveStructArray(const TArray<FPostInfo>& PostInfoList);
	
	UFUNCTION(BlueprintPure)
	static TArray<FPostInfo> ConvertSavePostInfoListToRuntimeStructArray(const TArray<FPostInfoV1>& PostInfoListV1);
	
	UFUNCTION(BlueprintPure)
	static FPostInfoV1 ConvertPostInfoToSaveStruct(const FPostInfo& FPostInfo);
	
	UFUNCTION(BlueprintPure)
	static FPostInfo ConvertSavePostInfoToRuntimeStruct(const FPostInfoV1& PostInfoV1);

	UFUNCTION(BlueprintPure)
	static TArray<FPostItemV1> ConvertPostItemArrayToSaveStructArray(TArray<FPostItem> PostItemArray);

	UFUNCTION(BlueprintPure)
	static TArray<FPostItem> ConvertSavePostItemArrayToRuntimeStructArray(TArray<FPostItemV1> PostItemV1Array);
	
	UFUNCTION(BlueprintPure)
	static FPostItemV1 ConvertPostItemToSaveStruct(const FPostItem& PostItem);
	
	UFUNCTION(BlueprintPure)
	static FPostItem ConvertSavePostItemToRuntimeStruct(const FPostItemV1& PostItemV1);
	
#pragma endregion 
};
