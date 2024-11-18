#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../GameManager/CCameraManagerComponent.h"
#include "CAnimNotify_DefaultCameraChange.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotify_DefaultCameraChange : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	ECameraEnum CameraMode = ECameraEnum::DefaultStandard;

	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

	
};
