#pragma once

#include "CoreMinimal.h"
#include "InteractEligibility.generated.h"

USTRUCT()
struct FInteractEligibility
{
	GENERATED_BODY()
	
	bool IsEligible;
	
	FText PromptText;
};
