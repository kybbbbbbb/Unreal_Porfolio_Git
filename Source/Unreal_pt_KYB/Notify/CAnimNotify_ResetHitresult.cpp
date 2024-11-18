#include "Notify/CAnimNotify_ResetHitresult.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Weapons/CDoAction.h"

FString UCAnimNotify_ResetHitresult::GetNotifyName_Implementation() const
{
	return "ResetHitresult";
}

void UCAnimNotify_ResetHitresult::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCWeaponComponent_reset* weapon =
		Cast<UCWeaponComponent_reset>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if (weapon == nullptr)
		return;
	if (weapon->GetDoAction() == nullptr)
		return;

	weapon->GetDoAction()->ResetHitresult();
}
