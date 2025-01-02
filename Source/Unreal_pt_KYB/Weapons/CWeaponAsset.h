#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructure.h"
#include "CWeaponAsset.generated.h"

/*------------------------------------------------------------------------

  @ ��  ��: UCWeaponAsset
  @ ��  ��: ������ �������� �� ������ ��� �����͸� ����, �����Ϳ��� �߰�
  @ Ư�̻��� : �Ʒ� Ŭ������ ���⸶�� ������ �־���ϱ⿡ ���� Ŭ������ ����� �������־���
				�����س��� ������ ��������ų� �������ݷ��Ϳ� ���� ������.
				AttachmentResult
				EquipmentResult
				DoActionResult

  @ �ݷ�Ʈ: Player->WeaponComponent->WeaponAsset
  @ TODO: X

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UCWeaponAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere)
	TSubclassOf<class ACAttachment> AttachmentClass;
	
	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCEquipment> EquipmentClass;

	UPROPERTY(EditAnywhere)
	FEquipmentData EquipmentData;

	UPROPERTY(EditAnywhere)
	TSubclassOf<class UCDoAction> DoActionClass;

	UPROPERTY(EditAnywhere)
	TArray<FDoActionData> DoActionDatas;

	UPROPERTY(EditAnywhere)
	TArray<FHitData> HitDatas;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<class UCSubAction_RightMouse>> SubActionsClass;

	
public:
	UCWeaponAsset();

	FORCEINLINE class TArray<UCSubAction_RightMouse*>* GetSubAction() { return &SubActionResult; }
	void BeginPlay(ACharacter* OwnerCharacter, class UCWeaponData** OutWeaponData);

private:
	UPROPERTY()
	TArray<class UCSubAction_RightMouse*> SubActionResult;

};
