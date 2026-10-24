// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GroupHead.generated.h"

class APortafilter;
class ACoffeeMachine;

UCLASS()
class CATCAFE_API AGroupHead : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionVolume;
	
	UPROPERTY(EditAnywhere)
	float InteractionRadius = 5.0f;

public:
	AGroupHead();

	UPROPERTY()
	ACoffeeMachine* CoffeeMachine;

	UPROPERTY()
	APortafilter* Portafilter;

	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return true; }
};
