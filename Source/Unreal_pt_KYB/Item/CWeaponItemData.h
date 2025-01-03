#pragma once

#include "CoreMinimal.h"
#include "Item/CItemDataAsset.h"
#include "CWeaponItemData.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCWeaponItemData
  @ 설  명: 무기 종류의 아이템은 이 에셋 데이터로 만들어서 통일
  @ 콜루트: 
  @ TODO: 무기 추가

------------------------------------------------------------------------*/

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
