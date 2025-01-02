#include "Notify/CAnimNotify_AttachString.h"
#include "../Components/CStateComponent.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Weapons/CDoAction_Bow.h"

FString UCAnimNotify_AttachString::GetNotifyName_Implementation() const
{
	return "AttachString";
}

void UCAnimNotify_AttachString::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
		doaction_bow->OnAttachString();
	}
}
