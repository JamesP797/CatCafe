// Fill out your copyright notice in the Description page of Project Settings.


#include "CoffeeMachine.h"

#include "Utils/AttachUtils.h"
#include "Components/StaticMeshComponent.h"
#include "CoffeeCup.h"
#include "NiagaraComponent.h"
#include "GroupHead.h"
#include "Portafilter.h"

ACoffeeMachine::ACoffeeMachine()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Machine = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Machine"));
	RootComponent = Machine;

	DripLeft = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DripLeft"));
	DripRight = CreateDefaultSubobject<UNiagaraComponent>(TEXT("DripRight"));
	DripLeft->SetupAttachment(RootComponent);
	DripRight->SetupAttachment(RootComponent);

	PlacePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlacePosition"));
	PlacePosition->SetupAttachment(RootComponent);
	PlacePosition->SetRelativeLocation(FVector(0.0f, 0.0f, 0.75f));
}

void ACoffeeMachine::BeginPlay()
{
	Super::BeginPlay();

	TArray<UChildActorComponent*> ChildActorComponents;
	GetComponents<UChildActorComponent>(ChildActorComponents);

	for (const UChildActorComponent* ChildActorComponent : ChildActorComponents)
	{
		GroupHead = Cast<AGroupHead>(ChildActorComponent->GetChildActor());
		if (GroupHead)
		{
			GroupHead->CoffeeMachine = this;
			break;
		}
	}
}

void ACoffeeMachine::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Pouring && CoffeeCup && !CoffeeCup->Transferring)
	{
		DripLeft->Deactivate();
		DripRight->Deactivate();
		Pouring = false;
	}
}

FInteractEligibility* ACoffeeMachine::CanInteract(AActor* HeldObject)
{
	// If the held object is a cup, and we don't currently have a cup, return true
	// If the player isn't holding a cup and the machine has one, return true
	if (HeldObject && !CoffeeCup)
	{
		return new FInteractEligibility(Cast<ACoffeeCup>(HeldObject) ? true : false, FText::FromString("Place"));
	}
	return new FInteractEligibility(!HeldObject && CoffeeCup && !Pouring, FText::FromString("Retrieve"));

	// In future, will also check group handle and jug
}

AActor* ACoffeeMachine::Interact(AActor* HeldObject)
{
	if (!HeldObject && CoffeeCup && !Pouring)
	{
		// Let the player pick up the coffee cup that we have stored
		AActor* CoffeeCupToRelease = CoffeeCup;
		CoffeeCup = nullptr;
		return CoffeeCupToRelease;
	}

	// Assumes the caller has already checked CanInteract
	if (ACoffeeCup* CoffeeCupToPlace = Cast<ACoffeeCup>(HeldObject))
	{
		HeldObject->AttachToComponent(PlacePosition, AttachUtils::GetPlaceAttachRules());
		CoffeeCup = CoffeeCupToPlace;
	}

	return nullptr;
}

void ACoffeeMachine::PourCoffee()
{
	if (!CoffeeCup) return;

	CoffeeCup->FillUp(0.5f);
	DripLeft->Activate(true);
	DripRight->Activate(true);
	Pouring = true;
	GroupHead->Portafilter->IsUsed = true;
}
