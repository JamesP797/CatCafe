// Fill out your copyright notice in the Description page of Project Settings.

#include "CoffeeCup.h"

#include "MilkJug.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ACoffeeCup::ACoffeeCup()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	OuterCup = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("OuterCup"));
	RootComponent = OuterCup;

	ContentsContainer->SetupAttachment(RootComponent);
	ContentsSurface->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void ACoffeeCup::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ACoffeeCup::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* ACoffeeCup::CanInteract(AActor* HeldObject)
{
	if (AMilkJug* MilkJug = Cast<AMilkJug>(HeldObject))
	{
		return new FInteractEligibility(GetContentsLevel() > 0.29f && !IsFull() && MilkJug->IsFull(), FText::FromString("Pour Milk"));
	}
	return new FInteractEligibility(!HeldObject && !Transferring, FText::FromString("Pick up"));
}

AActor* ACoffeeCup::Interact(AActor* HeldObject)
{
	if (AMilkJug* MilkJug = Cast<AMilkJug>(HeldObject))
	{
		// TODO change material colour
		FillUp();
		MilkJug->Drain();
		return HeldObject;
	}
	return this;
}
