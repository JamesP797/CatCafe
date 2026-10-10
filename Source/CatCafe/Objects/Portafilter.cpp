// Fill out your copyright notice in the Description page of Project Settings.


#include "Portafilter.h"

#include "CoffeeMachine.h"
#include "CoffeeCup.h"
#include "Tamper.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
APortafilter::APortafilter()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Portafilter = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Portafilter"));
	RootComponent = Portafilter;
	
	ContentsSurface->SetupAttachment(Portafilter);
}

void APortafilter::BeginPlay()
{
	Super::BeginPlay();
}

void APortafilter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* APortafilter::CanInteract(AActor* HeldObject)
{
	// We can interact if we are not in the coffee machine, or if we are in the coffee machine and it is not pouring
	// Although when we are in the coffee machine raycasts might be disabled anyway?
	const bool CanInteractInCoffeeMachine = CoffeeMachine 
		&& !CoffeeMachine->CoffeeCup->Transferring 
		&& !CoffeeMachine->CoffeeCup->IsFull();
	if (CanInteractInCoffeeMachine)
	{
		return new FInteractEligibility(true, FText::FromString("Retrieve"));
	}
	if (ATamper* Tamper = Cast<ATamper>(HeldObject))
	{
		// TODO figure out why we can't tamp while in the grinder. Collider issue?
		const bool CanTamp = IsFull() && !IsTamped && !CoffeeMachine;
		return new FInteractEligibility(CanTamp, FText::FromString("Tamp"));
	}
	return new FInteractEligibility(!HeldObject, FText::FromString("Pick up"));
}

AActor* APortafilter::Interact(AActor* HeldObject)
{
	if (ATamper* Tamper = Cast<ATamper>(HeldObject))
	{
		Tamp();
		return HeldObject;
	}
	if (CoffeeMachine)
	{
		CoffeeMachine = nullptr;
	}
	return this;
}

void APortafilter::Tamp()
{
	const FVector CurrentScale = ContentsSurface->GetRelativeScale3D();
	const FVector ScaleVector(CurrentScale.X, CurrentScale.Y, 0.0001f);
	ContentsSurface->SetRelativeScale3D(ScaleVector);
	IsTamped = true;
}
