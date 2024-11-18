#pragma once

#include "CoreMinimal.h"
#include "Item/CItemDataAsset.h"
#include "CWeaponItemData.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCWeaponItemData : public UCItemDataAsset
{
	GENERATED_BODY()

public:
	UCWeaponItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MMItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Weapon)
	class UCWeaponAsset* WeaponClass;

	// TODO : 무기의 스탯 추가
	//UPROPERTY(EditAnywhere, Category = Weapon)
	//FMMCharacterStat WeaponStat;
};
