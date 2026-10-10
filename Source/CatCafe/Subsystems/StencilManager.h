// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Subsystems/GameInstanceSubsystem.h"
#include "StencilManager.generated.h"

/**
 * 
 */
UCLASS()
class CATCAFE_API UStencilManager : public UGameInstanceSubsystem
{
	GENERATED_BODY()
	
private:
	int32 NextIndex = 1;
	
	TArray<int32> ReleasedIndices;
	
public:
	int32 AllocateStencilIndex();
	
	void ReleaseStencilIndex(const int32 Index);
	
};
