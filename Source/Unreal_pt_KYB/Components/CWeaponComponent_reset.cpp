#include "Components/CWeaponComponent_reset.h"
#include "Components/CStateComponent.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CWeaponAsset.h"
#include "GameFramework/Character.h"
#include "Misc/AssertionMacros.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CSubAction_RightMouse.h"
#include "../Weapons/CWeaponData.h"
#include "../Weapons/SubActions/CSubAction_RightMouse_Parring.h"
#include "GameFramework/CharacterMovementComponent.h"

UCWeaponComponent_reset::UCWeaponComponent_reset()
{
	PrimaryComponentTick.bCanEverTick = true;
	
}

void UCWeaponComponent_reset::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());

	for (int32 i = 0; i < (int32)EWeaponType::Max; i++)
	{
		if (weaponDataAsset[i] != nullptr)
		{
			weaponDataAsset[i]->BeginPlay(OwnerCharacter, &Datas[i]);
		}
	}


	UCStateComponent* state = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	if (state != nullptr)
	{
		IsSameStateType_Delegate.BindUFunction(state, "GetCompareStateType");
		IsCheckSubAction_Delegate.BindUFunction(state, "IsCheckSubAction");
	}
}

void UCWeaponComponent_reset::SetMode(EWeaponType InType)
{
	bool isSubAction = false;
	IsCheckSubAction_Delegate.ExecuteIfBound(isSubAction);

	if (isSubAction == true)
		return;
	if (OwnerCharacter->GetCharacterMovement()->IsFalling())
		return;
	if (CurrentWeaponType == InType )
	{
		SetUnarmedMode();
		Datas[(uint8)InType]->GetEquipment()->UnEquip();
		return;
	}


	if (CurrentWeaponType != EWeaponType::Max)
	{
		GetEquipment()->UnEquip();
	}


	if (weaponDataAsset[(uint8)InType] != nullptr)
	{
		Datas[(uint8)InType]->GetEquipment()->Equip();
		ChangeType(InType);
	}
}

void UCWeaponComponent_reset::DestroyWeapon()
{
	for (UCWeaponData* data : Datas)
	{
		if (data)
		{
			data->DestroyWeaponData(); // 안전하게 객체 파괴
		}
	}
}

void UCWeaponComponent_reset::SetWeaponUnVisible()
{
	for (UCWeaponData* data : Datas)
	{
		if (data)
		{
			data->SetUnVisibility_Weapon();
		}
	}

}

void UCWeaponComponent_reset::SetWeaponOnVisible()
{
	for (UCWeaponData* data : Datas)
	{
		if (data)
		{
			data->SetOnVisibility_Weapon();
		}
	}

}






void UCWeaponComponent_reset::SetSwordMode()
{
	bool bSameType = false;
	//반드시 아이들 상태에서만 무기 변경이 가능하게 확인
	IsSameStateType_Delegate.ExecuteIfBound(EStateType::Idle, bSameType);


	if (bSameType == true)
		SetMode(EWeaponType::Sword);

}

void UCWeaponComponent_reset::SetFistMode()
{
	bool bSameType = false;
	//반드시 아이들 상태에서만 무기 변경이 가능하게 확인
	IsSameStateType_Delegate.ExecuteIfBound(EStateType::Idle, bSameType);


	if (bSameType == true)
		SetMode(EWeaponType::Fist);

}

void UCWeaponComponent_reset::SetUnarmedMode()
{
	bool bSameType = false;
	//반드시 아이들 상태에서만 무기 변경이 가능하게 확인
	IsSameStateType_Delegate.ExecuteIfBound(EStateType::Idle, bSameType);
	ChangeType(EWeaponType::Max);
	
}

void UCWeaponComponent_reset::SetBowMode()
{
	bool bSameType = false;
	//반드시 아이들 상태에서만 무기 변경이 가능하게 확인
	IsSameStateType_Delegate.ExecuteIfBound(EStateType::Idle, bSameType);

	if (bSameType == true)
		SetMode(EWeaponType::Bow);
}

void UCWeaponComponent_reset::DoAction()
{
	if (CurrentWeaponType == EWeaponType::Max)
		return;

	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return;
	if (GetDoAction() == nullptr)
		return;
	GetDoAction()->DoAction();
}


void UCWeaponComponent_reset::DoAction_AI(int8 InAttackNumber)
{
	if (CurrentWeaponType == EWeaponType::Max)
		return;

	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return;
	if (GetDoAction() == nullptr)
		return;

	GetDoAction()->DoAction_AI(InAttackNumber);

}



