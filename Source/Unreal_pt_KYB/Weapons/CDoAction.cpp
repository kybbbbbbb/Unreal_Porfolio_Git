#include "Weapons/CDoAction.h"
#include "Components/CStateComponent.h"
#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"


UCDoAction::UCDoAction()
{}

void UCDoAction::BeginPlay(ACharacter* Owner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InData, TArray<FHitData>& InHitDatas, TArray<class UCSubAction_RightMouse*>& InSubActionData)
{
	OwnerCharacter = Owner;
	World = OwnerCharacter->GetWorld();

	State = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	
	DoActionDatas = InData;
	HitDatas = InHitDatas;

	SubActionData = &InSubActionData;
}


void UCDoAction::DoAction()
{
	bInAction = true;
	State->SetActionMode();
}

//AI 캐릭터는 공격 시, 해당 함수를 사용한다. 
void UCDoAction::DoAction_AI(int8 InAttackNumber)
{
	bInAction = true;
	State->SetActionMode();
}

void UCDoAction::Begin_DoAction()
{
	bBeginAction = true;
}

//공격 종료시 노티파이에서 호출.
void UCDoAction::End_DoAction()
{
	State->SetIdleMode();
	Movement->Move();
	bInAction = false;
	bBeginAction = false;
}

//공격 종료 시, 노티파이에서 호출.
void UCDoAction::ResetHitresult()
{
	AttackedActor.Empty();
}

//현재 활성화된 스킬을 리턴하는 함수.
ESubActionEnum UCDoAction::GetCurrentSubActionSkillNumber()
{
	if (SubActionData == nullptr)
		return ESubActionEnum::NoExistSkill;
	if (SubActionData->IsEmpty() == true)
		return ESubActionEnum::NoExistSkill;

	for (UCSubAction_RightMouse* subAction : *SubActionData)
	{
		if (subAction->GetInSubAction_RightMouse() == true)
		{
			ESubActionEnum skillnumber = subAction->GetInSubAction_SkillNumber();
			return skillnumber;
		}
	}

	return ESubActionEnum::NoExistSkill;
}


