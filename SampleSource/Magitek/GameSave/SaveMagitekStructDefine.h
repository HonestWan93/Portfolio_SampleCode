// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "SaveMagitekStructDefine.generated.h"

USTRUCT(BlueprintType)
struct FPostItemV1
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 ItemCount;
};

USTRUCT(BlueprintType)
struct FPostInfoV1
{
	GENERATED_USTRUCT_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 PostID;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Title;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FString Description;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	int32 EndTimestamp;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool HasItem;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool IsReceive;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	TArray<FPostItemV1> PostItemList;
};

