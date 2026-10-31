// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Coin.generated.h"

UCLASS()
class CATCAFE_API ACoin : public AActor
{
	GENERATED_BODY()
	
private:
	float CoinMaterialOpacity = 1.0f;
	
	UPROPERTY()
	UMaterialInstanceDynamic* CoinMaterial;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* CoinMesh;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	ACoin();

};
