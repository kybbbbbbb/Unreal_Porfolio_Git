
#include "Notify/CAnimNotify_EndDamaged.h"
#include "../Components/CStateComponent.h"
#include <Characters/CMonsters.h>
#include "../Characters/CPlayer.h"

FString UCAnimNotify_EndDamaged::GetNotifyName_Implementation() const
{
	return "EndDamage";
}

void UCAnimNotify_EndDamaged::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACMonsters* monster = Cast<ACMonsters>(MeshComp->GetOwner());
	ACPlayer* player = Cast<ACPlayer>(MeshComp->GetOwner());
	if (player != nullptr)
	{
		player->End_Damaged();
		return;
	}
	
	if (monster != nullptr)
	{
		monster->End_Damaged();
		return;

	}



	
}
