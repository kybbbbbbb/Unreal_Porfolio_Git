#include "BeHaviorTree/CBTTaskNode_Arround.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "NavigationSystem.h"

UCBTTaskNode_Arround::UCBTTaskNode_Arround()
{
	NodeName = "Around";

	bNotifyTick = true;

}

EBTNodeResult::Type UCBTTaskNode_Arround::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	FVector targetLocation = target->GetActorLocation();
	FVector aiLocation = ai->GetActorLocation();


	FRotator lookAtRotation = (targetLocation - aiLocation).Rotation();
	ai->SetActorRotation(FRotator(0, lookAtRotation.Yaw, 0));

	if (FVector::Dist2D(targetLocation, aiLocation) > maxDistance)
	{
		ai->PlayAnimMontage(ForwardMoveMontage);
	}







	return EBTNodeResult::Succeeded;
}

void UCBTTaskNode_Arround::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return;

	ACharacter* target = ai->GetAIBehevior()->GetTarget();
	if (target == nullptr)
		return;

	FVector targetLocation = target->GetActorLocation();
	FVector aiLocation = ai->GetActorLocation();
	FRotator lookAtRotation = (targetLocation - aiLocation).Rotation();
	ai->SetActorRotation(FRotator(0, lookAtRotation.Yaw, 0));  // Yaw¸¸ È¸Àü


	
}

EBTNodeResult::Type UCBTTaskNode_Arround::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	return EBTNodeResult::Aborted;
}
