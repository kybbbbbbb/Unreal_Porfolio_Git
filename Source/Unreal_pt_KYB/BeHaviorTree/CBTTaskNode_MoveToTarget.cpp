#include "BeHaviorTree/CBTTaskNode_MoveToTarget.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "Navigation/PathFollowingComponent.h"

UCBTTaskNode_MoveToTarget::UCBTTaskNode_MoveToTarget()
{
	NodeName = "MoveToTarget";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_MoveToTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCAIBehaviorComponent* behavior = Cast< UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	if (behavior == nullptr)
		return EBTNodeResult::Failed;
	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
		return EBTNodeResult::Failed;


	EPathFollowingRequestResult::Type MoveResult = controller->MoveToActor(target, GoalDistance, true,true);
	// 성공적인 이동 요청 여부에 따라 결과 반환
	if (MoveResult == EPathFollowingRequestResult::RequestSuccessful)
		return EBTNodeResult::InProgress;
	else
		return EBTNodeResult::Failed;
	
}

void UCBTTaskNode_MoveToTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	

	// 타겟 도착 여부 확인
	if (controller->GetMoveStatus() == EPathFollowingStatus::Idle)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
}

EBTNodeResult::Type UCBTTaskNode_MoveToTarget::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);

	// AI Controller 캐스팅
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Aborted;

	// 이동 중지
	controller->StopMovement();

	return EBTNodeResult::Aborted;
}
