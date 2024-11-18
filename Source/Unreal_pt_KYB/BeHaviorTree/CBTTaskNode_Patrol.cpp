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

	//@중요: AI 갈 수 있는 지점 무한 검사(비용 측정 필요)
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

	// 감속을 시작할 거리 (예: 200 units)
	float slowDownDistance = 150.0f;

	// 목표 위치에 가까워지면 감속을 시작
	if (distanceToPoint < slowDownDistance)
	{
		float speedFraction = FMath::Clamp(distanceToPoint / slowDownDistance, 0.1f, 1.0f); // 최소 속도 10% 유지
		ai->GetCharacterMovement()->MaxWalkSpeed = 200.0 * speedFraction; // 기본 최대 속도 600
	}
	else
	{
		ai->GetCharacterMovement()->MaxWalkSpeed = 200.0f; // 기본 최대 속도로 이동
	}

	// 목표 방향 계산 (요 값만 변경)
	FVector direction = (location - ai->GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = direction.Rotation();

	// 피치(Pitch)와 롤(Roll)은 0으로 고정하고 요(Yaw)만 변경
	targetRotation.Pitch = 0.0f;
	targetRotation.Roll = 0.0f;

	FRotator newRotation = FMath::RInterpTo(ai->GetActorRotation(), targetRotation, DeltaSeconds, 5.0f); // 부드러운 회전

	// 회전 적용
	ai->SetActorRotation(newRotation);

	// 캐릭터를 목표 방향으로 이동
	FVector moveDirection = newRotation.Vector(); // 정면 방향으로 이동
	ai->AddMovementInput(moveDirection);

	// 도착 확인 로직
	if (distanceToPoint < GoalDistance)
	{
		// 목표에 도달했을 때의 처리
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
