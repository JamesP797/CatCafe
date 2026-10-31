// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/CatCafeGameState.h"

void ACatCafeGameState::AddCoins(int32 Amount)
{
	if (Amount <= 0)
	{
		return;
	}

	CoinCount += Amount;
	OnCoinCountChanged.Broadcast(CoinCount);
}
