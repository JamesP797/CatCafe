// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "SeatingManager.generated.h"

class AChair;
class ACatController;
/**
 * 
 */
UCLASS()
class CATCAFE_API USeatingManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
public:
	AChair* RequestSeat(ACatController* Cat);
	
	void ReleaseSeat(AChair* Seat);
	
private:
	void RegisterSeats();
	
	UPROPERTY()
	TMap<AChair*, ACatController*> SeatAllocations;
};
