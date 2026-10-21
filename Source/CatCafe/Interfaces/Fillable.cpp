#include "Interfaces/Fillable.h"

#include "Components/StaticMeshComponent.h"
#include "Engine/GameInstance.h"
#include "Materials/MaterialInstanceDynamic.h"
#include "Subsystems/StencilManager.h"

AFillable::AFillable()
{
	PrimaryActorTick.bCanEverTick = true;

	ContentsBody = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentsBody"));
	ContentsSurface = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentsSurface"));
	ContentsMask = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentsMask"));
	ContentsContainer = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ContentsContainer"));
}

void AFillable::BeginPlay()
{
	Super::BeginPlay();

	if (UStencilManager* StencilManager = GetGameInstance()->GetSubsystem<UStencilManager>())
	{
		StencilIndex = StencilManager->AllocateStencilIndex();

		if (StencilIndex != -1)
		{
			// Applicable for objects with non-transparent bodies, and surfaces that we mask
			ContentsContainer->SetRenderCustomDepth(true);
			ContentsContainer->SetCustomDepthStencilValue(StencilIndex);

			if (ContentsSurface->GetStaticMesh())
			{
				if (UMaterialInstanceDynamic* SurfaceMaterial = ContentsSurface->CreateAndSetMaterialInstanceDynamic(0))
					SurfaceMaterial->SetScalarParameterValue(TEXT("StencilValue"), StencilIndex);
			}

			// Applicable for objects with transparent bodies, with surfaces that do not mask
			ContentsMask->SetRenderCustomDepth(true);
			ContentsMask->SetCustomDepthStencilValue(StencilIndex);

			if (ContentsBody->GetStaticMesh())
			{
				ContentsBody->SetTranslucentSortPriority(10);
				if (UMaterialInstanceDynamic* SurfaceMaterial = ContentsBody->CreateAndSetMaterialInstanceDynamic(0))
					SurfaceMaterial->SetScalarParameterValue(TEXT("StencilValue"), StencilIndex);
			}
		}
	}
}

void AFillable::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (UStencilManager* StencilManager = GetGameInstance()->GetSubsystem<UStencilManager>())
		StencilManager->ReleaseStencilIndex(StencilIndex);
}

void AFillable::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Transferring)
	{
		const float levelRange = MaxLevel - MinLevel;
		const float Target = levelRange * TargetFillAmount + MinLevel;
		float MoveAmount = 0;
		if (Fill && ContentsSurface->GetRelativeLocation().Z <= Target)
			MoveAmount = Speed * DeltaTime;
		else if (!Fill && ContentsSurface->GetRelativeLocation().Z >= Target)
			MoveAmount = -Speed * DeltaTime;
		else
			Stop();

		const FVector DropVector(0.0f, 0.0f, MoveAmount);
		ContentsMask->AddRelativeLocation(DropVector);
		ContentsSurface->AddRelativeLocation(DropVector);

		// Transform the current level to a range for mound scaling
		const float deformRange = DeformMax - DeformMin;
		if (deformRange != 0.0f && levelRange != 0.0f)
		{
			const FVector CurrentScale = ContentsSurface->GetRelativeScale3D();
			const float scaleAmount = (ContentsSurface->GetRelativeLocation().Z - MinLevel) / (levelRange / (DeformMax -
				DeformMin)) + DeformMin;
			const FVector ScaleVector(CurrentScale.X, CurrentScale.Y, scaleAmount);
			ContentsSurface->SetRelativeScale3D(ScaleVector);
		}
	}
}

void AFillable::FillUp()
{
	FillUp(1.0f);
}

void AFillable::FillUp(float Target)
{
	TargetFillAmount = Target;
	Fill = true;
	Transferring = true;
}

void AFillable::Drain()
{
	Drain(0.0f);
}

void AFillable::Drain(float Target)
{
	TargetFillAmount = Target;
	Fill = false;
	Transferring = true;
}

void AFillable::Drain(float Target, float TargetSpeed)
{
	Speed = TargetSpeed;
	Drain(Target);
}

void AFillable::Empty()
{
	FVector ContentsSurfaceLocation = ContentsSurface->GetRelativeLocation();
	ContentsSurface->SetRelativeLocation(FVector(ContentsSurfaceLocation.X, ContentsSurfaceLocation.Y, MinLevel));

	FVector ContentsMaskTranslation = ContentsSurface->GetRelativeLocation() - ContentsSurfaceLocation;
	ContentsMask->AddRelativeLocation(FVector(0.0f, 0.0f, ContentsMaskTranslation.Z));

	const FVector CurrentScale = ContentsSurface->GetRelativeScale3D();
	const FVector ScaleVector(CurrentScale.X, CurrentScale.Y, DeformMin);
	ContentsSurface->SetRelativeScale3D(ScaleVector);
}

void AFillable::Stop()
{
	Transferring = false;
	if (IsEmpty()) OnFillableEmpty.Broadcast();
}

bool AFillable::IsFull() const
{
	return ContentsSurface->GetRelativeLocation().Z >= MaxLevel;
}

bool AFillable::IsEmpty() const
{
	return ContentsSurface->GetRelativeLocation().Z <= MinLevel;
}

float AFillable::GetContentsLevel() const
{
	if (MaxLevel - MinLevel == 0.0f) return 0.0f;
	return (ContentsSurface->GetRelativeLocation().Z - MinLevel) / (MaxLevel - MinLevel);
}
