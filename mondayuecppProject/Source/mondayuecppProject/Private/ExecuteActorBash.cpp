// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteActorBash.h"

// Sets default values
AExecuteActorBase::AExecuteActorBase()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AExecuteActorBase::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExecuteActorBase::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExecuteActorBase::ExecuteTask()
{
	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Cyan,
			TEXT("ExecuteActorING")
		);
	}
}

