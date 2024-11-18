#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Action.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTService_Action : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_Action();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
