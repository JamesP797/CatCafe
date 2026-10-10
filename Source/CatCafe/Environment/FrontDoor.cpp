// Fill out your copyright notice in the Description page of Project Settings.


#include "Environment/FrontDoor.h"

#include "Cat/CatController.h"
#include "Components/SphereComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFrontDoor::AFrontDoor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	RootComponent = SceneRoot;

	LeftDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("LeftDoor"));
	LeftDoor->SetupAttachment(RootComponent);
	
	RightDoor = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("RightDoor"));
	RightDoor->SetupAttachment(RootComponent);
	
	InteractionVolume = CreateDefaultSubobject<USphereComponent>(TEXT("InteractionVolume"));
	InteractionVolume->SetupAttachment(RootComponent);
	InteractionVolume->SetSphereRadius(100.0f);
	InteractionVolume->SetCollisionProfileName(TEXT("Trigger"));
}

// Called when the game starts or when spawned
void AFrontDoor::BeginPlay()
{
	Super::BeginPlay();
	
	InteractionVolume->OnComponentBeginOverlap.AddDynamic(this, &AFrontDoor::OnTriggerOverlap);
	InteractionVolume->OnComponentEndOverlap.AddDynamic(this, &AFrontDoor::OnTriggerEndOverlap);
}

// Called every frame
void AFrontDoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bOpening)
	{
		OpenDoor(LeftDoor, -90.0f, DeltaTime);
		OpenDoor(RightDoor, 90.0f, DeltaTime);
	}
	
	if (bClosing)
	{
		OpenDoor(LeftDoor, 0.0f, DeltaTime);
		OpenDoor(RightDoor, 0.0f, DeltaTime);
	}
}

void AFrontDoor::Open()
{
	bClosing = false;
	bOpening = true;
}

void AFrontDoor::Close()
{
	bOpening = false;
	bClosing = true;
}

void AFrontDoor::OpenDoor(UStaticMeshComponent* Door, float TargetRotation, float DeltaTime)
{
	FRotator CurrentRotation = Door->GetRelativeRotation();
	FRotator NewRotation = FRotator(
		CurrentRotation.GetComponentForAxis(EAxis::Y), 
		FMath::FInterpTo(CurrentRotation.GetComponentForAxis(EAxis::Z), TargetRotation, DeltaTime, 0.5f), 
		CurrentRotation.GetComponentForAxis(EAxis::X));
	Door->SetRelativeRotation(NewRotation);
	
	if (FMath::IsNearlyEqual(CurrentRotation.GetComponentForAxis(EAxis::Z), TargetRotation))
		bOpening = false;
}

void AFrontDoor::OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (Cast<ACatController>(OtherActor))
	{
		Open();
	}
}

void AFrontDoor::OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex)
{
	if (Cast<ACatController>(OtherActor))
	{
		Close();
	}
}