// Fill out your copyright notice in the Description page of Project Settings.


#include "Cat/CatAnimInstance.h"

#include "CatController.h"

void UCatAnimInstance::AnimNotify_JumpComplete()
{
	ACatController* Cat = Cast<ACatController>(TryGetPawnOwner());
	if (Cat)
	{
		Cat->OnJumpComplete();
	}
}

void UCatAnimInstance::AnimNotify_SitComplete()
{
	ACatController* Cat = Cast<ACatController>(TryGetPawnOwner());
	if (Cat)
	{
		Cat->OnSitComplete();
	}
}
