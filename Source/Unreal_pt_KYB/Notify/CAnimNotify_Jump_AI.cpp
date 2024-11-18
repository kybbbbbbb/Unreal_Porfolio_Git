#include "Notify/CAnimNotify_Jump_AI.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_Jump_AI::GetNotifyName_Implementation() const
{
	return "AI_Jump";
}

void UCAnimNotify_Jump_AI::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr)
		return;

	
	ACharacter* AICharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (AICharacter == nullptr)
		return;

	
	AICharacter->Jump();

}
