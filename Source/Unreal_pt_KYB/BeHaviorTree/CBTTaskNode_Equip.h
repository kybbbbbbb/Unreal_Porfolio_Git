#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "../Components/CWeaponComponent_reset.h"
#include "CBTTaskNode_Equip.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCBTTaskNode_Equip : public UBTTaskNode
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere, Category = "Weapon")
	EWeaponType Type = EWeaponType::Max;
	UPROPERTY(EditAnywhere, Category = "Weapon")
	bool UnEquip = false;

public:
	UCBTTaskNode_Equip();
private:
	EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

};
