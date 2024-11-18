

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "CBTService_BossMonster.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTService_BossMonster : public UBTService
{
	GENERATED_BODY()

public:
	UCBTService_BossMonster();

protected:
	void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

private:
	UPROPERTY(EditAnywhere)
	float ActionRange = 200.0f;
	UPROPERTY(EditAnywhere, Category = "Action")
	float AvoidTimer = 3;

	bool bCanEvade = true;
	bool bIsRandomModeActive = false;

private:
	void StartEvadeCooldown(float InAvoidTimer);
	void ResetEvade();
	void StartRandomModeCooldown(float Duration);
	void ResetRandomMode();
};
