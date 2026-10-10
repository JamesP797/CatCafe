// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FrontDoor.generated.h"

class USphereComponent;

UCLASS()
class CATCAFE_API AFrontDoor : public AActor
{
	GENERATED_BODY()
	
private:	
	bool bOpening = false;
	
	bool bClosing = false;
	
	void OpenDoor(UStaticMeshComponent* Door, float TargetRotation, float DeltaTime);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	USceneComponent* SceneRoot;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* LeftDoor;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components")
	UStaticMeshComponent* RightDoor;
	
	UPROPERTY(VisibleAnywhere)
	USphereComponent* InteractionVolume;
	
	UFUNCTION()
	void OnTriggerOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, 
		const FHitResult& SweepResult);

	UFUNCTION()
	void OnTriggerEndOverlap(UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:	
	// Sets default values for this actor's properties
	AFrontDoor();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	void Open();
	
	void Close();

};
