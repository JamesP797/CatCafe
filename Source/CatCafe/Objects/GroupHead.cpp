// Fill out your copyright notice in the Description page of Project Settings.


#include "GroupHead.h"

#include "Utils/AttachUtils.h"
#include "Components/StaticMeshComponent.h"
#include "Portafilter.h"
#include "CoffeeMachine.h"
#include "CoffeeCup.h"

AGroupHead::AGroupHead()
{
	PrimaryActorTick.bCanEverTick = true;

	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	RootComponent = InteractionVolume;

	InteractionVolume->SetSphereRadius(InteractionRadius);
	InteractionVolume->SetCollisionProfileName(TEXT("Custom"));
	InteractionVolume->SetCollisionResponseToAllChannels(ECR_Ignore);
	InteractionVolume->SetCollisionResponseToChannel(ECC_Visibility, ECR_Block);
	InteractionVolume->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}

void AGroupHead::BeginPlay()
{
	Super::BeginPlay();

	InteractionVolume->SetSphereRadius(InteractionRadius);
	
	if (const UActorComponent* ParentComp = GetParentComponent())
		CoffeeMachine = Cast<ACoffeeMachine>(
			ParentComp->GetOwner());
}

void AGroupHead::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* AGroupHead::CanInteract(AActor* HeldObject)
{
	if (!HeldObject && Portafilter && CoffeeMachine && (!CoffeeMachine->CoffeeCup || !CoffeeMachine->CoffeeCup->
		Transferring))
		return new FInteractEligibility(true, FText::FromString("Retrieve"));

	if (APortafilter* PortafilterToPlace = Cast<APortafilter>(HeldObject))
	{
		if (!Portafilter && PortafilterToPlace->IsTamped)
			return new FInteractEligibility(true, FText::FromString("Place"));
	}

	return new FInteractEligibility(false, FText::FromString(""));
}

AActor* AGroupHead::Interact(AActor* HeldObject)
{
	if (!HeldObject)
	{
		APortafilter* PortafilterToPickup = Portafilter;
		Portafilter = nullptr;
		return PortafilterToPickup;
	}

	if (APortafilter* PortafilterToPickup = Cast<APortafilter>(HeldObject))
	{
		Portafilter = PortafilterToPickup;
		HeldObject->AttachToComponent(InteractionVolume, AttachUtils::GetPlaceAttachRules());
	}

	return nullptr;
}
