#include "Notify/CAnimNotify_isReload_Bow.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Weapons/CDoAction_Bow.h"
#include "../Components/CStateComponent.h"

FString UCAnimNotify_isReload_Bow::GetNotifyName_Implementation() const
{
	return "Bow_Reload";
}

void UCAnimNotify_isReload_Bow::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCStateComponent* state =
		Cast<UCStateComponent>(MeshComp->GetOwner()->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state == nullptr)
		return;
	if (state->IsSubActionMode() == false)
		return;

	UCWeaponComponent_reset* weapon =
		Cast<UCWeaponComponent_reset>(MeshComp->GetOwner()->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if (weapon == nullptr)
		return;
	if (weapon->GetDoAction() == nullptr)
		return;

	UCDoAction_Bow* doaction_bow = Cast<UCDoAction_Bow>(weapon->GetDoAction());
	if (doaction_bow != nullptr)
	{
		doaction_bow->OnReload_Bow();
	}

}
