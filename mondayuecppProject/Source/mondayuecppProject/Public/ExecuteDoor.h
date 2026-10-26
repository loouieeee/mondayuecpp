// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "ExecuteDoor.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EDoorActivationMode : uint8
{
	AnySwitch UMETA(DisplayName = "Any Switch"),
	AllRequiredSwitches UMETA(DisplayName = "All Required Switches")
};

UENUM(BlueprintType)
enum class EDoorState : uint8
{
	Closed UMETA(DisplayName = "Closed"),
	Ready UMETA(DisplayName = "Ready To Open"),
	Opening UMETA(DisplayName = "Opening"),
	Open UMETA(DisplayName = "Open")
};

//UENUM(BlueprintType)
//enum class EActorState : uint8
//{
//	None        UMETA(DisplayName = "None"),        // 初期状態
//	Active		UMETA(DisplayName = "Active"),
//	InActive	UMETA(DisplayName = "InActive")
//};

UCLASS()
class MONDAYUECPPPROJECT_API AExecuteDoor : public AActorBase
{
	GENERATED_BODY()

public:
	AExecuteDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void Execute(float DeltaTime);

	//virtual void ExecuteAction_Implementation() override;

	//virtual AActor* GetArrow_Implementation() override;

	virtual bool TryRegisterActivation_Implementation(AActor* InstigatorActor) override;
	virtual void ExecuteAction_Implementation(AActor* InstigatorActor) override;
	virtual bool IsExecutionComplete_Implementation() override;


	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Door|Activation")
	EDoorActivationMode ActivationMode = EDoorActivationMode::AnySwitch;

	/** Used only when ActivationMode is AllRequiredSwitches. Assign actual Switch actors here. */
	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Door|Activation")
	TArray<TObjectPtr<AActor>> RequiredSwitches;

	/** Door movement duration in seconds. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Animation", meta = (ClampMin = "0.01"))
	float OpenDuration = 2.0f;

	/** Distance the door moves along its forward vector when opening. */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Door|Animation", meta = (ClampMin = "0.0"))
	float OpenDistance = 100.0f;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = "Door")
	EDoorState DoorState = EDoorState::Closed;

private:
	UPROPERTY(Transient)
	TSet<TObjectPtr<AActor>> ActivatedSwitches;

	FVector OpeningStartLocation = FVector::ZeroVector;

	FVector OpeningTargetLocation = FVector::ZeroVector;
	float OpeningElapsed = 0.0f;

//private:
//	EActorState ActorState;
//	float speed;
//	float timer;
//	bool bIsExucute;
//
//	//UPROPERTY(EditAnywhere)
//	int32 ActivateCounter;
//
//	UPROPERTY(EditAnywhere)
//	int32 NumToActivate;

	
};
