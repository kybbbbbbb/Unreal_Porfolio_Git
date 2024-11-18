#include "Weapons/CEquipment.h"
#include "GameFramework/Character.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"

void UCEquipment::BeginPlay(ACharacter* InOwner, const FEquipmentData& InData)
{
	OwnerCharacter = InOwner;

	Data = InData;
	

	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
}

void UCEquipment::Equip_Implementation()
{
	StateComponent->SetEquipMode();
	if (Data.bCanMove == false)
		MovementComponent->Stop();
	if (Data.bUseControlRotation == true)
		MovementComponent->EnableControlRotation();

	if (Data.Montage != nullptr)
	{
		//몽타주가 실행되면 몽타주의 노티파이에서 아래 작업 함수를 호출
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	MovementComponent->WeaponEquipRunSpeed(true);
}


//장착이 시작되면 해야할 작업들, 노티파이 호출
void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();

}

//장착이 끝나면 해야할 작업들, 노티파이 호출
void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;
	if (Data.bCanMove == false)
		MovementComponent->Move();

	StateComponent->SetIdleMode();

}

//장착이 해제되면 해야할 작업들
void UCEquipment::UnEquip_Implementation()
{
	bEquipped = false;
	if (Data.bUseControlRotation)
		MovementComponent->DisableControlRotation();
	if (OnEquipmentUnEquip.IsBound())
		OnEquipmentUnEquip.Broadcast();

	MovementComponent->WeaponEquipRunSpeed(false);
}
