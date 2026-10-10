#include "AttachUtils.h"

#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"

void AttachUtils::SetCollisionOfHeld(const AActor* Object, const bool Enabled)
{
	if (UPrimitiveComponent* Root = Cast<UPrimitiveComponent>(Object->GetRootComponent())) 
		Root->SetSimulatePhysics(Enabled);
		
	TArray<UPrimitiveComponent*> Components;
	Object->GetComponents<UPrimitiveComponent>(Components);
	for (UPrimitiveComponent* Comp : Components) Comp->SetCollisionEnabled(Enabled 
		? ECollisionEnabled::QueryAndPhysics : ECollisionEnabled::NoCollision);
}