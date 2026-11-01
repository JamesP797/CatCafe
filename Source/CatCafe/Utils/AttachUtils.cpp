#include "AttachUtils.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

void AttachUtils::SetCollisionOfHeld(const AActor* Object, const bool Enabled)
{
	UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(Object->GetRootComponent());
	if (!Root)
	{
		return;
	}

	// Only the root is the physics body. Visual child components (such as
	// Fillable's contents meshes) must keep their own collision settings.
	if (Enabled)
	{
		Root->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		Root->SetSimulatePhysics(true);
	}
	else
	{
		Root->SetSimulatePhysics(false);
		Root->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}
}