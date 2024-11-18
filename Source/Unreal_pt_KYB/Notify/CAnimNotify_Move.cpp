#include "Notify/CAnimNotify_Move.h"
#include "../Components/CMovementComponent.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_Move::GetNotifyName_Implementation() const
{
	return "MoveOn";
}

void UCAnimNotify_Move::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
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

	MovementComponent->Move();

}
