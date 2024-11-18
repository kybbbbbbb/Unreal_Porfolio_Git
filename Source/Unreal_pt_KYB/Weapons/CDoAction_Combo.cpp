#include "Weapons/CDoAction_Combo.h"
#include "Components/CStateComponent.h"
#include "Engine/DamageEvents.h"
#include "GameFramework/Character.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCDoAction_Combo::DoAction()
{
	if (DoActionDatas.Num() < 1)
		return;

	if (State->IsSubActionMode() == true)
		return;
	ESubActionEnum skillnumber = GetCurrentSubActionSkillNumber();
	if (skillnumber != ESubActionEnum::NoExistSkill)
	{
		DoActionDatas[(int8)skillnumber].DoAction(OwnerCharacter);
		State->OnSubActionMode();
		Super::DoAction();
		return;
	}

	if (bEnable == true)
	{
		bEnable = false;
		bExist = true;
		return;
	}

	
	if (State->IsIdleMode() == false) return;
	if (State->IsEquipMode() == true) return;
	if (State->IsSubActionMode() == false)
	{
		Super::DoAction();

		if (OwnerCharacter->GetCharacterMovement()->IsFalling() == true)
		{
			DoActionDatas[index_Air].DoAction(OwnerCharacter);
			return;
		}
		DoActionDatas[index].DoAction(OwnerCharacter);
		return;
	}

}

void UCDoAction_Combo::DoAction_AI(int8 InAttackNumber)
{
	if (DoActionDatas.Num() < 1)
		return;

	if (State->IsIdleMode() == false) return;
	if (State->IsEquipMode() == true) return;
	if (State->IsSubActionMode() == false)
	{
		Super::DoAction();
		DoActionDatas[InAttackNumber].DoAction(OwnerCharacter);
		index = InAttackNumber;
		return;
	}



}

void UCDoAction_Combo::Begin_DoAction()
{
	Super::Begin_DoAction();
	if (bExist == false)
		return;
	bExist = false;
	AttackedActor.Empty();

	if (OwnerCharacter->GetCharacterMovement()->IsFalling() == true)
	{
		DoActionDatas[++index_Air].DoAction(OwnerCharacter);
		return;
	}
	DoActionDatas[++index].DoAction(OwnerCharacter);
}

void UCDoAction_Combo::End_DoAction()
{
	Super::End_DoAction();
	AttackedActor.Empty();
	
	for (UCSubAction_RightMouse* subAction : *SubActionData)
	{
		if (subAction->GetInSubAction_RightMouse() == true)
		{
			subAction->StopSubAction();
		}
	}

	index = 0;
	index_Air = 16;
}

//추후 일반 액터 오브젝트를 파괴시킬 수도 있으니 InOther를 AActor*로 받아야 할 수도 있다.
void UCDoAction_Combo::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, SweepResult, OverlapComponent);
	if (InOther == nullptr || InOther == OwnerCharacter)
		return;
	if (HitDatas.Num() - 1 < index)
		return;
	if (AttackedActor.Contains(InOther))
		return;


	AttackedActor.Add(InOther);

	ESubActionEnum skillnumber = GetCurrentSubActionSkillNumber();
	if (skillnumber != ESubActionEnum::NoExistSkill)
	{
		HitDatas[(int8)skillnumber].SendDamage(InAttacker, InAttackCauser, InOther, SweepResult, OverlapComponent);
		return;
	}
	HitDatas[index].SendDamage(InAttacker, InAttackCauser, InOther, SweepResult, OverlapComponent);
}

void UCDoAction_Combo::OnAttachmentEndOverlap(ACharacter* InAttacker, ACharacter* InOther)
{

}
