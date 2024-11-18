#include "Notify/CAnimNotify_EndDead.h"
#include "Characters/IDeadable.h"

FString UCAnimNotify_EndDead::GetNotifyName_Implementation() const
{
	return "End_Dead";
}

void UCAnimNotify_EndDead::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	IIDeadable* deadable = Cast<IIDeadable>(MeshComp->GetOwner());
	if (deadable == nullptr)
		return;

	deadable->End_Dead();
}