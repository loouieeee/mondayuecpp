// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/SphereComponent.h"
#include "Interface/InteractableInterface.h"
#include "Switch.generated.h"

class AExecuteActorBase;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnInteractBegin,AActor*,InteractActor);

UCLASS()
class MONDAYUECPPPROJECT_API ASwitch : public AActor, public IInteractableInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASwitch();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;

	//UPROPERTY(EditAnywhere)
	//AActor* ExecuteActor = nullptr;

	UPROPERTY(EditAnywhere)
	//<AActor*> ExecuteActor = { nullptr };
	TArray<TObjectPtr<AActor>> ExecuteActors;

public:

	UPROPERTY(BlueprintAssignable, Category = "Interact")
	FOnInteractBegin OnInteractBegin;

	virtual void Interact_Implementation() override;

};
