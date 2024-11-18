#include "BeHaviorTree/CBTTaskNode_Speed.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"

UCBTTaskNode_Speed::UCBTTaskNode_Speed()
{
	NodeName = "Speed";

}



EBTNodeResult::Type UCBTTaskNode_Speed::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCMovementComponent* movement =Cast<UCMovementComponent>(ai->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (movement == nullptr)
		return EBTNodeResult::Failed;
	
	movement->SetSpeed(Type);

	Super::ExecuteTask(OwnerComp, NodeMemory);

	return EBTNodeResult::Succeeded;
}
