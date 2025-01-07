#include "BeHaviorTree/CBTService_FocusFollow.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"

UCBTService_FocusFollow::UCBTService_FocusFollow()
{
	NodeName = "CFocus_Follow";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_FocusFollow::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	UCAIBehaviorComponent* beheivior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	ACharacter* target = beheivior->GetTarget();


	// 목표 방향 계산 (요 값만 변경)
	if (target == nullptr)
		return;

	FVector direction = (target->GetActorLocation() - ai->GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = direction.Rotation();

	// 피치(Pitch)와 롤(Roll)은 0으로 고정하고 요(Yaw)만 변경
	targetRotation.Pitch = 0.0f;
	targetRotation.Roll = 0.0f;

	FRotator newRotation = FMath::RInterpTo(ai->GetActorRotation(), targetRotation, DeltaSeconds, 5.0f); // 부드러운 회전

	// 회전 적용
	ai->SetActorRotation(newRotation);

}
