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
	
	USeatingManager* SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
	TargetChair = SeatingManager->RequestSeat(this);
	
	AIController = Cast<ACatAIController>(GetController());
	if (TargetChair && AIController)
	{
		AIController->CatController = this;
		AIController->MoveToLocation(TargetChair->TargetPosition->GetComponentLocation(), 0.1f);
	}
	
	if (SpeechBubble) SpeechBubble->SetVisibility(false);
}

void ACatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	// TODO store this on BeginPlay rather than recomputing every tick
	APlayerController* PC = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (PC && PC->GetPawn())
	{
		FVector PlayerLocation = PC->GetPawn()->GetActorLocation();
		SpeechBubble->SetWorldRotation((PlayerLocation - SpeechBubble->GetComponentLocation()).Rotation());
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
			FVector DirectionToChair = (TargetChair->GetActorLocation() - GetActorLocation()).GetSafeNormal();
			FRotator FaceChairRotation = DirectionToChair.Rotation();
			FaceChairRotation.Pitch = 0;
			FaceChairRotation.Roll = 0;
			SetActorRotation(FaceChairRotation);
			if (UCatAnimInstance* AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance()))
			{
				GetCharacterMovement()->SetMovementMode(MOVE_Flying);
				AnimInstance->bIsJumping = true;
			}
		}
	}
	else
	{
		Destroy();
	}
}

void ACatController::OnJumpComplete()
{
	if (TargetChair)
	{
		GetCharacterMovement()->SetMovementMode(MOVE_Walking);
		SetActorRotation(TargetChair->GetActorRotation());
		FVector TargetLocation = TargetChair->GetActorLocation();
		TargetLocation.Z = GetActorLocation().Z;
		SetActorLocation(TargetLocation);
		if (SpeechBubble)
		{
			SpeechBubble->SetVisibility(true);
		}
		bWantsCoffee = true;
		if (UCatAnimInstance* AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsJumping = false;
		}
	}
	else
	{
		if (UCatAnimInstance* AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance()))
		{
			AnimInstance->bIsJumping = false;
		}
		GetCharacterMovement()->bOrientRotationToMovement = true;
		AIController->MoveToLocation(SpawnPosition, 5.0f);
	}
}

// TODO - the coffee will be placed on the table, and we will drink it
void ACatController::ReceiveCoffee(ACoffeeCup* CoffeeCupToGrab)
{
	// Grab coffee
	bWantsCoffee = false;
	CoffeeCup = CoffeeCupToGrab;
	AttachUtils::SetCollisionOfHeld(CoffeeCup, false);
	CoffeeCupToGrab->AttachToComponent(HoldPosition, AttachUtils::GetPickupAttachRules());
	FRotator CurrentRotation = CoffeeCupToGrab->GetActorRotation();
	CoffeeCupToGrab->SetActorRotation(FRotator(0.0f, CurrentRotation.Yaw, 0.0f));
	
	// Hide prompt
	if (SpeechBubble)
	{
		SpeechBubble->SetVisibility(false);
	}
	
	CoffeeCup->OnFillableEmpty.AddDynamic(this, &ACatController::OnCoffeeFinished);
	CoffeeCup->Drain(0.0f, 1.0f);
}

void ACatController::OnCoffeeFinished()
{

	AttachUtils::SetCollisionOfHeld(CoffeeCup, true);
	// Crash here?
	CoffeeCup->DetachFromActor(AttachUtils::GetDropDetachmentRules());
	CoffeeCup = nullptr;
	
	if (AIController)
	{
		USeatingManager* SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
		SeatingManager->ReleaseSeat(TargetChair);
		TargetChair = nullptr;
	}
	
	// Jump off the chair. In future this needs to be a different animation
	AddActorLocalRotation(FRotator(0.0f, 90.0f, 0.0f));
	if (UCatAnimInstance* AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsJumping = true;
	}
}
