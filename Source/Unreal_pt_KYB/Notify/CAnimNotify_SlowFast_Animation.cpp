#include "Notify/CAnimNotify_SlowFast_Animation.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_SlowFast_Animation::GetNotifyName_Implementation() const
{
	return "SlowFast_Animation";
}

void UCAnimNotify_SlowFast_Animation::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyBegin(MeshComp, Animation, TotalDuration, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->CustomTimeDilation = 0.05f;
	}
}

void UCAnimNotify_SlowFast_Animation::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::NotifyEnd(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* Character = Cast<ACharacter>(MeshComp->GetOwner());

	if (Character)
	{
		Character->CustomTimeDilation = 1.0f;
	}
}

