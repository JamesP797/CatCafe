// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Glass.h"

#include "CoffeeCup.h"
#include "MilkCarton.h"
#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

AGlass::AGlass()
{
	Glass = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Glass"));
	RootComponent = Glass;

	Glass->SetCollisionProfileName(TEXT("PhysicsActor"));
	Glass->SetSimulatePhysics(true);

	ContentsBody->SetupAttachment(Glass);
	ContentsSurface->SetupAttachment(Glass);
	ContentsMask->SetupAttachment(Glass);
	ContentsContainer->SetupAttachment(Glass);
}

void AGlass::BeginPlay()
{
	Super::BeginPlay();

	ContentsBodyMaterial = Cast<UMaterialInstanceDynamic>(ContentsBody->GetMaterial(0));
	if (!ContentsBodyMaterial)
	{
		ContentsBodyMaterial = ContentsBody->CreateAndSetMaterialInstanceDynamic(0);
	}

	ContentsSurfaceMaterial = Cast<UMaterialInstanceDynamic>(ContentsSurface->GetMaterial(0));
	if (!ContentsSurfaceMaterial)
	{
		ContentsSurfaceMaterial = ContentsSurface->CreateAndSetMaterialInstanceDynamic(0);
	}

	ApplyContentsColour();
}

void AGlass::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	ApplyContentsColour();
}

void AGlass::ApplyContentsColour()
{
	FLinearColor Colour = bColourLocked ? LockedColour : MilkColour;

	if (!bColourLocked && bContainsCoffee)
	{
		// Blend from milk white to coffee brown as the coffee level rises.
		const float CoffeeProgress = FMath::Clamp(
			(GetContentsLevel() - MilkFillAmount) / (1.0f - MilkFillAmount),
			0.0f,
			1.0f);
		Colour = FMath::Lerp(MilkColour, CoffeeColour, CoffeeProgress);
	}

	if (ContentsBodyMaterial)
	{
		ContentsBodyMaterial->SetVectorParameterValue(TEXT("BaseColour"), Colour);
	}
	if (ContentsSurfaceMaterial)
	{
		ContentsSurfaceMaterial->SetVectorParameterValue(TEXT("BaseColour"), Colour);
	}
}

FInteractEligibility* AGlass::CanInteract(AActor* HeldObject)
{
	if (Cast<AMilkCarton>(HeldObject))
	{
		return new FInteractEligibility(
			!bContainsMilk && !bContainsCoffee && !Transferring,
			FText::FromString("Pour milk"));
	}

	if (ACoffeeCup* CoffeeCup = Cast<ACoffeeCup>(HeldObject))
	{
		return new FInteractEligibility(
			bContainsMilk && !bContainsCoffee && CoffeeCup->IsEspressoOnly() && !IsFull() && !Transferring,
			FText::FromString("Pour coffee"));
	}

	return new FInteractEligibility(!HeldObject && !Transferring, FText::FromString("Pick up"));
}

AActor* AGlass::Interact(AActor* HeldObject)
{
	if (Cast<AMilkCarton>(HeldObject))
	{
		bContainsMilk = true;
		bContainsCoffee = false;
		bColourLocked = false;
		FillUp(MilkFillAmount);
		return HeldObject;
	}

	if (ACoffeeCup* CoffeeCup = Cast<ACoffeeCup>(HeldObject))
	{
		bContainsCoffee = true;
		bColourLocked = false;
		FillUp(1.0f);
		CoffeeCup->Drain();
		return HeldObject;
	}

	return this;
}

void AGlass::Stop()
{
	Super::Stop();

	if (IsEmpty())
	{
		// Allow the empty glass to be reused, but keep its made colour on screen.
		bContainsMilk = false;
		bContainsCoffee = false;
	}
	else if (bContainsCoffee)
	{
		// Once the coffee has finished pouring, keep that colour even after
		// the cat drinks the glass empty.
		bColourLocked = true;
		LockedColour = CoffeeColour;
	}
}
