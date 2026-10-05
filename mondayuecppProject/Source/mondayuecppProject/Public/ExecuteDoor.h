// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "ExecuteActorBash.h"
#include "ExecuteDoor.generated.h"

/**
 * 
 */
UCLASS()
class MONDAYUECPPPROJECT_API AExecuteDoor : public AExecuteActorBash
{
	GENERATED_BODY()

public:
	AExecuteDoor();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void ExecuteTask() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;




	
};
