// Copyright Epic Games, Inc. All Rights Reserved.

#include "CatCafeGameMode.h"

#include "Subsystems/CatCafeGameState.h"

ACatCafeGameMode::ACatCafeGameMode()
{
	GameStateClass = ACatCafeGameState::StaticClass();
}
