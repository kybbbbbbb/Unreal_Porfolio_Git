#include "BeHaviorTree/CBTTaskNode_Equip.h"
#include "Characters/CAIController.h"
#include "Characters/CMonsters_AI.h"
#include "../Components/CStateComponent.h"
#include "../Weapons/CEquipment.h"

UCBTTaskNode_Equip::UCBTTaskNode_Equip()
{
	NodeName = "Equip";

	bNotifyTick = true;
}

EBTNodeResult::Type UCBTTaskNode_Equip::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{

	Super::ExecuteTask(OwnerComp, NodeMemory);

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent_reset* weaponComponent = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));

	if (weaponComponent == nullptr)
		return EBTNodeResult::Failed;

	

	if (UnEquip == true)
	{
		EWeaponType currentType = weaponComponent->GetType();

		switch (currentType)
		{
		case EWeaponType::Sword:
			weaponComponent->SetSwordMode();
			break;
		case EWeaponType::Fist:
			weaponComponent->SetFistMode();
			break;
		case EWeaponType::Bow:
			weaponComponent->SetBowMode();
			break;
		case EWeaponType::Max:
			break;
		}
		return EBTNodeResult::Succeeded;
	}

	if (Type == EWeaponType::Max)
		EBTNodeResult::Failed;

	if (Type == weaponComponent->GetType())
		return EBTNodeResult::Succeeded;

	switch (Type)
	{
	case EWeaponType::Sword:
		weaponComponent->SetSwordMode();
		break;
	case EWeaponType::Fist:
		weaponComponent->SetFistMode();
		break;
	case EWeaponType::Bow:
		weaponComponent->SetBowMode();
		break;
	case EWeaponType::Max:
		weaponComponent->SetUnarmedMode();
		return EBTNodeResult::Succeeded;
		break;
	}

	return EBTNodeResult::InProgress;
}




void UCBTTaskNode_Equip::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	Super::TickTask(OwnerComp, NodeMemory, DeltaSeconds);
	if (UnEquip == true)
		return;

	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return;

	UCWeaponComponent_reset* weaponComponent = Cast<UCWeaponComponent_reset>(ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));
	if (weaponComponent == nullptr)
		return;


	UCStateComponent* StateComponent = Cast<UCStateComponent>(ai->GetComponentByClass(UCStateComponent::StaticClass()));
	if (StateComponent == nullptr)
		return;

	if (weaponComponent->GetType() == EWeaponType::Max)
	{
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
		return;
	}
	const bool* isEquip = weaponComponent->GetEquipment()->GetEquipped();
	if (*isEquip && StateComponent->IsIdleMode())
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	else
		FinishLatentTask(OwnerComp, EBTNodeResult::InProgress);
}


//실행중 종료시 행동 태스크
EBTNodeResult::Type UCBTTaskNode_Equip::AbortTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	Super::AbortTask(OwnerComp, NodeMemory);


	ACAIController* controller = Cast<ACAIController>(OwnerComp.GetOwner());
	if (controller == nullptr)
		return EBTNodeResult::Failed;

	ACMonsters_AI* ai = Cast<ACMonsters_AI>(controller->GetPawn());
	if (ai == nullptr)
		return EBTNodeResult::Failed;

	UCWeaponComponent_reset* weapon = Cast<UCWeaponComponent_reset> (ai->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));


	bool bBeginEquip = weapon->GetEquipment()->GetBeginEquipped_AI();

	if (bBeginEquip == true)
	{
		weapon->GetEquipment()->Begin_Equip();
	}
	else
	{
		weapon->GetEquipment()->End_Equip();
	}

	return EBTNodeResult::Aborted;
}
