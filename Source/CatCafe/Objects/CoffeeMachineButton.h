// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "CoffeeMachineButton.generated.h"

class USphereComponent;
class ACoffeeMachine;

UCLASS()
class CATCAFE_API ACoffeeMachineButton : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	ACoffeeMachine* CoffeeMachine;

	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionVolume;

public:
	ACoffeeMachineButton();

	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return false; }
};
