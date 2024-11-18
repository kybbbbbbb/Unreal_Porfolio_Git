#include "BeHaviorTree/CBTTaskNode_Action.h"
#include "../Characters/CAIController.h"
#include "../Characters/CMonsters_AI.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Components/CStateComponent.h"
#include "../Weapons/CDoAction.h"
#include "../Components/CAIBehaviorComponent.h"

UCBTTaskNode_Action::UCBTTaskNode_Action()
{
	NodeName = "Action";

	bNotifyTick = true;

}

EBTNodeResult::Type UCBTTaskNode_Action::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	
	
	UCWeaponComponent_reset* weaponComponent = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if(weaponComponent == nullptr)
		return EBTNodeResult::Failed;
	if(weaponComponent->GetType() == EWeaponType::Max)
		return EBTNodeResult::Failed;
	if (ai->GetAIBehevior()->IsHittedMode())
		return EBTNodeResult::Failed;

	UCAIBehaviorComponent* beheivior = Cast<UCAIBehaviorComponent>(ai->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));
	ACharacter* target = beheivior->GetTarget();
	if (target == nullptr)
		return EBTNodeResult::Failed;
	UCStateComponent* targetState = Cast<UCStateComponent>(target->GetComponentByClass(UCStateComponent::StaticClass()));
	
	float distance = ai->GetDistanceTo(target);


	if (IsBoss == false)
	{
		uint8 NormalAttackNumber = FMath::RandRange(0, 2);
		weaponComponent->DoAction_AI(NormalAttackNumber);
	}

	if (IsBoss == true)
	{
		if (beheivior->GetPhaseNumber() == 1)
		{
			if (distance > 200.0f)
			{
				int num = FMath::RandRange(0,0);
				switch (num)
				{
				case 0:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::ApproachAttack);
					break;
				default:
					break;
				}
			}
			else
			{

				uint8 NormalAttackNumber = FMath::RandRange(0, 2);
				weaponComponent->DoAction_AI(NormalAttackNumber);
			
			}
		}
		else if (beheivior->GetPhaseNumber() == 2)
		{
			if (distance > 200.0f)
			{
				int num = FMath::RandRange(0, 1);
				switch (num)
				{
				case 0:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Boss_SwordJumpLandCombo);
					break;
				case 1:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Boss_SwordApprochCombo);
					break;
				default:
					break;
				}
			}
			else
			{
				int num = FMath::RandRange(0, 3);
				switch (num)
				{
				case 0:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Boss_SwordJumpLandCombo);
					break;
				case 1:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Normal1);
					break;
				case 2:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Normal2);
					break;
				case 3:
					weaponComponent->DoAction_AI((uint8)EAIAttackNumber::Normal3);
					break;
				default:
					break;
				}
			}

		}




	}

	controller->SetSightConfigByIndex(1);

	return EBTNodeResult::InProgress;
}

void UCBTTaskNode_Action::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return ;

	UCWeaponComponent_reset* weaponComponent = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));
	if (weaponComponent == nullptr)
		return;

	UCStateComponent* StateComponent = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	if (StateComponent == nullptr)
		return;
	if (weaponComponent->GetDoAction() == nullptr)
		return;

	if (StateComponent->IsIdleMode() && weaponComponent->GetDoAction()->GetInAction() == false)
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);

}

EBTNodeResult::Type UCBTTaskNode_Action::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);
	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent_reset* weapon = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	bool bBeginAction = weapon->GetDoAction()->GetBeginAction();

	if (bBeginAction == false)
		weapon->GetDoAction()->Begin_DoAction();
	weapon->GetDoAction()->End_DoAction();



	return EBTNodeResult::Aborted;
}
