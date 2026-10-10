// Fill out your copyright notice in the Description page of Project Settings.


#include "Objects/Grinder.h"

#include "NiagaraComponent.h"
#include "Portafilter.h"
#include "Components/StaticMeshComponent.h"
#include "Utils/AttachUtils.h"

// Sets default values
AGrinder::AGrinder()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	Grinder = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Grinder"));
	RootComponent = Grinder;
	
	GrindSelector = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("GrindSelector"));
	GrindSelector->SetupAttachment(Grinder);
	
	GrindParticles = CreateDefaultSubobject<UNiagaraComponent>(TEXT("GrindParticles"));
	GrindParticles->SetupAttachment(Grinder);
	
	ContentsBody->SetupAttachment(Grinder);
	ContentsSurface->SetupAttachment(Grinder);
	ContentsMask->SetupAttachment(Grinder);
	
	PlacePosition = CreateDefaultSubobject<USceneComponent>(TEXT("PlacePosition"));
	PlacePosition->SetupAttachment(Grinder);
	PlacePosition->SetRelativeLocation(FVector(0.5f, 0.0f, -1.0f));
}

// Called when the game starts or when spawned
void AGrinder::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AGrinder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (Transferring)
	{
		if (Portafilter && !Portafilter->Transferring)
		{
			Stop();
		}
	}

}

FInteractEligibility* AGrinder::CanInteract(AActor* HeldObject)
{
	if (Portafilter && !Portafilter->IsFull())
	{
		return new FInteractEligibility(!Transferring, FText::FromString("Start grinding"));
	}
	if (Portafilter && Portafilter->IsFull() && !HeldObject)
	{
		return new FInteractEligibility(true, FText::FromString("Retrieve portafilter"));
	}
	if (APortafilter* PortafilterToPlace = Cast<APortafilter>(HeldObject))
	{
		return new FInteractEligibility(!Portafilter, FText::FromString("Place portafilter"));
	} 
	return new FInteractEligibility(false, FText::FromString(""));
}

AActor* AGrinder::Interact(AActor* HeldObject)
{
	if (APortafilter* PortafilterToPlace = Cast<APortafilter>(HeldObject))
	{
		PortafilterToPlace->AttachToComponent(PlacePosition, AttachUtils::GetPlaceAttachRules());
		Portafilter = PortafilterToPlace;
		return nullptr;
	}
	
	if (Portafilter && !Portafilter->IsFull())
	{
		Drain();
		Portafilter->FillUp();
		GrindParticles->Activate(true);
		return HeldObject;
	}
	
	if (Portafilter)
	{
		AActor* PortafilterToRetrieve = Portafilter;
		Portafilter = nullptr;
		return PortafilterToRetrieve;
	}
	
	return nullptr;
}

void AGrinder::Stop()
{
	Super::Stop();
	GrindParticles->Deactivate();
}
