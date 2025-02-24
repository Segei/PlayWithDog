#include "RunnerCharacter.h"

#include "AIController.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Net/UnrealNetwork.h"


ARunnerCharacter::ARunnerCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	SetActorTickEnabled(false);
}

void ARunnerCharacter::BeginPlay()
{
	Super::BeginPlay();
	ENetMode Mode = GetWorld()->GetNetMode();
	if(Mode == NM_ListenServer || Mode == NM_DedicatedServer)
	{
		Blackboard = Cast<AAIController>(GetController())->GetBlackboardComponent();
		SetActorTickEnabled(true);
	}
}

void ARunnerCharacter::SetSpeed(float Speed)
{
	GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void ARunnerCharacter::SetTarget(AActor* Target)
{
	Blackboard->SetValueAsObject("TargetActor", Target);
	Target->OnDestroyed.AddDynamic(this, &ARunnerCharacter::StopWalk);
}

void ARunnerCharacter::SetCanRunning(bool CanRunning)
{
	Blackboard->SetValueAsBool("CanRunning", CanRunning);
}

void ARunnerCharacter::StopWalk(AActor* Actor)
{
	Actor->OnDestroyed.RemoveDynamic(this, &ARunnerCharacter::StopWalk);
	GetCharacterMovement()->StopActiveMovement();

}

void ARunnerCharacter::SetName_Implementation(const FString& Name)
{
	NameDog = Name;
}

void ARunnerCharacter::PlayAnimationPickUp_Implementation()
{
	GetWorld()->GetTimerManager().ClearTimer(AnimationPickUp);
	GetWorld()->GetTimerManager().SetTimer(AnimationPickUp, this, &ARunnerCharacter::EndAnimationPickUp, 1.f);
	PickUp = true;
}

void ARunnerCharacter::EndAnimationPickUp_Implementation()
{
	PickUp = false;
}

void ARunnerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(ARunnerCharacter, NameDog);
}

void ARunnerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
	VelocitySpeed = GetCharacterMovement()->Velocity.Length();
}
