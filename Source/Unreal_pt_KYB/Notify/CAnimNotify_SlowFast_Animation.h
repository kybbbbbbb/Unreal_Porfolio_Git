#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotify_SlowFast_Animation.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotify_SlowFast_Animation : public UAnimNotifyState
{
	GENERATED_BODY()
	
private:
	FString GetNotifyName_Implementation() const override;


	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
