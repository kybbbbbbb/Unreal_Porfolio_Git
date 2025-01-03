#include "Components/CStateComponent.h"


void UCStateComponent::GetCompareStateType(EStateType InType, bool& resultbool)
{
	resultbool = (CurrentStateType == InType);
}

void UCStateComponent::IsCheckSubAction(bool& resultbool)
{
	resultbool = bInSubActionMode;
}

UCStateComponent::UCStateComponent()
{

}


void UCStateComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UCStateComponent::SetIdleMode()
{
	ChangedType(EStateType::Idle);
}

void UCStateComponent::SetEvadeMode()
{
	ChangedType(EStateType::Evade);
}

void UCStateComponent::SetEquipMode()
{
	ChangedType(EStateType::Equip);
}

void UCStateComponent::SetActionMode()
{
	ChangedType(EStateType::Action);
}

void UCStateComponent::SetDamagedMode()
{
	ChangedType(EStateType::Damaged);
}

void UCStateComponent::SetDeadMode()
{
	ChangedType(EStateType::Dead);

}

void UCStateComponent::SetParringMode()
{
	ChangedType(EStateType::Parring);
}

void UCStateComponent::SetStunMode()
{
	ChangedType(EStateType::Stun);
}

void UCStateComponent::OnSubActionMode()
{
	bInSubActionMode = true;
}

void UCStateComponent::OffSubActionMode()
{
	bInSubActionMode = false;
}

void UCStateComponent::ChangedType(EStateType InType)
{
	EStateType prevtype = CurrentStateType;
	CurrentStateType = InType;
	if (OnStateTypeChanged.IsBound() != 0)
		OnStateTypeChanged.Broadcast(prevtype, CurrentStateType);

}
