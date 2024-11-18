#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "CBTTaskNode_Evade.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_Evade : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UCBTTaskNode_Evade();

private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

private:
	UPROPERTY(EditAnywhere, Category = "Evade")
	float evadeDistance = 400.0f; // 회피 거리
};
