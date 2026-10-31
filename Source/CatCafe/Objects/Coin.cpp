// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Coin.h"

#include "Components/StaticMeshComponent.h"
#include "Materials/MaterialInstanceDynamic.h"

// Sets default values
ACoin::ACoin()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	CoinMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("CoinMesh"));
	RootComponent = CoinMesh;

}

// Called when the game starts or when spawned
void ACoin::BeginPlay()
{
	Super::BeginPlay();
	
	CoinMaterial = CoinMesh->CreateAndSetMaterialInstanceDynamic(0);
	SetActorRelativeRotation(FRotator(0.0f, 0.0f, 90.0f));
}

// Called every frame
void ACoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	SetActorLocation(GetActorLocation() + FVector(0.0f, 0.0f, 3.0f));
	AddActorLocalRotation(FRotator(10.0f, 0.0f, 0.0f));
	CoinMaterialOpacity = FMath::FInterpTo(CoinMaterialOpacity, 0.0f, DeltaTime, 3.0f);
	CoinMaterial->SetScalarParameterValue(FName("Opacity"), CoinMaterialOpacity);
	
	if (CoinMaterialOpacity <= 0.0f) Destroy();
}

