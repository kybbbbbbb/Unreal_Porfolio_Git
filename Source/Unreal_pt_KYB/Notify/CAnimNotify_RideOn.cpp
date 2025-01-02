#include "Notify/CAnimNotify_RideOn.h"
#include "../Characters/CPlayer.h"
#include "../Characters/CHorse.h"

FString UCAnimNotify_RideOn::GetNotifyName_Implementation() const
{
	return "RideOn";
}

void UCAnimNotify_RideOn::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());

	if (player == nullptr)
		return;

	ACHorse* horse = player->GetCurrentHorse();

	if (horse)
	{
		horse->ComplateRiding();
	}

}
