
#include "PWDPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "PWDGameState.h"
#include "Engine/LocalPlayer.h"
#include "Kismet/GameplayStatics.h"


APWDGameState* APWDPlayerController::GetGameState()
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

void APWDPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
		GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(InputMappingContext, 0);
	}
}

void APWDPlayerController::RestartLevelCurrentLevel_Implementation()
{
	if (GetWorld())
	{
		GetWorld()->ServerTravel(UGameplayStatics::GetCurrentLevelName(GetWorld()));
	}
}