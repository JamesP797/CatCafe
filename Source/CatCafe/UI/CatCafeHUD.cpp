// Fill out your copyright notice in the Description page of Project Settings.


#include "CatCafeHUD.h"

#include "Engine/World.h"
#include "Subsystems/CatCafeGameState.h"

void UCatCafeHUD::NativeConstruct()
{
	Super::NativeConstruct();

	UWorld* World = GetWorld();
	if (!World)
	{
		return;
	}

	if (ACatCafeGameState* GameState = World->GetGameState<ACatCafeGameState>())
	{
		GameState->OnCoinCountChanged.AddDynamic(this, &UCatCafeHUD::OnCoinCountChanged);
		UpdateCoinCount(GameState->GetCoinCount());
	}
}

void UCatCafeHUD::NativeDestruct()
{
	if (UWorld* World = GetWorld())
	{
		if (ACatCafeGameState* GameState = World->GetGameState<ACatCafeGameState>())
		{
			GameState->OnCoinCountChanged.RemoveDynamic(this, &UCatCafeHUD::OnCoinCountChanged);
		}
	}

	Super::NativeDestruct();
}

void UCatCafeHUD::OnCoinCountChanged(int32 NewCoinCount)
{
	UpdateCoinCount(NewCoinCount);
}

void UCatCafeHUD::UpdateCoinCount(int32 NewCoinCount)
{
	if (CoinCount)
	{
		CoinCount->SetText(FText::AsNumber(NewCoinCount));
	}
}
