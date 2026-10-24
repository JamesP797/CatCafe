// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CatAnimInstance.generated.h"

/**
 * 
 */
UCLASS()
class CATCAFE_API UCatAnimInstance : public UAnimInstance
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsJumping = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsSitting = false;
	
	UPROPERTY(BlueprintReadWrite, Category = "Animation")
	bool bIsDrinking = false;
	
	UFUNCTION()
	void AnimNotify_JumpComplete();
	
	UFUNCTION()
	void AnimNotify_SitComplete();
};
