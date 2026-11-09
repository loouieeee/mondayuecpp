//// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Interface/InteractableInterface.h"
#include "Switch.generated.h"

class ASwitch;

UENUM(BlueprintType)
enum class ECameraSequenceMode : uint8
{
    SingleTarget UMETA(DisplayName = "All Actors / Single Camera Target"),
    Sequential UMETA(DisplayName = "Sequentially Show Each Actor")
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(
    FOnCameraTargetRequested,
    ASwitch*, SwitchActor,
    FTransform, TargetTransform
);

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(
    FOnCameraReturnRequested,
    ASwitch*, SwitchActor
);

UCLASS()
class MONDAYUECPPPROJECT_API ASwitch : public AActor, public IInteractableInterface
{
    GENERATED_BODY()

public:
    ASwitch();

    virtual void BeginPlay() override;
    virtual void Tick(float DeltaTime) override;
    virtual void Interact_Implementation() override;

    virtual void ShowInteractWidget_Implementation(bool _bShow) override;
    virtual void ShowHighlight_Implementation(bool _bShow) override;

    /** Call from Blueprint when camera has arrived at the requested target. */
    UFUNCTION(BlueprintCallable, Category = "Camera Sequence")
    void NotifyCameraArrived();

    /** Call from Blueprint after the camera has returned to the player. */
    UFUNCTION(BlueprintCallable, Category = "Camera Sequence")
    void NotifyCameraSequenceFinished();

    UFUNCTION(BlueprintPure, Category = "Camera Sequence")
    bool IsCameraSequenceInProgress() const { return bSequenceInProgress; }

    /** Blueprint moves the camera to this transform, then calls NotifyCameraArrived. */
    UPROPERTY(BlueprintAssignable, Category = "Camera Sequence")
    FOnCameraTargetRequested OnCameraTargetRequested;

    /** Blueprint returns the camera to the player, then calls NotifyCameraSequenceFinished. */
    UPROPERTY(BlueprintAssignable, Category = "Camera Sequence")
    FOnCameraReturnRequested OnCameraReturnRequested;

protected:
    UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
    TObjectPtr<UStaticMeshComponent> StaticMesh;

    /** Actors controlled by this switch. These should implement ExecuteInterface. */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Switch|Targets")
    TArray<TObjectPtr<AActor>> ExecuteActors;

    /** Optional explicit target for SingleTarget mode. If unset, the first ready actor is used. */
    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Switch|Camera")
    TObjectPtr<AActor> CameraTargetActor;

    UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = "Switch|Camera")
    ECameraSequenceMode CameraSequenceMode = ECameraSequenceMode::SingleTarget;

private:
    FTransform GetCameraTransformForActor(AActor* Actor) const;
    void RequestCameraMove(const FTransform& TargetTransform);
    void RequestCameraReturn();
    void StartCurrentActorExecution();
    void UpdateExecutionSequence();
    bool IsActorExecutionComplete(AActor* Actor) const;
    void ResetSequence();

    UPROPERTY(Transient)
    TArray<TObjectPtr<AActor>> PendingExecutionActors;

    int32 CurrentExecutionIndex = 0;
    ECameraSequenceMode ActiveCameraSequenceMode = ECameraSequenceMode::SingleTarget;
    bool bSequenceInProgress = false;
    bool bWaitingForCamera = false;
    bool bWaitingForExecutions = false;
    bool bWaitingForCameraReturn = false;


    UPROPERTY(EditDefaultsOnly, Category = "Highlight")
    UMaterialInterface* HighlightMaterial;

    UPROPERTY(VisibleAnywhere, Category = "UI")
    class UWidgetComponent* InteractWidget;
};

