#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructure.h"
#include "CEquipment.generated.h"


//����ġ��Ʈ���� ������ ���� ��������Ʈ.(���, ����)
//���⸦ �ô� �ٿ����ϱ� ����
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentBeginEquip);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEquipmentUnEquip);



//������ ���õ� ������ �����Ѵ�.
//�� ���: ��Ţ������ ����, ������Ʈ������ ��Ţ������ ���º���, ��Ţ�� �����Ʈ ����
//����: �������¿��� ����÷��̷� ȣ�� ������, ��Ţ�����͵� �Բ� �޾� �����Ϳ� ���� ������ �Ѵ�. 
UCLASS()
class UNREAL_PT_KYB_API UCEquipment : public UObject
{
	GENERATED_BODY()

public:
	void BeginPlay(class ACharacter* InOwner, const FEquipmentData& InData);

private:
	UPROPERTY(VisibleDefaultsOnly)//������
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
