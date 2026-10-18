// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatCafeCharacter.h"

#include "Utils/AttachUtils.h"
#include "Animation/AnimInstance.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CatCafe.h"
#include "Interfaces/Interactable.h"
#include "UI/CatCafeHUD.h"
#include "Blueprint/UserWidget.h"

ACatCafeCharacter::ACatCafeCharacter()
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
	FirstPersonCameraComponent->SetRelativeLocationAndRotation(FVector(-2.8f, 5.89f, 0.0f),
	                                                           FRotator(0.0f, 90.0f, -90.0f));
	FirstPersonCameraComponent->bUsePawnControlRotation = true;
	FirstPersonCameraComponent->bEnableFirstPersonFieldOfView = true;
	FirstPersonCameraComponent->bEnableFirstPersonScale = true;
	FirstPersonCameraComponent->FirstPersonFieldOfView = 70.0f;
	FirstPersonCameraComponent->FirstPersonScale = 0.6f;

	// configure the character comps
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->FirstPersonPrimitiveType = EFirstPersonPrimitiveType::WorldSpaceRepresentation;

	GetCapsuleComponent()->SetCapsuleSize(34.0f, 96.0f);

	// Configure character movement
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;
	GetCharacterMovement()->AirControl = 0.5f;

	// Configure coordinates to hold objects
	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(FirstPersonCameraComponent);
	HoldPosition->SetRelativeLocation(FVector(100.0f, 0.0f, -10.f));

	InteractRaycastQueryParams.AddIgnoredActor(this);
}

void ACatCafeCharacter::BeginPlay()
{
	Super::BeginPlay();

	// Set up UI
	if (HUDClass)
	{
		HUD = CreateWidget<UCatCafeHUD>(GetWorld(), HUDClass);
		HUD->AddToViewport();
	}
}

void ACatCafeCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Freeze the object's rotation
	if (HeldObject)
	{
		FRotator CurrentRotation = HeldObject->GetActorRotation();
		HeldObject->SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	}

	// See if we are looking at an actor that we can interact with
	IInteractable* Interactable = GetInteractableActor();

	if (Interactable)
	{
		FInteractEligibility* Eligibility = Interactable->CanInteract(HeldObject);
		if (Eligibility->IsEligible)
		{
			HUD->InteractPromptText->SetText(Eligibility->PromptText);
			HUD->InteractPromptText->SetVisibility(ESlateVisibility::Visible);
			HUD->InteractIcon->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}

	HUD->InteractPromptText->SetVisibility(ESlateVisibility::Hidden);
	HUD->InteractIcon->SetVisibility(ESlateVisibility::Hidden);
}

void ACatCafeCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ACatCafeCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACatCafeCharacter::DoJumpEnd);

		// Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ACatCafeCharacter::MoveInput);

		// Looking/Aiming
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ACatCafeCharacter::LookInput);
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this,
		                                   &ACatCafeCharacter::LookInput);

		EnhancedInputComponent->BindAction(PickUpAction, ETriggerEvent::Triggered, this, &ACatCafeCharacter::Interact);
	}
	else
	{
		UE_LOG(LogCatCafe, Error,
		       TEXT(
			       "'%s' Failed to find an Enhanced Input Component! This template is built to use the Enhanced Input system. If you intend to use the legacy system, then you will need to update this C++ file."
		       ), *GetNameSafe(this));
	}
}

IInteractable* ACatCafeCharacter::GetInteractableActor() const
{
	// Get the camera location and direction
	const FVector Start = FirstPersonCameraComponent->GetComponentLocation();
	const FVector End = Start + FirstPersonCameraComponent->GetForwardVector() * 200.0f;

	// Do the raycast
	FHitResult HitResult;

	if (GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, InteractRaycastQueryParams))
	{
		AActor* HitActor = HitResult.GetActor();

		if (HitActor && HitActor->Implements<UInteractable>())
			return Cast<IInteractable>(HitActor);
	}
	return nullptr;
}

void ACatCafeCharacter::Interact()
{
	IInteractable* Interactable = GetInteractableActor();
	if (Interactable && Interactable->CanInteract(HeldObject)->IsEligible)
	{
		HeldObject = Interactable->Interact(HeldObject);
		if (HeldObject)
		{
			if (Interactable->IsPhysicsObject())
			{
				AttachUtils::SetCollisionOfHeld(HeldObject, false);
			}
			HeldObject->AttachToComponent(HoldPosition, AttachUtils::GetPickupAttachRules());
			FRotator CurrentRotation = HeldObject->GetActorRotation();
			HeldObject->SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
		}

		return;
	}

	// Drop
	if (IInteractable* HeldInteractable = Cast<IInteractable>(HeldObject))
	{
		if (HeldInteractable && HeldInteractable->IsPhysicsObject())
		{
			AttachUtils::SetCollisionOfHeld(HeldObject, true);
		}
		HeldObject->DetachFromActor(AttachUtils::GetDropDetachmentRules());
		HeldObject = nullptr;
		// TODO place at reticule (raycast) rather than in air
	}
}

void ACatCafeCharacter::MoveInput(const FInputActionValue& Value)
{
	// get the Vector2D move axis
	FVector2D MovementVector = Value.Get<FVector2D>();

	// pass the axis values to the move input
	DoMove(MovementVector.X, MovementVector.Y);
}

void ACatCafeCharacter::LookInput(const FInputActionValue& Value)
{
	// get the Vector2D look axis
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	// pass the axis values to the aim input
	DoAim(LookAxisVector.X, LookAxisVector.Y);
}

void ACatCafeCharacter::DoAim(float Yaw, float Pitch)
{
	if (GetController())
	{
		// pass the rotation inputs
		AddControllerYawInput(Yaw);
		AddControllerPitchInput(Pitch);
	}
}

void ACatCafeCharacter::DoMove(float Right, float Forward)
{
	if (GetController())
	{
		// pass the move inputs
		AddMovementInput(GetActorRightVector(), Right);
		AddMovementInput(GetActorForwardVector(), Forward);
	}
}

void ACatCafeCharacter::DoJumpStart()
{
	// pass Jump to the character
	Jump();
}

void ACatCafeCharacter::DoJumpEnd()
{
	// pass StopJumping to the character
	StopJumping();
}
