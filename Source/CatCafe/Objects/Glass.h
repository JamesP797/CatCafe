// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Interfaces/Fillable.h"
#include "Interfaces/Interactable.h"
#include "Glass.generated.h"

class UMaterialInstanceDynamic;

/**
 * 
 */
UCLASS()
class CATCAFE_API AGlass : public AFillable, public IInteractable
{
	GENERATED_BODY()
	
protected:
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Glass;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Contents")
	bool bContainsMilk = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Contents")
	bool bContainsCoffee = false;

	UPROPERTY(EditAnywhere, Category = "Contents")
	float MilkFillAmount = 0.8f;

	UPROPERTY(EditAnywhere, Category = "Contents")
	FLinearColor MilkColour = FLinearColor(0.9f, 0.87f, 0.78f);

	UPROPERTY(EditAnywhere, Category = "Contents")
	FLinearColor CoffeeColour = FLinearColor(0.32f, 0.16f, 0.07f);

	bool bColourLocked = false;

	FLinearColor LockedColour = FLinearColor::White;

	UPROPERTY()
	UMaterialInstanceDynamic* ContentsBodyMaterial;

	UPROPERTY()
	UMaterialInstanceDynamic* ContentsSurfaceMaterial;

private:
	void ApplyContentsColour();

public:
	AGlass();

	virtual void BeginPlay() override;

	virtual void Tick(float DeltaTime) override;
	
	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual void Stop() override;

	virtual bool IsReadyToServe() const override { return bContainsCoffee && !Transferring && IsFull(); }

	virtual bool IsPhysicsObject() override { return true; }
	
};
