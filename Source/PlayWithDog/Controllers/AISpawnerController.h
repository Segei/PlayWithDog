// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/Actor.h"
#include "AISpawnerController.generated.h"

class ARunnerCharacter;

UCLASS()
class PLAYWITHDOG_API AAISpawnerController : public AActor
{
	GENERATED_BODY()

public:
	TArray<FString> NamePool = {
		TEXT("Дарман"), TEXT("Цезарь"), TEXT("Одиссей"), TEXT("Эмир"), TEXT("Анда"), TEXT("Терра"), TEXT("Эльба"),
		TEXT("Герда"), TEXT("Эрнест"), TEXT("Раймонд"), TEXT("Флойд"), TEXT("Честер"), TEXT("Джек"), TEXT("Энджел"),
		TEXT("Милли"), TEXT("Вера"), TEXT("Нелли")
	};

	UPROPERTY(EditAnywhere, meta=(ClampMin = 2, ClampMax = 10))
	int32 CountAIToLevel = 2;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MinSpeed = 200.f;

	UPROPERTY(EditAnywhere, meta=(ClampMin="0", UIMin="0", ForceUnits="cm/s"))
	float MaxSpeed = 400.f;

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ARunnerCharacter> ClassRunner;

	UPROPERTY(EditDefaultsOnly)
	UBlackboardComponent* Blackboard;

	UPROPERTY(VisibleAnywhere)
	TArray<ARunnerCharacter*> Runners;

	virtual void BeginPlay() override;

	UFUNCTION()
	void SpawnTarget(AActor* Target);

	UFUNCTION()
	void TargetReady(AActor* Target);
};
