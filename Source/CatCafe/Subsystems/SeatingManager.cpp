// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SeatingManager.h"

#include "Cat/CatController.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/Chair.h"

void USeatingManager::RegisterSeats()
{
	UE_LOG(LogTemp, Warning, TEXT("Initializing seats"));
	TArray<AActor*> Seats;
	UGameplayStatics::GetAllActorsWithTag(GetWorld(), TEXT("Chair"), Seats);
	for (AActor* Seat : Seats)
	{
		if (AChair* Chair = Cast<AChair>(Seat))
		{
			UE_LOG(LogTemp, Warning, TEXT("Registering a seat"));
			SeatAllocations.Add(Chair, nullptr);
		}
	}
}

AChair* USeatingManager::RequestSeat(ACatController* Cat)
{
	UE_LOG(LogTemp, Warning, TEXT("Cat requested a seat"));
	// Lazy initialize
	if (SeatAllocations.IsEmpty())
	{
		RegisterSeats();
	}
	
	// Get a random free seat from the seat allocations map
	TArray<AChair*> Seats;
	SeatAllocations.GetKeys(Seats);
	for (AChair* Seat : Seats)
	{
		UE_LOG(LogTemp, Warning, TEXT("Checking a seat"));
		if (ACatController** SittingCat = SeatAllocations.Find(Seat))
		{
			UE_LOG(LogTemp, Warning, TEXT("The seat is in the map"));
			if (*SittingCat == nullptr)
			{
				UE_LOG(LogTemp, Warning, TEXT("The seat is free"));
				SeatAllocations.Add(Seat, Cat);
				return Seat;
			}
		}
	}
	
	// No seats free
	return nullptr;
}

void USeatingManager::ReleaseSeat(AChair* Seat)
{
	SeatAllocations.Add(Seat, nullptr);
}
