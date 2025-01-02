#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CWeaponStructure.h"
#include "CWeaponAsset.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCWeaponAsset
  @ 설  명: 데이터 에셋으로 각 무기의 모든 데이터를 저장, 에디터에서 추가
  @ 특이사항 : 아래 클래스는 무기마다 가지고 있어야하기에 따로 클래스를 만들어 저장해주었음
				저장해놓지 않으면 덮어씌워지거나 가비지콜렉터에 의해 삭제됌.
				AttachmentResult
				EquipmentResult
				DoActionResult

  @ 콜루트: Player->WeaponComponent->WeaponAsset
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
