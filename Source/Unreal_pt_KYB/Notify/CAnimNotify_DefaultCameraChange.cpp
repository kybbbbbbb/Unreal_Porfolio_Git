#include "Notify/CAnimNotify_DefaultCameraChange.h"
#include "GameFramework/Character.h"

FString UCAnimNotify_DefaultCameraChange::GetNotifyName_Implementation() const
{
	return "CameraChange";
}

void UCAnimNotify_DefaultCameraChange::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	ACharacter* character = Cast<ACharacter>(MeshComp->GetOwner());
	if (character == nullptr)
		return;
	UCCameraManagerComponent* CameraComp = character->FindComponentByClass<UCCameraManagerComponent>();
	if (CameraComp == nullptr)
		return;

	if (CameraMode == ECameraEnum::Parrying)
	{
		CameraComp->OnParry();
		return;
	}

	CameraComp->DefaultCameraChange(CameraMode);



}
