#include "Notify/CAnimNotify_OnVisiable_Arrow.h"
#include "../Components/CWeaponComponent_reset.h"
#include "Weapons/CDoAction_Bow.h"
#include "Components/CStateComponent.h"

FString UCAnimNotify_OnVisiable_Arrow::GetNotifyName_Implementation() const
{
	return "OnArrow";
}

void UCAnimNotify_OnVisiable_Arrow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCStateComponent* state =
		Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	UCWeaponComponent_reset* weapon =
		Cast<UCWeaponComponent_reset>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if (state == nullptr)
		return;
	if (state->IsSubActionMode() == false)
		return;

	if (weapon == nullptr)
		return;
	if (weapon->GetDoAction() == nullptr)
		return;

	UCDoAction_Bow* doaction_bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	if (doaction_bow != nullptr)
	{
		ACArrow* arrow = doaction_bow->GetAttachedArrow();
		if (arrow != nullptr)
		{
			arrow->OnVisibility();
		}
	}

}
