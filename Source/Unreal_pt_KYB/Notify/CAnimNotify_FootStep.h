#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "CAnimNotify_FootStep.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotify_FootStep : public UAnimNotify
{
	GENERATED_BODY()

private:
	FString GetNotifyName_Implementation() const override;

	void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
	FName GetSurfaceType(const FVector& Location, const AActor* Actor);
};
