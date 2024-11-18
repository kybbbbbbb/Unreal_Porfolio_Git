#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Weapons/CWeaponStructure.h"
#include "CSubAction_RightMouse.h"
#include "CDoAction.generated.h"

/*------------------------------------------------------------------------
 
  @ ��  ��: UCDoAction
  @ ��  ��: WeaponAsset���� ȣ�� �޾� ������ ����
  @ �ݷ�Ʈ: Player->WeaponComponent->WeaponAsset->DoAction
  @ TODO: X
 
------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCDoAction : public UObject
{
	GENERATED_BODY()

public:
	FORCEINLINE bool GetInAction() { return bInAction; }
	FORCEINLINE bool GetBeginAction() { return bBeginAction; }
	
public:
	virtual void BeginPlay(ACharacter* Owner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InData,TArray<FHitData>& InHitDatas, TArray<class UCSubAction_RightMouse*>& InSubActionData);
	
protected:
	UPROPERTY()
	class ACharacter* OwnerCharacter;
	UPROPERTY()
	class UWorld* World;
	UPROPERTY()
	class UCMovementComponent* Movement;
	UPROPERTY()
	class UCStateComponent* State;

	TArray<FDoActionData> DoActionDatas;
	TArray<FHitData> HitDatas;

	
	TArray<class UCSubAction_RightMouse*>* SubActionData;

	TSet<AActor*> AttackedActor;

public:
	UCDoAction();
	virtual void DoAction();
	virtual void DoAction_AI(int8 InAttackNumber);
	virtual void Begin_DoAction();
	virtual void End_DoAction();

	virtual void Tick(float InDeltaTime) {}

public:
	UFUNCTION()
	virtual void OnBeginEquip() {};
	UFUNCTION()
	virtual void OnUnEquip() {};
	
public:
	UFUNCTION()
	virtual void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent) {};
	UFUNCTION()
	virtual void OnAttachmentEndOverlap(class ACharacter* InAttacker, ACharacter* InOther) {};

public:
	//@�߿�: AI�� ���� �޺� ���ݽ� �ݵ�� �߰��� ��Ƽ���̷� ȣ�� ���
	void ResetHitresult();


public:
	//@�߿�: ���� �������� ����׼��� ���� ��ų�ѹ��� ������ �� �ִ� �Լ�
	ESubActionEnum GetCurrentSubActionSkillNumber();

protected:
	bool bBeginAction = false;
	bool bInAction = false;
};
