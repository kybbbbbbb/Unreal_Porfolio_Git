#include "Notify/CAnimNotify_EndJump.h"
#include "../Components/CMovementComponent.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_EndJump::GetNotifyName_Implementation() const
{
	return "EndJump_Stop";
}

void UCAnimNotify_EndJump::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if (character == nullptr)
		return;
	UCMovementComponent* MovementComponent = character->FindComponentByClass<UCMovementComponent>();
	if (MovementComponent == nullptr)
		return;

	MovementComponent->Stop();

}
