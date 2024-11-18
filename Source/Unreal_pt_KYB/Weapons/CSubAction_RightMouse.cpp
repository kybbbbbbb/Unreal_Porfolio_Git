#include "Weapons/CSubAction_RightMouse.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"

UCSubAction_RightMouse::UCSubAction_RightMouse()
{

}

void UCSubAction_RightMouse::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Owner = InOwner;
	Attachment = InAttachment;
	DoAction = InDoAction;
	
	State = Cast<UCStateComponent>(Owner->GetComponentByClass(UCStateComponent::StaticClass()));
	Movement = Cast<UCMovementComponent>(Owner->GetComponentByClass(UCMovementComponent::StaticClass()));
	
}

void UCSubAction_RightMouse::Pressed()
{
	
	
}

void UCSubAction_RightMouse::Released()
{
	
}

void UCSubAction_RightMouse::StopSubAction()
{
	

	
}
