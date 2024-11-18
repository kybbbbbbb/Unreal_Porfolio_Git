#include "Notify/CAnimNotify_Reset_NeutralizeGage.h"
#include "../Characters/CMonsters_AI.h"

FString UCAnimNotify_Reset_NeutralizeGage::GetNotifyName_Implementation() const
{
	return "Reset_NeutralizeGage";
}

void UCAnimNotify_Reset_NeutralizeGage::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACMonsters_AI* monster = Cast<ACMonsters_AI>(MeshComp->GetOwner());

	if (monster == nullptr)
		return;

	monster->SetNuetralizeGage(100.0f);
}
