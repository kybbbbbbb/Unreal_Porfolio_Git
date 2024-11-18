#include "Notify/CAnimNotifyState_SweepTraceOnOff.h"
#include "Weapons/CAttachment.h"
#include "../Components/CWeaponComponent_reset.h"

FString UCAnimNotifyState_SweepTraceOnOff::GetNotifyName_Implementation() const
{
	return "SweepTrace";
}

void UCAnimNotifyState_SweepTraceOnOff::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
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
	if (weapon->GetAttachment() == nullptr)
		return;

	ACAttachment* AttachComponent = Cast<ACAttachment>(weapon->GetAttachment());

	if (AttachComponent == nullptr)
		return;

	AttachComponent->OnSweepTrace(Name_Collision);
}

void UCAnimNotifyState_SweepTraceOnOff::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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
	if (weapon->GetAttachment() == nullptr)
		return;

	ACAttachment* AttachComponent = Cast<ACAttachment>(weapon->GetAttachment());

	if (AttachComponent == nullptr)
		return;

	AttachComponent->OffSweepTrace(Name_Collision);

}
