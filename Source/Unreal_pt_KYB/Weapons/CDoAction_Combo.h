#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "CDoAction_Combo.generated.h"

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
