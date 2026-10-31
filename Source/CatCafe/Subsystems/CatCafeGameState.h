// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "CatCafeGameState.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCoinCountChanged, int32, NewCoinCount);

/**
 * Holds the cafe's shared game state, including the player's coin total.
 */
UCLASS()
class CATCAFE_API ACatCafeGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	/**
	 * Adds coins to the player's total and notifies listeners.
	 * @param Amount Number of coins to add. Non-positive values are ignored.
	 */
	UFUNCTION(BlueprintCallable, Category = "Economy")
	void AddCoins(int32 Amount);

	UFUNCTION(BlueprintPure, Category = "Economy")
	int32 GetCoinCount() const { return CoinCount; }

	/** Broadcast whenever the coin total changes. */
	UPROPERTY(BlueprintAssignable, Category = "Economy")
	FOnCoinCountChanged OnCoinCountChanged;

private:
	UPROPERTY(BlueprintReadOnly, Category = "Economy", meta = (AllowPrivateAccess = "true"))
	int32 CoinCount = 0;
};
