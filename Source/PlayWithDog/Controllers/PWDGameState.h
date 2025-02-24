// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "Struct/StructHelper.h"
#include "PWDGameState.generated.h"

class APWDPlayerController;
class APWDGameMode;

class USpawnSubsystem;
/**
 * 
 */

UCLASS()
class PLAYWITHDOG_API APWDGameState : public AGameStateBase
{
	GENERATED_BODY()
	

public:
	UPROPERTY(Replicated)
	bool TargetOnScene;

	UPROPERTY(Replicated)
	bool PlayerReadyThrow;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnReplicatedUsingCurrentScore)
	TArray<FScoreStruct> CurrentScore;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FDateTime StartTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	FScoreStruct Leader;

	UPROPERTY(BlueprintReadOnly, ReplicatedUsing=OnReplicatedUsingEndGame)
	bool EndGame;

	UFUNCTION(Server, Reliable)
	void EndTime();	

	UFUNCTION(Server, Unreliable)
	void PlayerCanReadyThrow();

	UFUNCTION(Server, Unreliable)
	void PlayerUnReadyThrow();
	

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly)
	float SecondToRound = 360.f;

	
	APWDGameState();
	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	UFUNCTION(BlueprintImplementableEvent)
	void OnReplicatedUsingCurrentScore();
	UFUNCTION(BlueprintImplementableEvent)
	void OnReplicatedUsingEndGame();

private:
	FTimerHandle TimerEndRound;
	TWeakObjectPtr<USpawnSubsystem> SpawnSubsystem;
	bool LinkSpawnTarget;
	TArray<APWDPlayerController*> PlayersReady;
	TWeakObjectPtr<APWDGameMode> GameMode;

	UFUNCTION()
	void SpawnedTarget(AActor* Target);
	UFUNCTION()
	void DestroyedTarget(AActor* Target);
};
