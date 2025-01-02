#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCDoAction_Combo
  @ 설  명: 일반 근거리 무기의 공격 호출
  @ 콜루트: Player->WeaponComponent->WeaponAsset->DoAction(DoAction_Combo0
  @ TODO: X

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UCDoAction_Combo : public UCDoAction
{
	GENERATED_BODY()

public:
	FORCEINLINE void EnableCombo() { bEnable = true; }
	FORCEINLINE void DisableCombo() { bEnable = false; }

public:
	void DoAction() override;
	void DoAction_AI(int8 InAttackNumber) override;
	void Begin_DoAction() override;
	void End_DoAction() override;
	
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent) override;
	void OnAttachmentEndOverlap(class ACharacter* InAttacker, class ACharacter* InOther) override;
	

private:
	int8 index=0;
	int8 index_Air=16;

	bool bEnable = false;
	bool bExist = false;
};
