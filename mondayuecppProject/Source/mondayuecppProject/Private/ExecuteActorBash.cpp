// Fill out your copyright notice in the Description page of Project Settings.


#include "ExecuteActorBash.h"

// Sets default values
AExecuteActorBash::AExecuteActorBash()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	StaticMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	SetRootComponent(StaticMesh);
}

// Called when the game starts or when spawned
void AExecuteActorBash::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AExecuteActorBash::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AExecuteActorBash::ExecuteTask()
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

