// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat/CatSpawner.h"

#include "CatController.h"
#include "Engine/World.h"
#include "Subsystems/SeatingManager.h"
#include "Engine/GameInstance.h"

// Sets default values
ACatSpawner::ACatSpawner()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void ACatSpawner::BeginPlay()
{
	Super::BeginPlay();
	
	SeatingManager = GetGameInstance()->GetSubsystem<USeatingManager>();
}

// Called every frame
void ACatSpawner::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	TimeToSpawn -= DeltaTime;
	
	if (TimeToSpawn <= 0 && SeatingManager && !SeatingManager->CafeFull())
	{
		TimeToSpawn = SpawnCooldown;
		SpawnCat();
	}
}

void ACatSpawner::SpawnCat()
{
	ACatController* Cat = GetWorld()->SpawnActor<ACatController>(CatBlueprint, GetActorLocation(), GetActorRotation());
	Cat->SpawnPosition = GetActorLocation();
}

