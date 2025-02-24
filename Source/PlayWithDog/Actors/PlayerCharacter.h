
#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Logging/LogMacros.h"
#include "PlayerCharacter.generated.h"

class APWDGameState;
class ATargetRun;
class UInputComponent;
class USkeletalMeshComponent;
class UCameraComponent;
class UInputAction;
class UInputMappingContext;
struct FInputActionValue;

UENUM(BlueprintType)
enum EThrowType
{
	None,
	Cube,
	Sphere
};

DECLARE_LOG_CATEGORY_EXTERN(LogTemplateCharacter, Log, All);

UCLASS(config=Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()
	DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FPlayerEvent, APlayerCharacter*, Player);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Mesh, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh1P;

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* PointSpawnTarget;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* JumpAction;
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* ThrowAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* TakeCubeAction;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category=Input, meta=(AllowPrivateAccess = "true"))
	UInputAction* TakeSphereAction;

public:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, ReplicatedUsing=OnReplicatedUsingTypeChange)
	TEnumAsByte<EThrowType> ThrowTargetType = EThrowType::None;

	UPROPERTY(Replicated)
	bool ReadyToThrow;
	
	APlayerCharacter();
	
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Input, meta = (AllowPrivateAccess = "true"))
	class UInputAction* LookAction;
	
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }

protected:

	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);


	UFUNCTION(Server, Unreliable)
	void Throw(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void TakeCube(const FInputActionValue& Value);

	UFUNCTION(Server, Unreliable)
	void TakeSphere(const FInputActionValue& Value);

	void SetTypeTarget(EThrowType Type);
	
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;

	virtual void BeginPlay();

	UFUNCTION(BlueprintImplementableEvent)
	void OnReplicatedUsingTypeChange();

	virtual void GetLifetimeReplicatedProps(TArray<FLifetimeProperty>& OutLifetimeProps) const override;

private:
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATargetRun> Cube;
	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<ATargetRun> Sphere;

	TWeakObjectPtr<APWDGameState> State;
	
	TSubclassOf<ATargetRun> GetClassTarget();
	APWDGameState* GetGameState();
};
