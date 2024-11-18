#include "Weapons/SubActions/CSubAction_Sword_Skill_Q.h"
#include "../CDoAction_Combo.h"
#include "Components/CStateComponent.h"

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



	bInAction = true;
	UCDoAction_Combo* DoActionCombo = Cast<UCDoAction_Combo>(DoAction);
	if(DoActionCombo != nullptr)
		DoActionCombo->DoAction();
	

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
