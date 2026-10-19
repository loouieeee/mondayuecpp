// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/IExecuteInterface.h"
#include "ExecuteActorBash.generated.h"

UCLASS()
class MONDAYUECPPPROJECT_API AExecuteActorBase : public AActor, public IIExecuteInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AExecuteActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;


public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	virtual void ExecuteTask();

private:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMesh;
};
