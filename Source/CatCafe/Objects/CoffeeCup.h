// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Interactable.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fillable.h"
#include "CoffeeCup.generated.h"

UCLASS(Blueprintable, BlueprintType)
class CATCAFE_API ACoffeeCup : public AFillable, public IInteractable
{
	GENERATED_BODY()

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* OuterCup;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Contents")
	bool bContainsEspresso = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Contents")
	bool bContainsMilk = false;

public:
	ACoffeeCup();

	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual void Stop() override;

	void FillWithEspresso(float Amount = 0.5f);

	void AddMilk();

	bool IsEspressoOnly() const { return bContainsEspresso && !bContainsMilk; }

	virtual bool IsReadyToServe() const override { return bContainsEspresso && !Transferring && GetContentsLevel() > 0.0f; }

	virtual bool IsPhysicsObject() override { return true; }
};
