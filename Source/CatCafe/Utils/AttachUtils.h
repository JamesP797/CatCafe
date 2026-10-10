#pragma once

#include "CoreMinimal.h"
#include "Engine/EngineTypes.h"

class CATCAFE_API AttachUtils
{
public:
	static FAttachmentTransformRules GetPlaceAttachRules()
	{
		return FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,  // position
			EAttachmentRule::SnapToTarget,  // rotation
			EAttachmentRule::KeepWorld,     // scale
			false
		);
	}
	
	static FAttachmentTransformRules GetPickupAttachRules()
	{
		return FAttachmentTransformRules(
			EAttachmentRule::SnapToTarget,  // position
			EAttachmentRule::SnapToTarget,  // rotation
			EAttachmentRule::KeepWorld,     // scale
			false
		);
	}
	
	static FDetachmentTransformRules GetDropDetachmentRules()
	{
		return FDetachmentTransformRules(EDetachmentRule::KeepWorld, true);
	}
	
	static void SetCollisionOfHeld(const AActor* Object, const bool Enabled);
	
};
