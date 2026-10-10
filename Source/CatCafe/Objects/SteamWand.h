// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Interactable.h"
#include "SteamWand.generated.h"

class AMilkJug;

UCLASS()
class CATCAFE_API ASteamWand : public AActor, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* SteamWand;
	
	UPROPERTY(EditAnywhere, Category = "Position")
	USceneComponent* PlacePosition;
	
	UPROPERTY()
	AMilkJug* MilkJug;

public:	
	ASteamWand();

	virtual void Tick(float DeltaTime) override;
	
	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return false; }
};
