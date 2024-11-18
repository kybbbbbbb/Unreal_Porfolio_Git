
#pragma once

#include "CoreMinimal.h"
#include "Weapons/CDoAction.h"
#include "AddOns/CArrow.h"
#include "CDoAction_Bow.generated.h"

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCDoAction_Bow : public UCDoAction
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditDefaultsOnly, Category = "Arrow")
	TSubclassOf<class ACArrow> ArrowClass;

private:
	void CreateArrow();
public:
	class ACArrow* GetAttachedArrow();

public:
	UCDoAction_Bow();
public:
	void BeginPlay(ACharacter* Owner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InData, TArray<FHitData>& InHitDatas, TArray<class UCSubAction_RightMouse*>& InSubActionData) override;

public:
	void DoAction() override;
	void Begin_DoAction() override;
	void End_DoAction()	override;

public:
	void OnBeginEquip() override;
	void OnUnEquip() override;

public:
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

private:
	UPROPERTY()
	class USkeletalMeshComponent* SkeletalMesh;
	UPROPERTY()
	class UPoseableMeshComponent* PosableMesh;

public:
	void Tick(float InDeltaTime) override;
	void OnAttachmentBeginOverlap(class ACharacter* InAttacker, AActor* InAttackCauser, class ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent) override;

private:
	FVector OriginLocation;
private:
	UPROPERTY()
	TArray<class ACArrow*> Arrows;
	TSet<AActor*> AttackedActor;

private:
	const bool* bEquipped;
	float* Bending;
	bool bAttachedString = false;
	bool isReload = false;
	int8 index = 0;

};
