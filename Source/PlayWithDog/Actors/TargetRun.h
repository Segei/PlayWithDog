// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "TargetRun.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FRunnerEvent, AActor*, Target);
UCLASS()
class PLAYWITHDOG_API ATargetRun : public AActor
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditDefaultsOnly)
	int32 ScoreCost = 0;
	
	UPROPERTY(BlueprintAssignable)
	FRunnerEvent OnReadyPickUp;
	
	ATargetRun();

	UFUNCTION(BlueprintCallable)
	void PickUpTarget(AActor* Runner);
	
	UFUNCTION(BlueprintCallable)
	void Throw(FVector Direction);
	virtual void Tick(float DeltaTime) override;

	
};
