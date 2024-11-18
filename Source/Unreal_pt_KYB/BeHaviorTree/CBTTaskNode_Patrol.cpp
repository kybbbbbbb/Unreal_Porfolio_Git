#include "BeHaviorTree/CBTTaskNode_Patrol.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "NavigationSystem.h"
#include "Navigation/PathFollowingComponent.h"
#include "CPatrolPath.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/CMovementComponent.h"

UCBTTaskNode_Patrol::UCBTTaskNode_Patrol()
{
	NodeName = "Patrol";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Patrol::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
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

	controller->SetSightConfigByIndex(0);

	if (ai->GetPatrolPath() != nullptr)
	{
		FVector moveToPoint = ai->GetPatrolPath()->GetMoveTo();
		behavior->SetPatrolLocation(moveToPoint);

		if (bDebugMode)
			DrawDebugSphere(ai->GetWorld(), moveToPoint, 10, 10, FColor::Green, false, 10);

		return EBTNodeResult::InProgress;
	}

	FVector Location = ai->GetActorLocation();
	UNavigationSystemV1* navigation = FNavigationSystem::GetCurrent<UNavigationSystemV1>(ai->GetWorld());
	if (navigation == nullptr)
		return EBTNodeResult::Failed;

	FNavLocation point(Location);

	//@�߿�: AI �� �� �ִ� ���� ���� �˻�(��� ���� �ʿ�)
	while (true)
	{
		if (navigation->GetRandomPointInNavigableRadius(Location, RandomRadius, point))
			break;
	}
	point.Location += FVector(0, 0, 50);
	behavior->SetPatrolLocation(point.Location);

	if (bDebugMode)
		DrawDebugSphere(ai->GetWorld(), point.Location, 10, 10, FColor::Green, false, 10);

	return EBTNodeResult::InProgress;

}

void UCBTTaskNode_Patrol::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return;
	UCAIBehaviorComponent* behavior = Cast< UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	if (behavior == nullptr)
		return;
	

	///////////////////////////////////////////////////////////////////////////
	FVector location = behavior->GetPatrolLocation();
	float distanceToPoint = FVector::Dist(ai->GetActorLocation(), location);

	// ������ ������ �Ÿ� (��: 200 units)
	float slowDownDistance = 150.0f;

	// ��ǥ ��ġ�� ��������� ������ ����
	if (distanceToPoint < slowDownDistance)
	{
		float speedFraction = FMath::Clamp(distanceToPoint / slowDownDistance, 0.1f, 1.0f); // �ּ� �ӵ� 10% ����
		ai->GetCharacterMovement()->MaxWalkSpeed = 200.0 * speedFraction; // �⺻ �ִ� �ӵ� 600
	}
	else
	{
		ai->GetCharacterMovement()->MaxWalkSpeed = 200.0f; // �⺻ �ִ� �ӵ��� �̵�
	}

	// ��ǥ ���� ��� (�� ���� ����)
	FVector direction = (location - ai->GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = direction.Rotation();

	// ��ġ(Pitch)�� ��(Roll)�� 0���� �����ϰ� ��(Yaw)�� ����
	targetRotation.Pitch = 0.0f;
	targetRotation.Roll = 0.0f;

	FRotator newRotation = FMath::RInterpTo(ai->GetActorRotation(), targetRotation, DeltaSeconds, 5.0f); // �ε巯�� ȸ��

	// ȸ�� ����
	ai->SetActorRotation(newRotation);

	// ĳ���͸� ��ǥ �������� �̵�
	FVector moveDirection = newRotation.Vector(); // ���� �������� �̵�
	ai->AddMovementInput(moveDirection);

	// ���� Ȯ�� ����
	if (distanceToPoint < GoalDistance)
	{
		// ��ǥ�� �������� ���� ó��
		if (ai->GetPatrolPath() != nullptr)
		{
			ai->GetPatrolPath()->UpdateIndex();
		}
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
	}

	
	//////////////////////////////////////////////////////////////////////////


	//EPathFollowingRequestResult::Type result = controller->MoveToLocation(location, GoalDistance, false);
	//switch (result)
	//{
	//	case EPathFollowingRequestResult::Failed:
	//	{
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);
	//		break;
	//	}

	//	case EPathFollowingRequestResult::AlreadyAtGoal:
	//	{

	//		if (ai->GetPatrolPath() != nullptr)
	//			ai->GetPatrolPath()->UpdateIndex();
	//		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	//		break;
	//	}

	//}

}
