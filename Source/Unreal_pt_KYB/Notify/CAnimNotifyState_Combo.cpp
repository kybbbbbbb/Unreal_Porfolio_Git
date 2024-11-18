#include "Notify/CAnimNotifyState_Combo.h"
#include "../Components/CWeaponComponent_reset.h"
#include "Weapons/CDoAction_Combo.h"


FString UCAnimNotifyState_Combo::GetNotifyName_Implementation() const
{
	return "Combo";
}

void UCAnimNotifyState_Combo::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

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

	UCDoAction_Combo* DoAtion_Combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());
	
	if (DoAtion_Combo == nullptr)
		return;

	DoAtion_Combo->EnableCombo();
}

void UCAnimNotifyState_Combo::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

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

	UCDoAction_Combo* DoAtion_Combo = Cast<UCDoAction_Combo>(weapon->GetDoAction());

	if (DoAtion_Combo == nullptr)
		return;

	DoAtion_Combo->DisableCombo();

}
