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


	// ��ǥ ���� ��� (�� ���� ����)
	if (target == nullptr)
		return;

	FVector direction = (target->GetActorLocation() - ai->GetActorLocation()).GetSafeNormal();
	FRotator targetRotation = direction.Rotation();

	// ��ġ(Pitch)�� ��(Roll)�� 0���� �����ϰ� ��(Yaw)�� ����
	targetRotation.Pitch = 0.0f;
	targetRotation.Roll = 0.0f;

	FRotator newRotation = FMath::RInterpTo(ai->GetActorRotation(), targetRotation, DeltaSeconds, 5.0f); // �ε巯�� ȸ��

	// ȸ�� ����
	ai->SetActorRotation(newRotation);

}
