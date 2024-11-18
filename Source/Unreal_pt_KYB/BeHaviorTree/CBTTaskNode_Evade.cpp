#include "BeHaviorTree/CBTTaskNode_Evade.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Components/CAIBehaviorComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

UCBTTaskNode_Evade::UCBTTaskNode_Evade()
{
	NodeName = "Action";

}

EBTNodeResult::Type UCBTTaskNode_Evade::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent_reset* weaponComponent = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if (weaponComponent == nullptr)
		return EBTNodeResult::Failed;
	if (weaponComponent->GetType() == EWeaponType::Max)
		return EBTNodeResult::Failed;

	ai->StartEvade();
	return EBTNodeResult::Succeeded;

}
