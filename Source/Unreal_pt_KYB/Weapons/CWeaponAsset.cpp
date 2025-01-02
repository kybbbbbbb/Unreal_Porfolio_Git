#include "Weapons/CWeaponAsset.h"
#include "GameFramework/Character.h"
#include "Weapons/CAttachment.h"
#include "Weapons/CEquipment.h"
#include "Weapons/CDoAction.h"
#include "Weapons/CWeaponData.h"
#include "CSubAction_RightMouse.h"

UCWeaponAsset::UCWeaponAsset()
{
	AttachmentClass = ACAttachment::StaticClass();
	EquipmentClass = UCEquipment::StaticClass();
}

void UCWeaponAsset::BeginPlay(ACharacter* OwnerCharacter, UCWeaponData**OutWeaponData)
{
	ACAttachment* attachment = nullptr;
	if (AttachmentClass != nullptr)
	{
		FActorSpawnParameters params;
		params.Owner = Cast<AActor>(OwnerCharacter);

		attachment = OwnerCharacter->GetWorld()->SpawnActor<ACAttachment>(AttachmentClass, params);
	}

	UCEquipment* equipment = nullptr;
	if (EquipmentClass != nullptr)
	{
		equipment = NewObject<UCEquipment>(this, EquipmentClass);
		equipment->BeginPlay(OwnerCharacter, EquipmentData);

		if (attachment != nullptr)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(attachment, &ACAttachment::OnBeginEquip);
			equipment->OnEquipmentUnEquip.AddDynamic(attachment, &ACAttachment::OnUnEquip);
		}
	}

	UCDoAction* doAction = nullptr;
	if (DoActionClass != nullptr)
	{
		doAction = NewObject<UCDoAction>(this, DoActionClass);
		doAction->BeginPlay(OwnerCharacter, attachment, equipment, DoActionDatas, HitDatas, SubActionResult);

		if (attachment != nullptr)
		{
			attachment->OnAttachmentBeginOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentBeginOverlap);
			attachment->OnAttachmentEndOverlap.AddDynamic(doAction, &UCDoAction::OnAttachmentEndOverlap);
		}
		if (equipment != nullptr)
		{
			equipment->OnEquipmentBeginEquip.AddDynamic(doAction, &UCDoAction::OnBeginEquip);
			equipment->OnEquipmentUnEquip.AddDynamic(doAction, &UCDoAction::OnUnEquip);
		}
	}

	if (SubActionsClass.IsEmpty() == false)
	{
		SubActionResult.Empty();
		if (SubActionResult.IsEmpty())
		{
			for (int32 i = 0; i <= SubActionsClass.Num() - 1; i++)
			{
				if (SubActionsClass[i] != nullptr)
				{
					UCSubAction_RightMouse* SubAction = NewObject<UCSubAction_RightMouse>(this, SubActionsClass[i]);
					SubAction->AddToRoot();
					SubActionResult.Add(SubAction);
					SubAction->BeginPlay(OwnerCharacter, attachment, doAction);
				}
			}
		}
	}
	

	//웨폰데이터클래스에 저장해서 보관
	*OutWeaponData = NewObject<UCWeaponData>();
	(*OutWeaponData)->AttachmentResult = attachment;
	(*OutWeaponData)->EquipmentResult = equipment;
	(*OutWeaponData)->DoActionResult= doAction;
}