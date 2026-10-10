// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/MilkJug.h"

#include "MilkCarton.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AMilkJug::AMilkJug()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MilkJug = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MilkJug"));
	RootComponent = MilkJug;
	
	ContentsSurface->SetupAttachment(RootComponent);
	ContentsContainer->SetupAttachment(RootComponent);
}

// Called when the game starts or when spawned
void AMilkJug::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AMilkJug::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

FInteractEligibility* AMilkJug::CanInteract(AActor* HeldObject)
{
	if (AMilkCarton* MilkCarton = Cast<AMilkCarton>(HeldObject))
	{
		return new FInteractEligibility(!Transferring && !IsFull() && GetContentsLevel() < 0.3f, FText::FromString("Pour milk"));
	}
	return new FInteractEligibility(!HeldObject && !Transferring, FText::FromString("Pick up"));
}

AActor* AMilkJug::Interact(AActor* HeldObject)
{
	if (AMilkCarton* MilkCarton = Cast<AMilkCarton>(HeldObject))
	{
		// TODO not deformed
		FillUp(0.3f);
		return HeldObject;
	}
	return this;
}

