//// Fill out your copyright notice in the Description page of Project Settings.
//
#include "Switch.h"
#include "Interface/IExecuteInterface.h"
#include "Components/WidgetComponent.h"

ASwitch::ASwitch()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
    SetActorTickEnabled(false);

    StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    SetRootComponent(StaticMesh);

    InteractWidget = CreateDefaultSubobject<UWidgetComponent>(TEXT("InteractWidget"));
    InteractWidget->SetupAttachment(RootComponent);
}

void ASwitch::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(false);

    InteractWidget->SetVisibility(false);
}

void ASwitch::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);
    UpdateExecutionSequence();
}

void ASwitch::Interact_Implementation()
{
    // Do not start another sequence from this same switch while it owns one.
    if (bSequenceInProgress)
    {
        return;
    }

    PendingExecutionActors.Reset();
    CurrentExecutionIndex = 0;

    for (const TObjectPtr<AActor>& ActorPtr : ExecuteActors)
    {
        AActor* TargetActor = ActorPtr.Get();
        if (!IsValid(TargetActor) || !TargetActor->Implements<UIExecuteInterface>())
        {
            continue;
        }

        const bool bReadyToExecute = IIExecuteInterface::Execute_TryRegisterActivation(TargetActor, this);

        const bool bAlreadyQueued = PendingExecutionActors.ContainsByPredicate(
            [TargetActor](const TObjectPtr<AActor>& ExistingActor)
            {
                return ExistingActor.Get() == TargetActor;
            });

        if (bReadyToExecute && !bAlreadyQueued)
        {
            PendingExecutionActors.Add(TargetActor);
        }
    }

    // In AllRequiredSwitches mode, this switch may only register a requirement.
    // If no actor became ready, there is no camera sequence to play yet.
    if (PendingExecutionActors.Num() == 0)
    {
        return;
    }

    bSequenceInProgress = true;
    ActiveCameraSequenceMode = CameraSequenceMode;

    if (ActiveCameraSequenceMode == ECameraSequenceMode::SingleTarget)
    {
        AActor* CameraActor = IsValid(CameraTargetActor)
            ? CameraTargetActor.Get()
            : PendingExecutionActors[0].Get();

        RequestCameraMove(GetCameraTransformForActor(CameraActor));
    }
    else
    {
        RequestCameraMove(GetCameraTransformForActor(PendingExecutionActors[0].Get()));
    }
}

void ASwitch::ShowInteractWidget_Implementation(bool _bShow)
{
    if (!InteractWidget)
    {
        return;
    }

    InteractWidget->SetVisibility(_bShow);
}

void ASwitch::ShowHighlight_Implementation(bool _bShow)
{
    if (_bShow)
    {
        StaticMesh->SetOverlayMaterial(HighlightMaterial);

        if (GEngine)
        {

            GEngine->AddOnScreenDebugMessage(
                -1,
                2.f,
                FColor::Green,
                TEXT("Show Highlight"));
            UE_LOG(LogTemp, Warning, TEXT("%s"),
                *GetNameSafe(StaticMesh->GetOverlayMaterial()));
        }
    }

    else
    {
        StaticMesh->SetOverlayMaterial(nullptr);
    }
}

void ASwitch::NotifyCameraArrived()
{
    if (!bSequenceInProgress || !bWaitingForCamera)
    {
        return;
    }

    bWaitingForCamera = false;

    if (ActiveCameraSequenceMode == ECameraSequenceMode::Sequential)
    {
        StartCurrentActorExecution();
        return;
    }

    // SingleTarget: start all ready actors after the camera reaches the chosen target.
    for (const TObjectPtr<AActor>& ActorPtr : PendingExecutionActors)
    {
        AActor* TargetActor = ActorPtr.Get();
        if (IsValid(TargetActor) && TargetActor->Implements<UIExecuteInterface>())
        {
            IIExecuteInterface::Execute_ExecuteAction(TargetActor, this);
        }
    }

    bWaitingForExecutions = true;
    SetActorTickEnabled(true);
}

