// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CatSpawner.generated.h"

UCLASS()
class CATCAFE_API ACatSpawner : public AActor
{
	GENERATED_BODY()
	
private:
	float TimeToSpawn = 0.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	UPROPERTY(EditAnywhere, Category = "Configuration")
	float SpawnCooldown = 5000.0f;
	
	UPROPERTY(EditAnywhere, Category = "Configuration")
	TSubclassOf<AActor> CatBlueprint;
	
	void SpawnCat();

public:	
	ACatSpawner();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

};
