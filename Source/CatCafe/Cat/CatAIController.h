// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "CatController.h"
#include "CatAIController.generated.h"

/**
 * 
 */
UCLASS()
class CATCAFE_API ACatAIController : public AAIController
{
	GENERATED_BODY()
	
public:
	UPROPERTY()
	ACatController* CatController;
	
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;
};