void UCWeaponComponent_reset::SubAction_Pressed(FKey PressedKey)
{
	if (CurrentWeaponType == EWeaponType::Max)
		return;
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return;
	if (GetSubAction()->IsEmpty() == true)
		return;

	TArray<UCSubAction_RightMouse*>* subaction = GetSubAction();
	for (int32 i = 0; i < subaction->Num(); i++)
	{
		if (PressedKey == (*subaction)[i]->GetInSubAction_KeyNumber())
		{
			(*subaction)[i]->Pressed();
			return;
		}
	}
}



void UCWeaponComponent_reset::SubAction_Released(FKey ReleasedKey)
{
	if (CurrentWeaponType == EWeaponType::Max)
		return;
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return;
	if (GetSubAction()->IsEmpty() == true)
		return;

	TArray<UCSubAction_RightMouse*>* subaction = GetSubAction();
	for (int32 i = 0; i < subaction->Num(); i++)
	{
		if (ReleasedKey == (*subaction)[i]->GetInSubAction_KeyNumber())
		{
			(*subaction)[i]->Released();
			return;
		}
	}
}

void UCWeaponComponent_reset::SubAction_Parrying(AController* InEventInstigator)
{
	if (CurrentWeaponType == EWeaponType::Max)
		return;
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return;
	if (GetSubAction()->IsEmpty() == true)
		return;

	TArray<UCSubAction_RightMouse*>* subaction = GetSubAction();
	for (int32 i = 0; i < subaction->Num(); i++)
	{

		if (EKeys::RightMouseButton == (*subaction)[i]->GetInSubAction_KeyNumber())
		{
			UCSubAction_RightMouse_Parring* paring = Cast<UCSubAction_RightMouse_Parring>((*subaction)[i]);
			if (paring != nullptr)
				paring->OnParring(CurrentWeaponType, InEventInstigator);
			return;
		}
	}
}



UCEquipment* UCWeaponComponent_reset::GetEquipment()
{
	//언암드면 장착된게 없음으로 종료
	if (CurrentWeaponType == EWeaponType::Max)
		return nullptr;

	//장착되었을때 웨폰 데이터가 없다면 종료
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return nullptr;

	return Datas[(uint8)CurrentWeaponType]->GetEquipment();
}

ACAttachment* UCWeaponComponent_reset::GetAttachment()
{
	//언암드면 장착된게 없음으로 종료
	if (CurrentWeaponType == EWeaponType::Max)
		return nullptr;

	//장착되었을때 웨폰 데이터가 없다면 종료
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return nullptr;

	return Datas[(uint8)CurrentWeaponType]->GetAttachment();
}


UCDoAction* UCWeaponComponent_reset::GetDoAction()
{
	//언암드면 장착된게 없음으로 종료
	if (CurrentWeaponType == EWeaponType::Max)
		return nullptr;

	//장착되었을때 웨폰 데이터가 없다면 종료
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return nullptr;

	return Datas[(uint8)CurrentWeaponType]->GetDoAction();
}

TArray<UCSubAction_RightMouse*>* UCWeaponComponent_reset::GetSubAction()
{
	//언암드면 장착된게 없음으로 종료
	if (CurrentWeaponType == EWeaponType::Max)
		return nullptr;

	//장착되었을때 웨폰 데이터가 없다면 종료
	if (weaponDataAsset[(uint8)CurrentWeaponType] == nullptr)
		return nullptr;

	return weaponDataAsset[(uint8)CurrentWeaponType]->GetSubAction();
}




//현재 타입을 바꿔주고 해당 타입변경에 영향을 받는 델리게이트를 호출
void UCWeaponComponent_reset::ChangeType(EWeaponType InType)
{
	EWeaponType prevType = CurrentWeaponType;
	CurrentWeaponType = InType;

	if (WeaponTypeChanged.IsBound())
		WeaponTypeChanged.Broadcast((int8)CurrentWeaponType);

}


void UCWeaponComponent_reset::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!!GetDoAction())
		GetDoAction()->Tick(DeltaTime);

	if (!!GetSubAction())
	{
		TArray<UCSubAction_RightMouse*>* subaction = GetSubAction();
		for (int32 i = 0; i < subaction->Num(); i++)
		{
			(*subaction)[i]->Tick(DeltaTime);
		}
	}
}


