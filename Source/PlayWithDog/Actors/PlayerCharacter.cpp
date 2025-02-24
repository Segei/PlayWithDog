#include "PlayerCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "InputActionValue.h"
#include "TargetRun.h"
#include "Controllers/PWDGameState.h"
#include "Engine/LocalPlayer.h"
#include "Net/UnrealNetwork.h"
#include "Subsystems/SpawnSubsystem.h"

DEFINE_LOG_CATEGORY(LogTemplateCharacter);

APlayerCharacter::APlayerCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(-10.f, 0.f, 60.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeLocation(FVector(-30.f, 0.f, -150.f));

	PointSpawnTarget = CreateDefaultSubobject<USceneComponent>("Point");
	PointSpawnTarget->SetupAttachment(FirstPersonCameraComponent);
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	GetMesh1P()->SetHiddenInGame(true);
	GetMesh()->SetHiddenInGame(true);

	if (GetLocalRole() == ENetRole::ROLE_SimulatedProxy)
	{
		GetMesh()->SetHiddenInGame(false);
	}
	if (GetLocalRole() == ENetRole::ROLE_AutonomousProxy)
	{
		GetMesh1P()->SetHiddenInGame(false);
	}
}

void APlayerCharacter::GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	DOREPLIFETIME(APlayerCharacter, ThrowTargetType);
	DOREPLIFETIME(APlayerCharacter, ReadyToThrow);
}

TSubclassOf<ATargetRun> APlayerCharacter::GetClassTarget()
{
	switch (ThrowTargetType)
	{
	case EThrowType::Cube:
		return Cube;
	case EThrowType::Sphere:
		return Sphere;
	default:
		return nullptr;
	}
}

void APlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		EnhancedInputComponent->BindAction(ThrowAction, ETriggerEvent::Started, this, &APlayerCharacter::Throw);

		EnhancedInputComponent->BindAction(TakeCubeAction, ETriggerEvent::Started, this, &APlayerCharacter::TakeCube);
		EnhancedInputComponent->BindAction(TakeSphereAction, ETriggerEvent::Started, this,
		                                   &APlayerCharacter::TakeSphere);

		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Move);

		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &APlayerCharacter::Look);
	}
}


void APlayerCharacter::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddMovementInput(GetActorForwardVector(), MovementVector.Y);
		AddMovementInput(GetActorRightVector(), MovementVector.X);
	}
}

void APlayerCharacter::Look(const FInputActionValue& Value)
{
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void APlayerCharacter::TakeCube_Implementation(const FInputActionValue& Value)
{
	SetTypeTarget(EThrowType::Cube);
}

void APlayerCharacter::TakeSphere_Implementation(const FInputActionValue& Value)
{
	SetTypeTarget(EThrowType::Sphere);
}

void APlayerCharacter::SetTypeTarget(EThrowType Type)
{
	if (!GetGameState())
	{
		return;
	}
	if ((State->PlayerReadyThrow && !ReadyToThrow) || State->TargetOnScene || State->EndGame)
	{
		return;
	}

	if (ThrowTargetType != Type)
	{
		ThrowTargetType = Type;
		State->PlayerCanReadyThrow();
		ReadyToThrow = true;
		return;
	}
	ThrowTargetType = EThrowType::None;
	State->PlayerUnReadyThrow();
	ReadyToThrow = false;
}


APWDGameState* APlayerCharacter::GetGameState()
{
	if (!State.IsValid())
	{
		State = Cast<APWDGameState>(GetWorld()->GetGameState());
	}
	if(!State.IsValid())
	{
		UE_LOG(LogTemp, Error, TEXT("GameState not valid"));
	}
	return State.Get();
}

void APlayerCharacter::Throw_Implementation(const FInputActionValue& Value)
{
	USpawnSubsystem* Spawner = GetWorld()->GetSubsystem<USpawnSubsystem>();
	UClass* ClassTarget = GetClassTarget();
	GetGameState();
	bool CanSpawnTarget = !State->TargetOnScene;
	bool EndGame = State->EndGame;
	if (ClassTarget != nullptr && CanSpawnTarget && ReadyToThrow && !EndGame)
	{
		ThrowTargetType = EThrowType::None;
		ReadyToThrow = false;
		ATargetRun* Target = Spawner->SpawnTarget(ClassTarget, PointSpawnTarget->GetComponentLocation());
		Target->Throw(PointSpawnTarget->GetForwardVector() * 1000.f);
	}
}
