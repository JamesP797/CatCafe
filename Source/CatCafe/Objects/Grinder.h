// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/Fillable.h"
#include "Interfaces/Interactable.h"
#include "Grinder.generated.h"

class UNiagaraComponent;
class APortafilter;

UCLASS()
class CATCAFE_API AGrinder : public AFillable, public IInteractable
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* Grinder;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* GrindSelector;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UNiagaraComponent* GrindParticles;
	
	UPROPERTY(EditAnywhere, Category = "Position")
	USceneComponent* PlacePosition;
	
	UPROPERTY()
	APortafilter* Portafilter;

public:
	AGrinder();
	
	virtual void Tick(float DeltaTime) override;

	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;

	virtual AActor* Interact(AActor* HeldObject) override;

	virtual bool IsPhysicsObject() override { return false; }
	
	virtual void Stop() override;

};
