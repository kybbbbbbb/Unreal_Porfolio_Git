#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CWeaponData.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCWeaponData : public UObject
{
	GENERATED_BODY()
	
private:
	friend class UCWeaponAsset;

public:
	FORCEINLINE class UCEquipment* GetEquipment() { return EquipmentResult; }
	FORCEINLINE class ACAttachment* GetAttachment() { return AttachmentResult; }
	FORCEINLINE class UCDoAction* GetDoAction() { return DoActionResult; }

	void DestroyWeaponData();
	void SetUnVisibility_Weapon();
	void SetOnVisibility_Weapon();
private:
	UPROPERTY()
	class ACAttachment* AttachmentResult;
	UPROPERTY()
	class UCEquipment* EquipmentResult;
	UPROPERTY()
	class UCDoAction* DoActionResult;



};
