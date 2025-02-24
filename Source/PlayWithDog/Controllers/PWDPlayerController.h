// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "PWDPlayerController.generated.h"

class APWDGameState;
class UInputMappingContext;

/**
 *
 */
UCLASS()
class PLAYWITHDOG_API APWDPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Server, Reliable)
	void RestartLevelCurrentLevel();
	
protected:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input)
	UInputMappingContext* InputMappingContext;


	virtual void BeginPlay() override;
private:
	APWDGameState* GetGameState();
	TWeakObjectPtr<APWDGameState> State;
	
};
