#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "../../../../Plugins/Cameras/GameplayCameras/Source/GameplayCameras/Public/Shakes/LegacyCameraShake.h"
#include "CAnimNotify_CameraShake.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotify_CameraShake : public UAnimNotify
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<ULegacyCameraShake> Camerashake;

	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;


	
};