void ASwitch::NotifyCameraSequenceFinished()
{
    if (!bSequenceInProgress || !bWaitingForCameraReturn)
    {
        return;
    }

    ResetSequence();
}

FTransform ASwitch::GetCameraTransformForActor(AActor* Actor) const
{
    if (!IsValid(Actor))
    {
        return GetActorTransform();
    }

    if (Actor->Implements<UIExecuteInterface>())
    {
        return IIExecuteInterface::Execute_GetCameraTargetTransform(Actor);
    }

    return Actor->GetActorTransform();
}

void ASwitch::RequestCameraMove(const FTransform& TargetTransform)
{
    bWaitingForCamera = true;
    bWaitingForCameraReturn = false;
    SetActorTickEnabled(false);

    if (OnCameraTargetRequested.IsBound())
    {
        OnCameraTargetRequested.Broadcast(this, TargetTransform);
    }
    else
    {
        // Safe fallback: if no Blueprint camera listener is bound, still execute gameplay.
        UE_LOG(LogTemp, Warning,
            TEXT("Switch %s has no OnCameraTargetRequested listener; skipping camera movement."),
            *GetName());
        NotifyCameraArrived();
    }
}

void ASwitch::RequestCameraReturn()
{
    bWaitingForExecutions = false;
    bWaitingForCamera = false;
    bWaitingForCameraReturn = true;
    SetActorTickEnabled(false);

    if (OnCameraReturnRequested.IsBound())
    {
        OnCameraReturnRequested.Broadcast(this);
    }
    else
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Switch %s has no OnCameraReturnRequested listener; finishing without camera return."),
            *GetName());
        NotifyCameraSequenceFinished();
    }
}

void ASwitch::StartCurrentActorExecution()
{
    if (!PendingExecutionActors.IsValidIndex(CurrentExecutionIndex))
    {
        RequestCameraReturn();
        return;
    }

    AActor* TargetActor = PendingExecutionActors[CurrentExecutionIndex].Get();
    if (IsValid(TargetActor) && TargetActor->Implements<UIExecuteInterface>())
    {
        IIExecuteInterface::Execute_ExecuteAction(TargetActor, this);
    }

    bWaitingForExecutions = true;
    SetActorTickEnabled(true);
}

void ASwitch::UpdateExecutionSequence()
{
    if (!bSequenceInProgress || !bWaitingForExecutions)
    {
        return;
    }

    if (ActiveCameraSequenceMode == ECameraSequenceMode::SingleTarget)
    {
        for (const TObjectPtr<AActor>& ActorPtr : PendingExecutionActors)
        {
            AActor* TargetActor = ActorPtr.Get();
            if (!IsActorExecutionComplete(TargetActor))
            {
                return;
            }

            //RequestCameraMove(GetCameraTransformForActor(ActorPtr));

        }

        
        RequestCameraReturn();
        return;
    }

    AActor* CurrentActor = PendingExecutionActors.IsValidIndex(CurrentExecutionIndex)
        ? PendingExecutionActors[CurrentExecutionIndex].Get()
        : nullptr;

    if (!IsActorExecutionComplete(CurrentActor))
    {
        return;
    }

    bWaitingForExecutions = false;
    SetActorTickEnabled(false);
    ++CurrentExecutionIndex;

    if (PendingExecutionActors.IsValidIndex(CurrentExecutionIndex))
    {
        RequestCameraMove(
            GetCameraTransformForActor(PendingExecutionActors[CurrentExecutionIndex].Get()));
    }
    else
    {
        RequestCameraReturn();
    }
}

bool ASwitch::IsActorExecutionComplete(AActor* Actor) const
{
    if (!IsValid(Actor) || !Actor->Implements<UIExecuteInterface>())
    {
        return true;
    }

    return IIExecuteInterface::Execute_IsExecutionComplete(Actor);
}

void ASwitch::ResetSequence()
{
    SetActorTickEnabled(false);
    PendingExecutionActors.Reset();
    CurrentExecutionIndex = 0;
    bSequenceInProgress = false;
    bWaitingForCamera = false;
    bWaitingForExecutions = false;
    bWaitingForCameraReturn = false;
}
