// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat/CatAIController.h"

void ACatAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	
	if (CatController)
	{
		CatController->ArriveAtLocation();
	}
}