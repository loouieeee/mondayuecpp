// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "Components/ArrowComponent.h"
#include "IExecuteInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UIExecuteInterface : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class MONDAYUECPPPROJECT_API IIExecuteInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	//void ExecuteAction();

	//UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	//AActor* GetArrow();

	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	bool TryRegisterActivation(AActor* InstigatorActor);

	/** Starts the actual action. InstigatorActor is the Switch that started it. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	void ExecuteAction(AActor* InstigatorActor);

	/** Transform where the camera should look/move for this actor. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	FTransform GetCameraTargetTransform();

	/** Return true when the action has fully finished. */
	UFUNCTION(BlueprintNativeEvent, BlueprintCallable, Category = "Execute")
	bool IsExecutionComplete();

protected:
	//int32 ActivateCounter;
	//int32 NumToActivate;

};
