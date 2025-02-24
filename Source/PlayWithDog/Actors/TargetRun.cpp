

#include "TargetRun.h"

#include "RunnerCharacter.h"
#include "Subsystems/ScoreSubsystem.h"


ATargetRun::ATargetRun()
{
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>("Mesh");
	SetRootComponent(Mesh);
	
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
	SetReplicates(true);
	SetReplicateMovement(true);
}

void ATargetRun::PickUpTarget(AActor* Runner)
{
	ARunnerCharacter* RunnerCharacter = Cast<ARunnerCharacter>(Runner);
	RunnerCharacter->PlayAnimationPickUp();
	GetWorld()->GetSubsystem<UScoreSubsystem>()->AddScore(RunnerCharacter, ScoreCost);
	Destroy();
}

void ATargetRun::Throw(FVector Direction)
{
	Mesh->AddImpulse(Direction);
	SetActorTickEnabled(true);
}

void ATargetRun::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	float Velocity = Mesh->GetComponentVelocity().Length();
	if(Velocity <= 1.f && Velocity != 0.f)
	{
		SetActorTickEnabled(false);
		OnReadyPickUp.Broadcast(this);
	}
}

