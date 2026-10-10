// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MilkCarton.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMilkCarton::AMilkCarton()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MilkCarton = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MilkCarton"));
	RootComponent = MilkCarton;
}

// Called when the game starts or when spawned
void AMilkCarton::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMilkCarton::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractEligibility* AMilkCarton::CanInteract(AActor* HeldObject)
{
	return new FInteractEligibility(!HeldObject, FText::FromString("Pick up"));
}

AActor* AMilkCarton::Interact(AActor* HeldObject)
{
	return this;
}