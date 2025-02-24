// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Object.h"
#include "StructHelper.generated.h"

/**
 * 
 */
USTRUCT(BlueprintType)
struct FScoreStruct
{
	GENERATED_BODY()
	UPROPERTY(BlueprintReadWrite)
	FString RunnerName;
	UPROPERTY(BlueprintReadWrite)
	int32 Score;
};