// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/SeatingManager.h"

#include "Cat/CatController.h"
#include "Kismet/GameplayStatics.h"
#include "Objects/Chair.h"

void USeatingManager::RegisterSeats()
{
	TArray<AActor*> Seats;
	UGameplayStatics::GetAllActorsWithTag(Cast<UObject>(GetWorld()), TEXT("Chair"), Seats);
	for (AActor* Seat : Seats)
	{
		if (AChair* Chair = Cast<AChair>(Seat))
		{
			SeatAllocations.Add(Chair, nullptr);
		}
	}
}

void USeatingManager::RegisterSeat(AChair* Seat)
{
	SeatAllocations.Add(Seat, nullptr);
}

AChair* USeatingManager::RequestSeat(ACatController* Cat)
{
	// Get a random free seat from the seat allocations map
	TArray<AChair*> Seats;
	SeatAllocations.GetKeys(Seats);
	for (AChair* Seat : Seats)
	{
		if (ACatController** SittingCat = SeatAllocations.Find(Seat))
		{
			if (*SittingCat == nullptr)
			{
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

bool USeatingManager::CafeFull()
{
	for (const TPair<AChair*, ACatController*>& SeatAllocation : SeatAllocations)
	{
		if (SeatAllocation.Value == nullptr) return false;
	}
	
	return true;
}
