// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Interfaces/Interactable.h"
#include "CatController.generated.h"

class UCatAnimInstance;
class USeatingManager;
class AChair;
class ACatAIController;
class ACoffeeCup;
class UWidgetComponent;

UCLASS()
class CATCAFE_API ACatController : public ACharacter, public IInteractable
{
	GENERATED_BODY()
	
private:
	void ReceiveCoffee(ACoffeeCup* CoffeeCupToGrab);
	
	UFUNCTION()
	void OnCoffeeFinished();
	
	UPROPERTY()
	USeatingManager* SeatingManager;
	
	UPROPERTY()
	APlayerController* PlayerController;
	
	UPROPERTY()
	UCatAnimInstance* AnimInstance;
	
	bool bTurning = false;
	bool bOnChair = false;
	FVector TargetLocation;
	FRotator TargetRotation;
	
protected:
	UPROPERTY()
	AChair* TargetChair;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	UWidgetComponent* SpeechBubble;
	
	UPROPERTY()
	ACoffeeCup* CoffeeCup;
	
	UPROPERTY(EditAnywhere, Category = "Components")
	USceneComponent* HoldPosition;
	
	bool bWantsCoffee = false;
	bool bHadCoffee = false;
	
	UPROPERTY()
	ACatAIController* AIController;

public:
	virtual void BeginPlay() override;
	
	virtual void Tick(float DeltaTime) override;
	
	ACatController();
	
	virtual FInteractEligibility* CanInteract(AActor* HeldObject) override;
	
	virtual AActor* Interact(AActor* HeldObject) override;
	
	virtual bool IsPhysicsObject() override { return false; }
	
	void ArriveAtLocation();
	
	void OnJumpComplete();
	
	void OnSitComplete();
	
	FVector SpawnPosition;
};
