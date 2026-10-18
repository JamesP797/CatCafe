// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Fillable.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnFillableEmpty);

UCLASS(Abstract)
class CATCAFE_API AFillable : public AActor
{
	GENERATED_BODY()

private:
	int32 StencilIndex;
	
	float TargetFillAmount;

protected:
	virtual void BeginPlay() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FillableComponents")
	UStaticMeshComponent* ContentsBody;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FillableComponents")
	UStaticMeshComponent* ContentsSurface;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FillableComponents")
	UStaticMeshComponent* ContentsMask;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "FillableComponents")
	UStaticMeshComponent* ContentsContainer;

	UPROPERTY(EditAnywhere, Category = "FillConfiguration")
	float Speed = 5.0f;

	UPROPERTY(EditAnywhere, Category = "FillConfiguration")
	float MinLevel = -10.0f;

	UPROPERTY(EditAnywhere, Category = "FillConfiguration")
	float MaxLevel = 0.0f;

	UPROPERTY(EditAnywhere, Category = "FillConfiguration")
	float DeformMin = 1.0f;

	UPROPERTY(EditAnywhere, Category = "FillConfiguration")
	float DeformMax = 1.0f;

	bool Fill = true;

public:
	AFillable();

	virtual void Tick(float DeltaTime) override;

	void FillUp();
	
	void FillUp(float Target);

	void Drain();
	
	void Drain(float Target);
	
	void Drain(float Target, float TargetSpeed);

	void Empty();

	virtual void Stop();

	bool IsFull() const;

	bool IsEmpty() const;
	
	float GetContentsLevel() const;

	bool Transferring = false;

	UPROPERTY(BlueprintAssignable)
	FOnFillableEmpty OnFillableEmpty;
};
