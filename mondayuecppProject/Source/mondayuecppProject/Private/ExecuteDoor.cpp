// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteDoor.h"

AExecuteDoor::AExecuteDoor()
{
    PrimaryActorTick.bCanEverTick = true;
    PrimaryActorTick.bStartWithTickEnabled = false;
    SetActorTickEnabled(false);
}

void AExecuteDoor::BeginPlay()
{
    Super::BeginPlay();
    SetActorTickEnabled(false);
}

void AExecuteDoor::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    if (DoorState != EDoorState::Opening)
    {
        SetActorTickEnabled(false);
        return;
    }

    OpeningElapsed += DeltaTime;
    const float SafeDuration = FMath::Max(OpenDuration, KINDA_SMALL_NUMBER);
    const float Alpha = FMath::Clamp(OpeningElapsed / SafeDuration, 0.0f, 1.0f);

    SetActorLocation(FMath::Lerp(OpeningStartLocation, OpeningTargetLocation, Alpha));

    if (Alpha >= 1.0f)
    {
        DoorState = EDoorState::Open;
        SetActorTickEnabled(false);
    }


		//FVector OpeningElapsed = GetActorLocation();
		//OpeningElapsed += GetActorForwardVector() * 5 * DeltaTime;
		//SetActorLocation(OpeningElapsed);

		//if (timer >= 2.f)
		//{
		//	//bIsExucute = false;	
		//	ActorState = EActorState::InActive;
		//}
}

bool AExecuteDoor::TryRegisterActivation_Implementation(AActor* InstigatorActor)
{
    if (!IsValid(InstigatorActor) || DoorState != EDoorState::Closed)
    {
        return false;
    }

    if (ActivationMode == EDoorActivationMode::AnySwitch)
    {
        DoorState = EDoorState::Ready;
        return true;
    }

    // AllRequiredSwitches mode: a switch must be explicitly listed on this door.
    bool bIsRequiredSwitch = false;
    for (const TObjectPtr<AActor>& RequiredSwitchPtr : RequiredSwitches)
    {
        if (RequiredSwitchPtr.Get() == InstigatorActor)
        {
            bIsRequiredSwitch = true;
            break;
        }
    }

    if (RequiredSwitches.Num() == 0 || !bIsRequiredSwitch)
    {
        return false;
    }

    // TSet prevents the same switch from being counted twice.
    ActivatedSwitches.Add(InstigatorActor);

    bool bHasAtLeastOneValidRequirement = false;
    for (const TObjectPtr<AActor>& RequiredSwitchPtr : RequiredSwitches)
    {
        AActor* RequiredSwitch = RequiredSwitchPtr.Get();
        if (!IsValid(RequiredSwitch))
        {
            continue;
        }

        bHasAtLeastOneValidRequirement = true;

        bool bRequirementActivated = false;
        for (const TObjectPtr<AActor>& ActivatedSwitchPtr : ActivatedSwitches)
        {
            if (ActivatedSwitchPtr.Get() == RequiredSwitch)
            {
                bRequirementActivated = true;
                break;
            }
        }

        if (!bRequirementActivated)
        {
            return false;
        }
    }

    if (!bHasAtLeastOneValidRequirement)
    {
        UE_LOG(LogTemp, Warning,
            TEXT("Door %s uses AllRequiredSwitches but has no valid RequiredSwitches."),
            *GetName());
        return false;
    }

    DoorState = EDoorState::Ready;
    return true;
}

void AExecuteDoor::ExecuteAction_Implementation(AActor* InstigatorActor)
{
    if (DoorState != EDoorState::Ready)
    {
        return;
    }

    DoorState = EDoorState::Opening;
    OpeningElapsed = 0.0f;
    OpeningStartLocation = GetActorLocation();
    OpeningTargetLocation = OpeningStartLocation + GetActorForwardVector() * OpenDistance;

    SetActorTickEnabled(true);
}

bool AExecuteDoor::IsExecutionComplete_Implementation()
{
    return DoorState == EDoorState::Open;
}