#include "Weapons/CWeaponData.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"

void UCWeaponData::DestroyWeaponData()
{
	if (AttachmentResult)
	{
		AttachmentResult->ConditionalBeginDestroy(); // �����ϰ� ��ü �ı�
		AttachmentResult = nullptr;
	}

	if (EquipmentResult)
	{
		EquipmentResult->ConditionalBeginDestroy(); // �����ϰ� ��ü �ı�
		EquipmentResult = nullptr;
	}

	if (DoActionResult)
	{
		DoActionResult->ConditionalBeginDestroy(); // �����ϰ� ��ü �ı�
		DoActionResult = nullptr;
	}
}

void UCWeaponData::SetUnVisibility_Weapon()
{
	if (AttachmentResult)
	{
		AttachmentResult->SetActorHiddenInGame(true); 
	}
}

void UCWeaponData::SetOnVisibility_Weapon()
{
	if (AttachmentResult)
	{
		AttachmentResult->SetActorHiddenInGame(false);
	}
}
