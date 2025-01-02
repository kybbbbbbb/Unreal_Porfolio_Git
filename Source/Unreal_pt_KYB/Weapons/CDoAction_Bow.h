
#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "AddOns/CArrow.h"
#include "CDoAction_Bow.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: CDoAction_Bow
  @ 설  명: WeaponAsset에서 호출 받아 활 공격을 수행
  @ 콜루트: Player->WeaponComponent->WeaponAsset->DoAction_Bow
  @ TODO: X

------------------------------------------------------------------------*/

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
	
private:
	//생성할 활 BP 클래스
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACArrow> ArrowClass;

	UPROPERTY()
	TArray<class ACArrow*> Arrows;

	TSet<AActor*> AttackedActor;

	FVector OriginLocation;
	const bool* bEquipped;
	float* Bending;
	bool bAttachedString = false;
	bool isReload = false;
	int8 index = 0;

public:
	class ACArrow* GetAttachedArrow();
	UCDoAction_Bow();

	void BeginPlay(ACharacter* Owner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InData, TArray<FHitData>& InHitDatas, TArray<class UCSubAction_RightMouse*>& InSubActionData) override;
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction()	override;
	void OnBeginEquip() override;
	void OnUnEquip() override;

	FORCEINLINE void OnAttachString() { bAttachedString = true; }
	FORCEINLINE void OffAttachString() { bAttachedString = false; }
	FORCEINLINE void OnReload_Bow() { isReload = true; }
	FORCEINLINE void OffReload_Bow() { isReload = false; }
	FORCEINLINE bool GetisReload() { return isReload; }

private:
	UFUNCTION()
	void OnArrowHit(class AActor* InCauser, class ACharacter* InOtherCharacter, const FHitResult& Hit);

	UFUNCTION()
	void OnArrowEndPlay(class ACArrow* InDestroyer);

	UPROPERTY()
	class USkeletalMeshComponent* SkeletalMesh;
	
	UPROPERTY()
	class UPoseableMeshComponent* PosableMesh;

	void CreateArrow();

public:
	void Tick(float InDeltaTime) override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent) override;

};
