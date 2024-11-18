#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_FocusFollow.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTService_FocusFollow : public UBTService
{
	GENERATED_BODY()
public:
	UCBTService_FocusFollow();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
