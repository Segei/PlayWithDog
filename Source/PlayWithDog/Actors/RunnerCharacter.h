// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "RunnerCharacter.generated.h"


class UBlackboardComponent;

UCLASS()
class PLAYWITHDOG_API ARunnerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	float Distance;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, ReplicatedUsing=OnReplicatedUsingName)
	FString NameDog;

	TWeakObjectPtr<UBlackboardComponent> Blackboard;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	bool PickUp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Replicated)
	float VelocitySpeed;

	FTimerHandle AnimationPickUp;
	
	ARunnerCharacter();

	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void SetSpeed(float Speed);

	UFUNCTION(BlueprintCallable)
	void SetTarget(AActor* Target);

	UFUNCTION(BlueprintCallable)
	void SetCanRunning(bool CanRunning);

	UFUNCTION()
	void StopWalk(AActor* Actor);

	UFUNCTION(Server, Reliable)
	void SetName(const FString& Name);

	UFUNCTION(BlueprintImplementableEvent)
	void OnReplicatedUsingName();

	UFUNCTION(Server, Reliable)
	void PlayAnimationPickUp();

	UFUNCTION(Server, Reliable)
	void EndAnimationPickUp();

protected:
	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;
	virtual void Tick(float DeltaSeconds) override;
};
