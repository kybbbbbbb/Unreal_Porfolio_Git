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
		//��Ÿ�ְ� ����Ǹ� ��Ÿ���� ��Ƽ���̿��� �Ʒ� �۾� �Լ��� ȣ��
		OwnerCharacter->PlayAnimMontage(Data.Montage, Data.PlayRate);
	}
	else
	{
		Begin_Equip();
		End_Equip();
	}

	MovementComponent->WeaponEquipRunSpeed(true);
}


//������ ���۵Ǹ� �ؾ��� �۾���, ��Ƽ���� ȣ��
void UCEquipment::Begin_Equip_Implementation()
{
	bBeginEquip = true;

	if (OnEquipmentBeginEquip.IsBound())
		OnEquipmentBeginEquip.Broadcast();

}

//������ ������ �ؾ��� �۾���, ��Ƽ���� ȣ��
void UCEquipment::End_Equip_Implementation()
{
	bBeginEquip = false;
	bEquipped = true;
	if (Data.bCanMove == false)
		MovementComponent->Move();

	StateComponent->SetIdleMode();

}

//������ �����Ǹ� �ؾ��� �۾���
void UCEquipment::UnEquip_Implementation()
{
	bEquipped = false;
	if (Data.bUseControlRotation)
		MovementComponent->DisableControlRotation();
	if (OnEquipmentUnEquip.IsBound())
		OnEquipmentUnEquip.Broadcast();

	MovementComponent->WeaponEquipRunSpeed(false);
}
