#include "Notify/CAnimNotify_CameraShake.h"
#include "GameFramework/PlayerController.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_CameraShake::GetNotifyName_Implementation() const
{
	return "CameraShake";
}

void UCAnimNotify_CameraShake::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if (character == nullptr)
		return;
	APlayerController* controller = Cast<APlayerController>(character->GetController<APlayerController>());
	if (controller == nullptr)
		return;

	controller->PlayerCameraManager->StartCameraShake(Camerashake);

}
