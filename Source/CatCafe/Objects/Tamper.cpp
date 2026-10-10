// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Tamper.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
ATamper::ATamper()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Tamper = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Tamper"));
	RootComponent = Tamper;

}

// Called when the game starts or when spawned
void ATamper::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ATamper::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

FInteractEligibility* ATamper::CanInteract(AActor* HeldObject)
{
	return new FInteractEligibility(!HeldObject, FText::FromString("Pick up"));
}

AActor* ATamper::Interact(AActor* HeldObject)
{
	return this;
}
