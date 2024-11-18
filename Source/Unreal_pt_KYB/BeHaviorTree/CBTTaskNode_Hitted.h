#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Hitted.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_Hitted : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UCBTTaskNode_Hitted();
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
};
