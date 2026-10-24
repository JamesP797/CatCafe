// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat/CatController.h"

#include "CatAIController.h"
#include "CatAnimInstance.h"
#include "Components/SkeletalMeshComponent.h"
#include "Components/WidgetComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Objects/Chair.h"
#include "Objects/CoffeeCup.h"
#include "Subsystems/SeatingManager.h"
#include "Utils/AttachUtils.h"
#include "Engine/GameInstance.h"
#include "NavigationSystem.h"
#include "Kismet/GameplayStatics.h"

ACatController::ACatController()
{
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACatAIController::StaticClass();
	
	SpeechBubble = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechBubble"));
	SpeechBubble->SetupAttachment(RootComponent);
	
	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(RootComponent);
}

void ACatController::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
	TargetChair = SeatingManager->RequestSeat(this);
	
	AIController = Cast<ACatAIController>(GetController());
	if (TargetChair && AIController)
	{
		AIController->CatController = this;
		AIController->MoveToLocation(TargetChair->TargetPosition->GetComponentLocation(), 0.1f);
	}
	
	if (SpeechBubble) SpeechBubble->SetVisibility(false);
	
	PlayerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	
	AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance());
}

void ACatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (PlayerController && PlayerController->GetPawn() && bWantsCoffee)
	{
		const FVector PlayerLocation = PlayerController->GetPawn()->GetActorLocation();
		SpeechBubble->SetWorldRotation((PlayerLocation - SpeechBubble->GetComponentLocation()).Rotation());
	}
	
	if (bTurning)
	{
		const FRotator CurrentRotation = GetActorRotation();
		if (!TargetRotation.IsZero())
		{
			const FRotator NewRotation = FMath::RInterpTo(CurrentRotation, TargetRotation, DeltaTime, 3.0f);
			SetActorRotation(NewRotation);
		}
		
		const FVector CurrentLocation = GetActorLocation();
		if (!TargetLocation.IsZero())
		{
			const FVector NewLocation = FMath::VInterpTo(CurrentLocation, TargetLocation, DeltaTime, 10.0f);
			SetActorLocation(NewLocation);
		}
		
		if ((TargetRotation.IsZero() || CurrentRotation.Equals(TargetRotation, 1.0f)) && (TargetLocation.IsZero() || CurrentLocation.Equals(TargetLocation, 8.0f)))
		{
			if (!bOnChair)
			{
				// Trigger jump
				GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				if (AnimInstance) AnimInstance->bIsJumping = true;
			}
			else if (bOnChair && !bHadCoffee)
			{
			}
			else if (bOnChair && bHadCoffee)
			{
				// Trigger jump off chair
				if (AnimInstance) AnimInstance->bIsJumping = true;
			}
			bTurning = false;
			TargetLocation = FVector(0.0f, 0.0f, 0.0f);
			TargetRotation = FRotator(0.0f, 0.0f, 0.0f);
		}
	}
}

FInteractEligibility* ACatController::CanInteract(AActor* HeldObject)
{
	if (ACoffeeCup* CoffeeCupToGrab = Cast<ACoffeeCup>(HeldObject))
	{
		return new FInteractEligibility(!CoffeeCup 
			// && CoffeeCupToGrab->IsFull() 
			&& bWantsCoffee, 
			FText::FromString("Give coffee"));
	}
	return new FInteractEligibility(!HeldObject, FText::FromString("Pick up"));
}

AActor* ACatController::Interact(AActor* HeldObject)
{
	if (ACoffeeCup* CoffeeCupToGrab = Cast<ACoffeeCup>(HeldObject))
	{
		ReceiveCoffee(CoffeeCupToGrab);
		return nullptr;
	}
	return this;
}

void ACatController::ArriveAtLocation()
{
	if (!bHadCoffee)
	{
		if (TargetChair)
		{
			GetCharacterMovement()->bOrientRotationToMovement = false;
			const FVector DirectionToChair = (TargetChair->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FRotator FaceChairRotation = DirectionToChair.Rotation();
			FaceChairRotation.Pitch = 0;
			FaceChairRotation.Roll = 0;
			// SetActorRotation(FaceChairRotation);
			TargetRotation = FaceChairRotation;
			bTurning = true;
		}
	}
	else Destroy();
}

void ACatController::OnJumpComplete()
{
	if (TargetChair)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		bTurning = true;
		TargetLocation = TargetChair->GetActorLocation();
		TargetLocation.Z = GetActorLocation().Z;
		TargetRotation = TargetChair->GetActorRotation();
		
		if (AnimInstance) AnimInstance->bIsJumping = false;
		if (AnimInstance) AnimInstance->bIsSitting = true;
		bOnChair = true;
	}
	else
	{
		if (AnimInstance) AnimInstance->bIsJumping = false;
		GetCharacterMovement()->bOrientRotationToMovement = true;
		if (AIController) AIController->MoveToLocation(SpawnPosition, 5.0f);
	}
}

void ACatController::OnSitComplete()
{
	if (SpeechBubble) SpeechBubble->SetVisibility(true);
	bWantsCoffee = true;
}

void ACatController::ReceiveCoffee(ACoffeeCup* CoffeeCupToGrab)
{
	// Grab coffee
	bWantsCoffee = false;
	if (AnimInstance) AnimInstance->bIsDrinking = true;
	CoffeeCup = CoffeeCupToGrab;
	AttachUtils::SetCollisionOfHeld(CoffeeCup, false);
	CoffeeCupToGrab->AttachToComponent(HoldPosition, AttachUtils::GetPickupAttachRules());
	FRotator CurrentRotation = CoffeeCupToGrab->GetActorRotation();
	CoffeeCupToGrab->SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	
	// Hide prompt
	if (SpeechBubble) SpeechBubble->SetVisibility(false);
	
	CoffeeCup->OnFillableEmpty.AddDynamic(this, &ACatController::OnCoffeeFinished);
	CoffeeCup->Drain(0.0f, 1.0f);
}

void ACatController::OnCoffeeFinished()
{
	CoffeeCup->OnFillableEmpty.RemoveDynamic(this, &ACatController::OnCoffeeFinished);
	AttachUtils::SetCollisionOfHeld(CoffeeCup, true);
	CoffeeCup->DetachFromActor(AttachUtils::GetDropDetachmentRules());
	CoffeeCup = nullptr;
	
	if (AIController && SeatingManager)
	{
		SeatingManager->ReleaseSeat(TargetChair);
		TargetChair = nullptr;
	}
	
	bHadCoffee = true;
	TargetRotation = (FRotator(0.0f, 90.0f, 0.0f).Quaternion() * GetActorRotation().Quaternion()).Rotator();
	bTurning = true;
	if (AnimInstance)
	{
		AnimInstance->bIsSitting = false;
		AnimInstance->bIsDrinking = false;
	}
}
