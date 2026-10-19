// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ActorBase.h"
#include "ExecuteDoor.generated.h"

/**
 * 
 */

UENUM(BlueprintType)
enum class EActorState : uint8
{
	None        UMETA(DisplayName = "None"),        // 初期状態
	Active		UMETA(DisplayName = "Active"),
	InActive	UMETA(DisplayName = "InActive")
};

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

	virtual void ExecuteAction_Implementation() override;

	virtual AActor* GetArrow_Implementation() override;

private:
	EActorState ActorState;
	float speed;
	float timer;
	bool bIsExucute;

	//UPROPERTY(EditAnywhere)
	int32 ActivateCounter;

	UPROPERTY(EditAnywhere)
	int32 NumToActivate;

	
};
