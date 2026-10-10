// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/StencilManager.h"

int32 UStencilManager::AllocateStencilIndex()
{
	if (ReleasedIndices.Num() > 0)
		return ReleasedIndices.Pop();
	
	if (NextIndex > 255)
	{
		UE_LOG(LogTemp, Warning, TEXT("Stencil indices exhausted"));
		return -1;
	}
	
	return NextIndex++;
}

void UStencilManager::ReleaseStencilIndex(const int32 Index)
{
	ReleasedIndices.Add(Index);
}
