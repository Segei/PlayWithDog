// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/WorldSubsystem.h"
#include "SpawnSubsystem.generated.h"

class ATargetRun;
class ARunnerCharacter;
/**
 * 
 */
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSpawnActor, AActor*, SpawnedActor);
UCLASS()
class PLAYWITHDOG_API USpawnSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FSpawnActor OnSpawnedAI;
	UPROPERTY(BlueprintAssignable)
	FSpawnActor OnSpawnedTargets;

	UFUNCTION()
	ARunnerCharacter* SpawnRunner(TSubclassOf<ARunnerCharacter> Class, FVector PointSpawn, float Speed, FString Name);

	UFUNCTION()
	ATargetRun* SpawnTarget(TSubclassOf<AActor> Class, FVector PointSpawn);


	
private:
	
	
	template<class T> 
	T* SpawnActorFromClass(TSubclassOf<AActor> ClassObject);
};


