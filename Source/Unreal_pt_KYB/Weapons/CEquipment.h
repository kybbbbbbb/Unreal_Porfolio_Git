#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructure.h"
#include "CEquipment.generated.h"


//어태치먼트와의 연결을 위한 델리게이트.(비긴, 엔드)
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);

/*------------------------------------------------------------------------

  @ 이  름: UCEquipment
  @ 설  명: WeaponAsset에서 장착데이터를 받아 장착에 필요한 상태 변경, 현재 장착 데이터 저장
  @ 콜루트: Player->WeaponComponent->WeaponAsset->Equipment
  @ TODO: X

------------------------------------------------------------------------*/
 
UCLASS()
class UNREAL_PT_KYB_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

private:
	UPROPERTY(VisibleDefaultsOnly)
	class ACharacter* OwnerCharacter;
	UPROPERTY(VisibleDefaultsOnly)
	class UCMovementComponent* MovementComponent;
	UPROPERTY(VisibleDefaultsOnly)
	class UCStateComponent* StateComponent;

public:
	UFUNCTION(BlueprintNativeEvent)
		void Equip();
	void Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
		void UnEquip();
	void UnEquip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void Begin_Equip();
	void Begin_Equip_Implementation();

	UFUNCTION(BlueprintNativeEvent)
	void End_Equip();
	void End_Equip_Implementation();

	FORCEINLINE const bool* GetEquipped() { return &bEquipped; }
	FORCEINLINE bool GetBeginEquipped_AI() { return bBeginEquip; }

public:
	FEquipmentBeginEquip OnEquipmentBeginEquip;
	FEquipmentUnEquip OnEquipmentUnEquip;

private:
	FEquipmentData Data;
	bool bEquipped = false;
	bool bBeginEquip = false;
};
