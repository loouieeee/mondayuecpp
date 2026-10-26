// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "interface/IExecuteInterface.h"
#include "ActorBase.generated.h"

UCLASS()
class MONDAYUECPPPROJECT_API AActorBase : public AActor , public IIExecuteInterface
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AActorBase();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	//virtual void ExecuteAction_Implementation() override;

	//virtual AActor* GetArrow_Implementation() override;

	virtual bool TryRegisterActivation_Implementation(AActor* InstigatorActor) override;
	virtual void ExecuteAction_Implementation(AActor* InstigatorActor) override;
	virtual FTransform GetCameraTargetTransform_Implementation() override;
	virtual bool IsExecutionComplete_Implementation() override;


protected:
	UPROPERTY(EditAnywhere)
	AActor* Arrow;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	TObjectPtr<UStaticMeshComponent> StaticMesh;

	/** Place/rotate this arrow in the child Blueprint to choose the camera target. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Camera")
	TObjectPtr<UArrowComponent> CameraTargetArrow;

};
