#include "Notify/CAnimNotify_LookTarget_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_LookTarget_AI::GetNotifyName_Implementation() const
{
	return "LookTarget_AI";
}

void UCAnimNotify_LookTarget_AI::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr)
		return;
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(MeshComp->GetOwner()->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (behavior == nullptr)
		return;
	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
		return;

	ACharacter* ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (ownerCharacter == nullptr)
		return;
	
	
	FVector directionToTarget = target->GetActorLocation() - ownerCharacter->GetActorLocation();
	directionToTarget.Z = 0.0f;
	directionToTarget.Normalize();

	FRotator lookAtRotation = directionToTarget.Rotation();

	// 캐릭터의 회전 설정
	ownerCharacter->SetActorRotation(lookAtRotation);

}
