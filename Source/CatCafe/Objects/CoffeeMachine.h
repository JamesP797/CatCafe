// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "CoffeeMachine.generated.h"

class AGroupHead;
class APortafilter;
class ACoffeeCup;
class UNiagaraComponent;

UCLASS(Blueprintable, BlueprintType)
class CATCAFE_API ACoffeeMachine : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/**
	 * ===================================================
	 *                     PHYSICAL COMPONENTS
	 * ===================================================
	*/

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Machine;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* DripLeft;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* DripRight;

	/**
	 * ===================================================
	 *                     PLAYER STATE
	 * ===================================================
	*/

	UPROPERTY(EditAnywhere, Category = "Position")
	USceneComponent* PlacePosition;

	bool Pouring = false;

public:
	ACoffeeMachine();

	virtual void Tick(float DeltaTime) override;

	void PourCoffee();

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return false; }

	/**
	 * ===================================================
	 *                     PLAYER STATE
	 * ===================================================
	*/

	UPROPERTY()
	ACoffeeCup* CoffeeCup;

	UPROPERTY()
	AGroupHead* GroupHead;
};
