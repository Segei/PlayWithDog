// Fill out your copyright notice in the Description page of Project Settings.


#include "SpawnSubsystem.h"
#include "Actors/TargetRun.h"
#include "Actors/RunnerCharacter.h"

ARunnerCharacter* USpawnSubsystem::SpawnRunner(TSubclassOf<ARunnerCharacter> Class, FVector PointSpawn, float Speed, FString Name)
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if(Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return nullptr;
	}
	ARunnerCharacter* Runner = SpawnActorFromClass<ARunnerCharacter>(Class);
	Runner->SetActorLocation(PointSpawn);
	Runner->SetSpeed(Speed);
	Runner->SetName(Name);
	OnSpawnedAI.Broadcast(Runner);
	return Runner;
}

ATargetRun* USpawnSubsystem::SpawnTarget(TSubclassOf<AActor> Class, FVector PointSpawn)
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if(Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return nullptr;
	}
	ATargetRun* Target = SpawnActorFromClass<ATargetRun>(Class);
	Target->SetActorLocation(PointSpawn);
	OnSpawnedTargets.Broadcast(Target);
	return Target;
}

template <class T>
T* USpawnSubsystem::SpawnActorFromClass(TSubclassOf<AActor> ClassObject)
{
	return Cast<T>(GetWorld()->SpawnActor(ClassObject));
}
