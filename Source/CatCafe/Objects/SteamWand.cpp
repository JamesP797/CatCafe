// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/SteamWand.h"
#include "Components/StaticMeshComponent.h"
#include "Objects/MilkJug.h"
#include "Utils/AttachUtils.h"

// Sets default values
ASteamWand::ASteamWand()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SteamWand = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("SteamWand"));
	RootComponent = SteamWand;
	
	PlacePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlacePosition"));
	PlacePosition->SetupAttachment(RootComponent);
	PlacePosition->SetRelativeLocation(FVector(0.0f, 0.5f, -1.00f));
}

// Called when the game starts or when spawned
void ASteamWand::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteamWand::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractEligibility* ASteamWand::CanInteract(AActor* HeldObject)
{
	bool CanSteam = HeldObject && Cast<AMilkJug>(HeldObject) && !MilkJug;
	bool CanRetrieve = !HeldObject && MilkJug && MilkJug->IsFull();
	
	if (CanSteam)
		return new FInteractEligibility(true, FText::FromString("Steam Milk"));
	
	if (CanRetrieve)
		return new FInteractEligibility(true, FText::FromString("Retrieve"));
	
	return new FInteractEligibility(false, FText::FromString(""));
}

AActor* ASteamWand::Interact(AActor* HeldObject)
{
	if (AMilkJug* MilkJugToReceive = Cast<AMilkJug>(HeldObject))
	{
		MilkJug = MilkJugToReceive;
		MilkJug->AttachToComponent(PlacePosition, AttachUtils::GetPlaceAttachRules());
		MilkJug->FillUp();
		return nullptr;
	}
	
	AMilkJug* MilkJugToPickUp = MilkJug;
	MilkJug = nullptr;
	return MilkJugToPickUp;
}

