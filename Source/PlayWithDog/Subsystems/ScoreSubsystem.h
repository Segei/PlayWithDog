// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RunnerCharacter.h"
#include "Struct/StructHelper.h"
#include "Subsystems/WorldSubsystem.h"
#include "ScoreSubsystem.generated.h"

class APWDGameState;
class USpawnSubsystem;
class ARunnerCharacter;
/**
 * 
 */

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FChangeScore, FScoreStruct, Score);

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FScoreEvent);

UCLASS()
class PLAYWITHDOG_API UScoreSubsystem : public UWorldSubsystem
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FChangeScore OnUpdateScore;

	UPROPERTY(BlueprintAssignable)
	FChangeScore OnAddScore;

	UPROPERTY(BlueprintAssignable)
	FScoreEvent OnClearScore;

	TMap<FString, int32> CurrentScore;

	TWeakObjectPtr<APWDGameState> GameState;
	
	UFUNCTION(BlueprintCallable)
	void AddScore(ARunnerCharacter* Runner, int32 Count = 0);
	
	UFUNCTION()
	void AddRunner(AActor* Runner);
	
	UFUNCTION()
	void RemoveRunner(AActor* Runner);

	UFUNCTION(BlueprintCallable)
	FScoreStruct GetLeaderScore();

	UFUNCTION()
	void ClearScore();

	virtual void OnWorldBeginPlay(UWorld& InWorld) override;

	virtual void BeginDestroy() override;

	void UpdateScore();

private:
	TWeakObjectPtr<USpawnSubsystem> SpawnSubsystem;
	
	void SortScore();
};
