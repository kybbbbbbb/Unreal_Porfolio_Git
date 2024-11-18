#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotifyState.h"
#include "CAnimNotify_ApproachAttack_AI.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimNotify_ApproachAttack_AI : public UAnimNotifyState
{
	GENERATED_BODY()
	
public:
	virtual void NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference) override;
	virtual void NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
private:
	FString GetNotifyName_Implementation() const override;
	FVector GetGroundLocation(FVector StartLocation);

private:
	void CheckDistance(ACharacter* InOwnerCharacter, FVector InFirstLocation, float InCalculateDistance);
	float CalDistance;
	ACharacter* ownerCharacter = nullptr;
	FVector FirstLocation;
	FVector launchVelocity;
	bool bOnAirDash = false;

public:
	UPROPERTY(EditAnywhere)
	float LaunchRate = 1.0f;
	UPROPERTY(EditAnywhere)
	UMaterialInterface* AttackDecal;
	UPROPERTY(EditAnywhere)
	float AttackDecal_Size = 1.0f;
};
