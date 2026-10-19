// Fill out your copyright notice in the Description page of Project Settings.


#include "Switch.h"
#include "ExecuteActorBash.h"

// Sets default values
ASwitch::ASwitch()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;


	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);


}

// Called when the game starts or when spawned
void ASwitch::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASwitch::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASwitch::Interact_Implementation()
{
	if (ExecuteActors.Num() == 0)
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Yellow,
			TEXT("ExecuteActorStart")
		);
	}
	for (AActor* Actor : ExecuteActors)
	{
		if (!Actor)
		{
			continue;
		}

		if (Actor->Implements<UIExecuteInterface>())
		{
			IIExecuteInterface::Execute_ExecuteAction(Actor);

			AActor* Arrow = IIExecuteInterface::Execute_GetArrow(Actor);

			if (!Arrow)
			{
				return;
			}

			OnInteractBegin.Broadcast(Arrow);

		}
	}

}

