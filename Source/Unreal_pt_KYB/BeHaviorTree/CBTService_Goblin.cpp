#include "BeHaviorTree/CBTService_Goblin.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "../Components/CStateComponent.h"
#include "../Characters/CPlayer.h"

UCBTService_Goblin::UCBTService_Goblin()
{
	NodeName = "Goblin";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_Goblin::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickNode(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	UCAIBehaviorComponent* beheivior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	UCStateComponent* StateComponent = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	
	ACharacter* target = beheivior->GetTarget();

	if (StateComponent->IsDeadMode())
	{
		beheivior->SetDeadMode();
		return;
	}
	if (StateComponent->IsDamagedMode())
		return;
	if (beheivior->IsHittedMode())
		return;



	if (target == nullptr)
	{
		beheivior->SetPatrolMode();
		
		//beheivior->TargetMisssing();
		//beheivior->SetWaitMode();
		return;
	}

	ACPlayer* player = Cast<ACPlayer>(target);
	if (controller->bPlayerAttackable == true || player == nullptr)
	{
		UCStateComponent* targetState = Cast<UCStateComponent>(target->GetComponentByClass(UCStateComponent::StaticClass()));
		float distance = ai->GetDistanceTo(target);

		if (targetState != nullptr && beheivior->IsHittedMode() == false)
		{
			if (distance < 300.0f && targetState->IsActionMode() && bCanEvade == true)
			{
				bCanEvade = false;
				int32 randomInt = FMath::RandRange(0, 3);

				if (randomInt == 1)
				{
					beheivior->SetAvoidMode();
				}
				StartEvadeCooldown(AvoidTimer);
				return;
			}
		}


		if (distance < ActionRange)
		{
			beheivior->SetActionMode();
			return;
		}
		
		beheivior->SetApproachMode();
		return;
	}

	if (controller->bPlayerAttackable == false && player != nullptr)
	{
		controller->bPlayerAttackable = player->RequestAttackToken();
	}
	beheivior->SetArroundMode();
	
}

void UCBTService_Goblin::StartEvadeCooldown(float InAvoidTimer)
{
	// 회피를 다시 할 수 없도록 설정
	bCanEvade = false;

	FTimerHandle EvadeTimeHandler;

	// 쿨다운 타이머 시작
	GetWorld()->GetTimerManager().SetTimer(
		EvadeTimeHandler,
		this,
		&UCBTService_Goblin::ResetEvade,
		InAvoidTimer,
		false
	);
}

void UCBTService_Goblin::ResetEvade()
{
	// 쿨다운이 끝나면 다시 회피 가능 상태로 설정
	bCanEvade = true;
}