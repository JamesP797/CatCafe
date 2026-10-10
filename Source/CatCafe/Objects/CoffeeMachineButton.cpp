// Fill out your copyright notice in the Description page of Project Settings.

#include "CoffeeMachineButton.h"

#include "Components/SphereComponent.h"
#include "CoffeeMachine.h"
#include "CoffeeCup.h"
#include "GroupHead.h"
#include "Portafilter.h"

// Sets default values
ACoffeeMachineButton::ACoffeeMachineButton()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	RootComponent = InteractionVolume;

	InteractionVolume->SetSphereRadius(5.0f);
	InteractionVolume->SetCollisionProfileName(TEXT("Custom"));
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void ACoffeeMachineButton::BeginPlay()
{
	Super::BeginPlay();

	if (const UActorComponent* ParentComp = GetParentComponent())
		CoffeeMachine = Cast<ACoffeeMachine>(
			ParentComp->GetOwner());
}

void ACoffeeMachineButton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* ACoffeeMachineButton::CanInteract(AActor* HeldObject)
{
	const bool bCanInteract = CoffeeMachine
		&& CoffeeMachine->CoffeeCup
		&& !CoffeeMachine->CoffeeCup->Transferring
		&& !CoffeeMachine->CoffeeCup->IsFull()
		&& CoffeeMachine->GroupHead
		&& CoffeeMachine->GroupHead->Portafilter
		&& CoffeeMachine->GroupHead->Portafilter->IsFull()
		&& !CoffeeMachine->GroupHead->Portafilter->IsUsed
		&& !HeldObject;
	return new FInteractEligibility(bCanInteract, FText::FromString("Pour coffee"));
}

AActor* ACoffeeMachineButton::Interact(AActor* HeldObject)
{
	if (CoffeeMachine) CoffeeMachine->PourCoffee();
	return HeldObject;
}
