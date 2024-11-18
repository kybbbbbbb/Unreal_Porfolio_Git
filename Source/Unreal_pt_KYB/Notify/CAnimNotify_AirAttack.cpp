#include "Notify/CAnimNotify_AirAttack.h"
#include "../Characters/CPlayer.h"

FString UCAnimNotify_AirAttack::GetNotifyName_Implementation() const
{
	return "AirAttack_TimeLate";
}

void UCAnimNotify_AirAttack::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());

	if (player == nullptr)
		return;
	
	player->StopAirDash_Late();

}
