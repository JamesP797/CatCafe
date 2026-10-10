// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Fillable.h"
#include "Interfaces/Interactable.h"
#include "MilkJug.generated.h"

UCLASS()
class CATCAFE_API AMilkJug : public AFillable, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* MilkJug;

public:	
	AMilkJug();
	
	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return true; }

};
