// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fillable.h"
#include "Portafilter.generated.h"

class ACoffeeMachine;

UCLASS()
class CATCAFE_API APortafilter : public AFillable, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Portafilter;

	UPROPERTY()
	ACoffeeMachine* CoffeeMachine;
	
	void Tamp();

public:
	APortafilter();

	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return true; }
	
	bool IsTamped = false;
	
	bool IsUsed = false;
};
