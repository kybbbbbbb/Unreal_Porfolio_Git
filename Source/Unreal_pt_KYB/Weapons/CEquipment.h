#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructure.h"
#include "CEquipment.generated.h"


//어태치먼트와의 연결을 위한 델리게이트.(비긴, 엔드)
//무기를 뗐다 붙였다하기 위함
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);



//장착에 관련된 역할을 수행한다.
//주 기능: 이큅데이터 저장, 스테이트컴포에 이큅중으로 상태변경, 이큅중 무브먼트 결정
//과정: 웨폰에셋에서 비긴플레이로 호출 받으며, 이큅데이터도 함께 받아 데이터에 따른 동작을 한다. 
UCLASS()
class UNREAL_PT_KYB_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

private:
	UPROPERTY(VisibleDefaultsOnly)//디버깅용
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
