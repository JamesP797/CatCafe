// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Components/TextBlock.h"
#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Components/Image.h"
#include "CatCafeHUD.generated.h"

/**
 * 
 */
UCLASS()
class CATCAFE_API UCatCafeHUD : public UUserWidget
{
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UTextBlock* InteractPromptText;
	
	UPROPERTY(BlueprintReadWrite, meta = (BindWidget))
	UImage* InteractIcon;
};
