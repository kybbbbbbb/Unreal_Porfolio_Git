#include "Weapons/CWeaponData.h"
#include "CAttachment.h"
#include "CEquipment.h"
#include "CDoAction.h"

void UCWeaponData::DestroyWeaponData()
{
	if (AttachmentResult)
	{
		AttachmentResult->ConditionalBeginDestroy(); // ¾ÈÀüÇÏ°Ô °´Ã¼ ÆÄ±«
		AttachmentResult = nullptr;
	}

	if (EquipmentResult)
	{
		EquipmentResult->ConditionalBeginDestroy(); // ¾ÈÀüÇÏ°Ô °´Ã¼ ÆÄ±«
		EquipmentResult = nullptr;
	}

	if (DoActionResult)
	{
		DoActionResult->ConditionalBeginDestroy(); // ¾ÈÀüÇÏ°Ô °´Ã¼ ÆÄ±«
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
