#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_MoveToTarget.generated.h"

/**
 * 
 */
UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_MoveToTarget : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_MoveToTarget();

private:
	UPROPERTY(EditAnywhere, Category = "Patrol")
	bool bDebugMode;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	float GoalDistance = 200;



private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
};
