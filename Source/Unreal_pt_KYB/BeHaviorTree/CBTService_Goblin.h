#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_Goblin.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTService_Goblin : public UBTService
{
	GENERATED_BODY()
	
public:
	UCBTService_Goblin();
protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere, Category = "Action")
	float ActionRange = 150;
	UPROPERTY(EditAnywhere, Category = "Action")
	float AvoidTimer = 3;

	bool bCanEvade = true;



private:
	void StartEvadeCooldown(float InAvoidTimer);
	void ResetEvade();
};
