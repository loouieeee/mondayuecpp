// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteDoor.h"

AExecuteDoor::AExecuteDoor()
{
}

void AExecuteDoor::BeginPlay()
{
	Super::BeginPlay();

	ActorState = EActorState::InActive;
	speed = 50.f;
	ActivateCounter = 0;
}

// Called every frame
void AExecuteDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Execute(DeltaTime);
}

void AExecuteDoor::Execute(float DeltaTime)
{
	if (ActorState == EActorState::Active)
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Green,
				TEXT("ExecuteStart")
			);
		}

		timer += DeltaTime;

		FVector Location = GetActorLocation();
		Location += GetActorForwardVector() * speed * DeltaTime;
		SetActorLocation(Location);

		if (timer >= 2.f)
		{
			//bIsExucute = false;	
			ActorState = EActorState::InActive;
		}
	}

	ActivateCounter = 0;
}

void AExecuteDoor::ExecuteAction_Implementation()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Green,
			TEXT("ExecuteDoor")
		);
	}
	if (ActorState == EActorState::Active)
	{
		return;
	}

	timer = 0;
	ActivateCounter++;

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Green,
			TEXT("ActivateCounter " + ActivateCounter)
		);
	}

	if (ActivateCounter >= NumToActivate)
	{
		ActorState = EActorState::Active;
	}
	//bIsExucute = true;
}

AActor* AExecuteDoor::GetArrow_Implementation()
{
	return Arrow;
}


