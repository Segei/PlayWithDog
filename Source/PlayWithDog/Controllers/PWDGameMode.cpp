
#include "PWDGameMode.h"
#include "Actors/PlayerCharacter.h"
#include "UObject/ConstructorHelpers.h"

APWDGameMode::APWDGameMode()
	: Super()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/FirstPerson/Blueprints/BP_FirstPersonCharacter"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

}

TArray<APawn*> APWDGameMode::GetPlayerPawn()
{
	TArray<APawn*> Pawns;
	for (APlayerController* Player : Players)
	{
		Pawns.Add(Player->GetPawn());
	}
	return Pawns;
}

TArray<APlayerController*> APWDGameMode::GetPlayers()
{
	return Players;
}

APlayerController* APWDGameMode::SpawnPlayerController(ENetRole InRemoteRole, const FString& Options)
{
	APlayerController* PlayerController = Super::SpawnPlayerController(InRemoteRole, Options);
	Players.Add(PlayerController);
	return PlayerController;
}
