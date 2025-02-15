
#include "BeHaviorTree/CBTService_BossMonster.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "../Components/CStateComponent.h"

UCBTService_BossMonster::UCBTService_BossMonster()
{
	NodeName = "BossMonster";

	Interval = 0.1f;
	RandomDeviation = 0.0f;
}

void UCBTService_BossMonster::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);
	check()
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	UCAIBehaviorComponent* beheivior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	UCStateComponent* StateComponent = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));

	ACharacter* target = beheivior->GetTarget();
	if (target == nullptr)
		return;
	UCStateComponent* targetState = Cast<UCStateComponent>(target->GetComponentByClass(UCStateComponent::StaticClass()));


	if (StateComponent->IsDeadMode())
	{
		beheivior->SetDeadMode();
		return;
	}

	if (StateComponent->IsDamagedMode())
		return;

	if (beheivior->IsHittedMode())
		return;


	float distance = ai->GetDistanceTo(target);

	if (distance < ActionRange)
	{
		beheivior->SetActionMode();
		return;
	}
	if (distance >= ActionRange+70.0f)
	{
		if (!bIsRandomModeActive)
		{
			int ran = FMath::RandRange(0, 3);

			if (ran == 0)
			{
				beheivior->SetApproachMode();
				StartRandomModeCooldown(1.5f);
			}
			else
			{
				beheivior->SetActionMode();
			}
		}
		return;
	}

	beheivior->SetApproachMode();

	if (target == nullptr)
	{
		beheivior->SetPatrolMode();
		return;
	}

	return;
}

void UCBTService_BossMonster::StartEvadeCooldown(float InAvoidTimer)
{
	// 회피를 다시 할 수 없도록 설정
	bCanEvade = false;

	FTimerHandle EvadeTimeHandler;

	// 쿨다운 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		EvadeTimeHandler,
		this,
		&UCBTService_BossMonster::ResetEvade,
		InAvoidTimer,
		false
	);
}

void UCBTService_BossMonster::ResetEvade()
{
	bCanEvade = true;
}

void UCBTService_BossMonster::StartRandomModeCooldown(float Duration)
{
	bIsRandomModeActive = true;

	FTimerHandle RandomModeTimerHandle;

	GetWorld()->GetTimerManager().SetTimer(
		RandomModeTimerHandle,
		this,
		&UCBTService_BossMonster::ResetRandomMode,
		Duration,
		false
	);

}

void UCBTService_BossMonster::ResetRandomMode()
{
	bIsRandomModeActive = false;
}
