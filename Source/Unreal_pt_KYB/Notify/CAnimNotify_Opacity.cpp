#include "Notify/CAnimNotify_Opacity.h"
#include "Characters/IDeadable.h"

FString UCAnimNotify_Opacity::GetNotifyName_Implementation() const
{
	return "Opacity";
}

void UCAnimNotify_Opacity::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	IIDeadable* deadable = Cast<IIDeadable>(MeshComp->GetOwner());
	if (deadable == nullptr)
		return;

	deadable->Opacity_zero();
}