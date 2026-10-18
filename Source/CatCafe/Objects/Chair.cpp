// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Chair.h"

#include "Subsystems/SeatingManager.h"

// Sets default values
AChair::AChair()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	ChairMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ChairMesh"));
	RootComponent = ChairMesh;
	
	TargetPosition = CreateDefaultSubobject<USceneComponent>(TEXT("TargetPosition"));
	TargetPosition->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void AChair::BeginPlay()
{
	Super::BeginPlay();
	
	USeatingManager* SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
	SeatingManager->RegisterSeat(this);
}

// Called every frame
void AChair::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

