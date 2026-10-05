// Copyright Epic Games, Inc. All Rights Reserved.

#include "mondayuecppProjectCharacter.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "mondayuecppProject.h"
#include "Interface/InteractableInterface.h"

AmondayuecppProjectCharacter::AmondayuecppProjectCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
	// Create the first person mesh that will be viewed only by this character's owner
	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("First Person Mesh"));

	FirstPersonMesh->SetupAttachment(GetMesh());
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::FirstPerson;
	FirstPersonMesh->SetCollisionProfileName(FName("NoCollision"));

	// Create the Camera Component	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("First Person Camera"));
	FirstPersonCameraComponent->SetupAttachment(FirstPersonMesh, FName("head"));
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f), FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	ShowInterationUISphere = CreateDefaultSubobject<USphereComponent>(TEXT("ShowInteractUI"));
	
	InteractionSphere = CreateDefaultSubobject<USphereComponent>(TEXT("InteractSphere"));

	ShowInterationUISphere->SetupAttachment(RootComponent);
	InteractionSphere->SetupAttachment(RootComponent);

	ShowInterationUISphere->SetSphereRadius(400.f);
	InteractionSphere->SetSphereRadius(100.f);

	ShowInterationUISphere->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly
	);
	InteractionSphere->SetCollisionEnabled(
		ECollisionEnabled::QueryOnly
	);

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;
}

void AmondayuecppProjectCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{	
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &AmondayuecppProjectCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &AmondayuecppProjectCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AmondayuecppProjectCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AmondayuecppProjectCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &AmondayuecppProjectCharacter::LookInput);

		EnhancedInputComponent->BindAction(InteractAction, ETriggerEvent::Triggered, this, &AmondayuecppProjectCharacter::DoInteract);

	}
	else
	{
		UE_LOG(LogmondayuecppProject, Error, TEXT("'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."), *GetNameSafe(this));
	}


	ShowInterationUISphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnShowInteractionUIBegin);
	ShowInterationUISphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnShowInteractionUIEnd);

	InteractionSphere->OnComponentBeginOverlap.AddDynamic(this, &ThisClass::OnInteractionBegin);
	InteractionSphere->OnComponentEndOverlap.AddDynamic(this, &ThisClass::OnInteractionEnd);

}

void AmondayuecppProjectCharacter::OnInteractionBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor)
	{
		return;
	}

	if (OtherActor->Implements<UInteractableInterface>())
	{
		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				15.f,
				FColor::Cyan,
				TEXT("CurrentInteractActorFound")
			);
		}
		CurrentInteractActor = OtherActor;
	}
}

void AmondayuecppProjectCharacter::OnInteractionEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor)
	{
		return;
	}

	if (OtherActor->Implements<UInteractableInterface>())
	{
		if (OtherActor == CurrentInteractActor)
		{
			if (GEngine)
			{
				GEngine->AddOnScreenDebugMessage(
					-1,
					15.f,
					FColor::Cyan,
					TEXT("CurrentInteractActorDisFound")
				);
			}
			CurrentInteractActor = nullptr;
		}
	}
}

void AmondayuecppProjectCharacter::OnShowInteractionUIBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
}

void AmondayuecppProjectCharacter::OnShowInteractionUIEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
}


void AmondayuecppProjectCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);

}

void AmondayuecppProjectCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);

}

void AmondayuecppProjectCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void AmondayuecppProjectCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void AmondayuecppProjectCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void AmondayuecppProjectCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}

void AmondayuecppProjectCharacter::DoInteract()
{

	if (!CurrentInteractActor)
	{
		return;
	}

	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			15.f,
			FColor::Red,
			TEXT("DoInteractPressed")
		);
	}

	//if (CurrentInteractActor->Implements<UInteractableInterface>())
	//{
	//	if (IInteractableInterface* Interact = Cast<IInteractableInterface>(CurrentInteractActor))
	//	{
	//		Interact->Interact();
	//	}
	//}
	IInteractableInterface::Execute_Interact(CurrentInteractActor);
}
