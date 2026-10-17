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

// Sets default values
ACatController::ACatController()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;
	AIControllerClass = ACatAIController::StaticClass();
	
	SpeechBubble = CreateDefaultSubobject<UWidgetComponent>(TEXT("SpeechBubble"));
	SpeechBubble->SetupAttachment(RootComponent);
	
	HoldPosition = CreateDefaultSubobject<USceneComponent>(TEXT("HoldPosition"));
	HoldPosition->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACatController::BeginPlay()
{
	Super::BeginPlay();
	
	bUseControllerRotationYaw = false;
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f);
	
	// TODO make a waypoint manager, will be useful once we want to get a random table
	// TArray<AActor*> Waypoints;
	// UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("CounterWaypoint"), Waypoints);
	// if (Waypoints.Num()) TargetLocation = Waypoints[0];
	
	USeatingManager* SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
	
	TargetChair = SeatingManager->RequestSeat(this);
	
	AIController = Cast<ACatAIController>(GetController());
	if (TargetChair && AIController)
	{
		AIController->MoveToLocation(TargetChair->TargetPosition->GetComponentLocation(), 0.1f);
		AIController->CatController = this;
	}
	
	if (SpeechBubble)
	{
		SpeechBubble->SetVisibility(false);
	}
}

// Called every frame
void ACatController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* ACatController::CanInteract(AActor* HeldObject)
{
	if (ACoffeeCup* CoffeeCupToGrab = Cast<ACoffeeCup>(HeldObject))
	{
		return new FInteractEligibility(!CoffeeCup 
			&& CoffeeCupToGrab->IsFull() 
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

// TODO rename to chair
void ACatController::ArriveAtCounter()
{
	if (SpeechBubble)
	{
		SpeechBubble->SetVisibility(true);
	}
	// need to face chair first
	GetCharacterMovement()->bOrientRotationToMovement = false;
	FVector DirectionToChair = (TargetChair->GetActorLocation() - GetActorLocation()).GetSafeNormal();
	FRotator FaceChairRotation = DirectionToChair.Rotation();
	FaceChairRotation.Pitch = 0;
	FaceChairRotation.Roll = 0;
	SetActorRotation(FaceChairRotation);
	if (UCatAnimInstance* AnimInstance = Cast<UCatAnimInstance>(GetMesh()->GetAnimInstance()))
	{
		AnimInstance->bIsJumping = true;
	}
	bWantsCoffee = true;
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
	
	// Move away (in future to table)
	// TArray<AActor*> Waypoints;
	// UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("ExitWaypoint"), Waypoints);
	// if (Waypoints.Num()) TargetLocation = Waypoints[0];
	
	if (TargetChair && AIController)
	{
		AIController->MoveToLocation(TargetChair->TargetPosition->GetComponentLocation(), 0.1f);
		AIController->CatController = this;
	}
	
	// Hide prompt
	if (SpeechBubble)
	{
		SpeechBubble->SetVisibility(false);
	}
}
