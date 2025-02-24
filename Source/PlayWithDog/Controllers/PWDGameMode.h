// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "PWDGameMode.generated.h"

UCLASS(minimalapi)
class APWDGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	UPROPERTY()
	TArray<APlayerController*> Players;

	APWDGameMode();

	UFUNCTION(BlueprintCallable)
	TArray<APawn*> GetPlayerPawn();

	UFUNCTION(BlueprintCallable)
	TArray<APlayerController*> GetPlayers();
	
	virtual APlayerController* SpawnPlayerController(ENetRole InRemoteRole, const FString& Options) override;

	
};
