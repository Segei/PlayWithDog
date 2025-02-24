// Fill out your copyright notice in the Description page of Project Settings.


#include "ScoreSubsystem.h"
#include "SpawnSubsystem.h"
#include "Controllers/PWDGameState.h"

void UScoreSubsystem::AddScore(ARunnerCharacter* Runner, int32 Count)
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return;
	}
	if (!CurrentScore.Contains(Runner->NameDog))
	{
		CurrentScore.Add(Runner->NameDog, Count);
	}
	CurrentScore[Runner->NameDog] += Count;
	OnUpdateScore.Broadcast(FScoreStruct(Runner->NameDog, CurrentScore[Runner->NameDog]));
	UpdateScore();
}

void UScoreSubsystem::AddRunner(AActor* Runner)
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return;
	}
	Runner->OnDestroyed.AddDynamic(this, &UScoreSubsystem::RemoveRunner);
	if (ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(Runner))
	{
		AddScore(RunnerCharacter);
		OnAddScore.Broadcast(FScoreStruct(RunnerCharacter->NameDog, 0));
	}
}

void UScoreSubsystem::RemoveRunner(AActor* Runner)
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return;
	}
	Runner->OnDestroyed.RemoveDynamic(this, &UScoreSubsystem::RemoveRunner);
	if (ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(Runner))
	{
		CurrentScore.Remove(RunnerCharacter->NameDog);
	}
}

FScoreStruct UScoreSubsystem::GetLeaderScore()
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return FScoreStruct();
	}
	SortScore();
	TArray<FString> Runners;
	CurrentScore.GetKeys(Runners);
	FString RunnerName = CurrentScore[Runners[0]] == CurrentScore[Runners[1]] ? TEXT("Ничья") : Runners[0];
	return FScoreStruct(RunnerName, CurrentScore[Runners[0]]);
}

void UScoreSubsystem::ClearScore()
{
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return;
	}
	CurrentScore.Empty();
	OnClearScore.Broadcast();
}

void UScoreSubsystem::OnWorldBeginPlay(UWorld& InWorld)
{
	Super::OnWorldBeginPlay(InWorld);
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		return;
	}
	GameState = Cast<APWDGameState>(GetWorld()->GetGameState());
	SpawnSubsystem = GetWorld()->GetSubsystem<USpawnSubsystem>();
	if (SpawnSubsystem.IsValid())
	{
		SpawnSubsystem.Get()->OnSpawnedAI.AddDynamic(this, &UScoreSubsystem::AddRunner);
	}
}

void UScoreSubsystem::BeginDestroy()
{
	Super::BeginDestroy();
	if (SpawnSubsystem.IsValid())
	{
		SpawnSubsystem.Get()->OnSpawnedAI.RemoveDynamic(this, &UScoreSubsystem::AddRunner);
	}
}

void UScoreSubsystem::UpdateScore()
{
	if (!GameState.IsValid())
	{
		return;
	}
	SortScore();
	TArray<FScoreStruct> ResultScore;
	for (TTuple<FString, int32> Score : CurrentScore)
	{
		ResultScore.Add(FScoreStruct(Score.Key, Score.Value));
	}
	GameState->CurrentScore = ResultScore;
}

void UScoreSubsystem::SortScore()
{
	CurrentScore.ValueSort([](int32 a, int32 b)
	{
		return a > b;
	});
}
