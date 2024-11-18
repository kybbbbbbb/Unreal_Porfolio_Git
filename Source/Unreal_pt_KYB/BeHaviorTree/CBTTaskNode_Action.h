#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Action.generated.h"

UENUM()
enum class EAIAttackNumber : uint8
{
	Normal1, Normal2, Normal3, RangeAttack, ApproachAttack, Boss_SwordJumpLandCombo, Boss_SwordApprochCombo,
};

UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_Action : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Action();

private:
	UPROPERTY(EditAnywhere)
	bool IsBoss = false;

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
};
