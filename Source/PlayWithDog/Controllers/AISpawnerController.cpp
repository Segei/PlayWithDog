#include "AISpawnerController.h"

#include "NavigationSystem.h"
#include "Actors/RunnerCharacter.h"
#include "Actors/TargetRun.h"
#include "Kismet/KismetMathLibrary.h"
#include "Subsystems/SpawnSubsystem.h"


void AAISpawnerController::BeginPlay()
{
	Super::BeginPlay();
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer)
	{
		return;
	}
	TArray<FString> TempPool = NamePool;
	UNavigationSystemV1* Navigation = UNavigationSystemV1::GetNavigationSystem(GetWorld());
	USpawnSubsystem* SpawnSubsystem = GetWorld()->GetSubsystem<USpawnSubsystem>();
	for (int i = 0; i < CountAIToLevel; i++)
	{
		FNavLocation Point;
		Navigation->GetRandomPoint(Point);
		FString Name = TempPool[UKismetMathLibrary::RandomIntegerInRange(0, TempPool.Num() - 1)];
		TempPool.Remove(Name);
		ARunnerCharacter* Runner = SpawnSubsystem->SpawnRunner(ClassRunner, Point.Location,
		                                                       FMath::RandRange(MinSpeed, MaxSpeed), Name);
		Runners.Add(Runner);
	}
	SpawnSubsystem->OnSpawnedTargets.AddDynamic(this, &AAISpawnerController::SpawnTarget);
}

void AAISpawnerController::SpawnTarget(AActor* Target)
{
	for (ARunnerCharacter* Runner : Runners)
	{
		Runner->SetCanRunning(false);
		Runner->SetTarget(Target);
	}

	Cast<ATargetRun>(Target)->OnReadyPickUp.AddDynamic(this, &AAISpawnerController::TargetReady);
}

void AAISpawnerController::TargetReady(AActor* Target)
{
	Cast<ATargetRun>(Target)->OnReadyPickUp.RemoveDynamic(this, &AAISpawnerController::TargetReady);
	TMap<ARunnerCharacter*, float> DistanceToTarget;
	for (ARunnerCharacter* Runner : Runners)
	{
		DistanceToTarget.Add(Runner, Runner->Distance);
	}
	DistanceToTarget.ValueStableSort([](float a, float b)
	{
		return a > b;
	});

	TArray<ARunnerCharacter*> TempRunners;
	DistanceToTarget.GetKeys(TempRunners);
	for (int i = 1; i < TempRunners.Num(); i++)
	{
		TempRunners[i]->SetCanRunning(true);
	}
}
