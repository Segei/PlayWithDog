
#include "PWDGameState.h"

#include "PWDGameMode.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/ScoreSubsystem.h"
#include "Subsystems/SpawnSubsystem.h"

void APWDGameState::PlayerCanReadyThrow_Implementation()
{
	PlayerReadyThrow = true;
}

void APWDGameState::PlayerUnReadyThrow_Implementation()
{
	PlayerReadyThrow = false;
}


APWDGameState::APWDGameState()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void APWDGameState::BeginPlay()
{
	Super::BeginPlay();
	ENetMode Mode = GetWorld()->GetNetMode();
	if (Mode != NM_ListenServer && Mode != NM_DedicatedServer && Mode != NM_Standalone)
	{
		SetActorTickEnabled(false);
		return;
	}
	SetActorTickEnabled(true);
	StartTime = FDateTime::Now();
	GameMode = Cast<APWDGameMode>(GetWorld()->GetAuthGameMode());
	GetWorld()->GetTimerManager().SetTimer(TimerEndRound, this, &APWDGameState::EndTime, SecondToRound);
}

void APWDGameState::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	if (!SpawnSubsystem.IsValid())
	{
		SpawnSubsystem = GetWorld()->GetSubsystem<USpawnSubsystem>();
	}
	if (SpawnSubsystem.IsValid() && !LinkSpawnTarget)
	{
		SpawnSubsystem->OnSpawnedTargets.AddDynamic(this, &APWDGameState::SpawnedTarget);
		LinkSpawnTarget = true;
	}
}

void APWDGameState::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APWDGameState, TargetOnScene);
	DOREPLIFETIME(APWDGameState, CurrentScore);
	DOREPLIFETIME(APWDGameState, StartTime);
	DOREPLIFETIME(APWDGameState, PlayerReadyThrow);
	DOREPLIFETIME(APWDGameState, EndGame);
	DOREPLIFETIME(APWDGameState, Leader);
	
}

void APWDGameState::SpawnedTarget(AActor* Target)
{
	Target->OnDestroyed.AddDynamic(this, &APWDGameState::DestroyedTarget);
	TargetOnScene = true;
	PlayerReadyThrow = false;
}

void APWDGameState::DestroyedTarget(AActor* Target)
{
	Target->OnDestroyed.RemoveDynamic(this, &APWDGameState::DestroyedTarget);
	TargetOnScene = false;
}

void APWDGameState::EndTime_Implementation()
{
	Leader = Cast<UScoreSubsystem>(GetWorld()->GetSubsystem<UScoreSubsystem>())->GetLeaderScore();
	EndGame = true;
}


