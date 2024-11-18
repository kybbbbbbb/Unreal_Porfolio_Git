#include "BeHaviorTree/CBTTaskNode_Hitted.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CStateComponent.h"
#include "../Components/CAIBehaviorComponent.h"

UCBTTaskNode_Hitted::UCBTTaskNode_Hitted()
{
	NodeName = "Action";

	bNotifyTick = true;


}

EBTNodeResult::Type UCBTTaskNode_Hitted::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	controller->StopMovement();
	

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Hitted::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return;

	UCStateComponent* StateComponent = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	if (StateComponent == nullptr)
		return;

	if (StateComponent->IsDamagedMode() == false && ai->GetAIBehevior()->IsWaitMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);



}
