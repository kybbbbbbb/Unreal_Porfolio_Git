#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Arround.generated.h"


UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_Arround : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Arround();

private:
	UPROPERTY(EditAnywhere)
	float minDistance = 400.0f; // 타겟과 AI 사이 최소 거리

	UPROPERTY(EditAnywhere)
	float maxDistance = 800.0f; // 타겟과 AI 사이 최소 거리

	UPROPERTY(EditAnywhere)
	bool bDebugMode = true;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RightMoveMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* LeftMoveMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* BackwardMoveMontage;
	UPROPERTY(EditAnywhere)
	UAnimMontage* ForwardMoveMontage;


	
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
