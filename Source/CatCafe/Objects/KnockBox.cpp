// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/KnockBox.h"
#include "Components/StaticMeshComponent.h"

#include "Portafilter.h"

// Sets default values
AKnockBox::AKnockBox()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	KnockBox = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KnockBox"));
	RootComponent = KnockBox;
}

// Called when the game starts or when spawned
void AKnockBox::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AKnockBox::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractEligibility* AKnockBox::CanInteract(AActor* HeldObject)
{
	if (APortafilter* Portafilter = Cast<APortafilter>(HeldObject))
	{
		return new FInteractEligibility(Portafilter->IsFull(), FText::FromString("Bin"));
	}
	return new FInteractEligibility(false, FText::FromString(""));
}

AActor* AKnockBox::Interact(AActor* HeldObject)
{
	if (APortafilter* Portafilter = Cast<APortafilter>(HeldObject))
	{
		Portafilter->Empty();
		Portafilter->IsTamped = false;
		Portafilter->IsUsed = false;
	}
	return HeldObject;
}
