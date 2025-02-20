#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "UObject/NameTypes.h"
#include "CAnimNotifyState_Collision.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotifyState_Collision : public UAnimNotifyState
{
	GENERATED_BODY()
	FString GetNotifyName_Implementation() const override;

	void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;

private:
	UPROPERTY(EditAnywhere, Category = "Collision_Name")
	FName Name_Collision = FName();

};
