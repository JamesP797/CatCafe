// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Utils/InteractEligibility.h"
#include "UObject/Interface.h"
#include "Interactable.generated.h"

UINTERFACE()
class CATCAFE_API UInteractable : public UInterface
{
	GENERATED_BODY()
};

class IInteractable
{
	GENERATED_BODY()

public:
	virtual FInteractEligibility* CanInteract(AActor* HeldObject) = 0;

	virtual AActor* Interact(AActor* HeldObject) = 0;

	virtual bool IsPhysicsObject() = 0;
};
