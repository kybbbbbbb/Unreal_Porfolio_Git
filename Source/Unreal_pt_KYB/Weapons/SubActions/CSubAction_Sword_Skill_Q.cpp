#include "Weapons/SubActions/CSubAction_Sword_Skill_Q.h"
#include "../CDoAction_Combo.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "GameManager/CUIManager_Game.h"

void UCSubAction_Sword_Skill_Q::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSubAction_Sword_Skill_Q::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}

void UCSubAction_Sword_Skill_Q::Pressed()
{
	Super::Pressed();
	if (bCoolTimeOn == true)
		return;


	bInAction = true;
	UCDoAction_Combo* DoActionCombo = Cast<UCDoAction_Combo>(DoAction);
	if(DoActionCombo != nullptr)
		DoActionCombo->DoAction();
	
	CoolTimeOn();
	
}

void UCSubAction_Sword_Skill_Q::Released()
{

}

void UCSubAction_Sword_Skill_Q::StopSubAction()
{
	Super::StopSubAction();
	bInAction = false;
	State->OffSubActionMode();
}

void UCSubAction_Sword_Skill_Q::CoolTimeOn()
{
	bCoolTimeOn = true;
	Owner->GetWorld()->GetTimerManager().SetTimer(CoolTimehandler, this, &UCSubAction_Sword_Skill_Q::CoolTimeOver, CoolTime, false);
	UCUIManager_Game::GetInstance(Owner->GetWorld())->SetSkillCoolTime(CoolTime, &CoolTimehandler, KeyName);
}

void UCSubAction_Sword_Skill_Q::CoolTimeOver()
{
	bCoolTimeOn = false;
}
