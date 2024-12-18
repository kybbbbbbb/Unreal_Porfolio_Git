#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystem.h"
#include "CWeaponStructure.generated.h"

/*------------------------------------------------------------------------
 
  @ ��  ��: WeaponStructure
  @ ��  ��: ��� ���� �����Ϳ� �׼� ������ ������ ����, �������� �׼ǵ� ���⼭ ����
  @ �ݷ�Ʈ: Player->WeaponComponent->WeaponAsset->DoAction->WeaponData(�ش� ����ü)
  @ TODO: ����ü ���� ���� �߰��ϰų� �����ϰ� �����Ͻ� �𸮾���� ����ü �����Ͱ� �����
  ����ü ���α����� �ٲ�� �𸮾󿡼� �ٽ� ���� �˻縦 �����ؼ� ������ٰ� ��, ���� ����ü �����͸� ���� ������ �� �ִ� ��� �ʿ�
 
------------------------------------------------------------------------*/
class ACSubAction_RangeSkill_BP_Base;



USTRUCT()
struct FEquipmentData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bUseControlRotation = true;
};

USTRUCT()
struct FDoActionData
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* Montage;

	UPROPERTY(EditAnywhere)
	float PlayRate = 1;

	UPROPERTY(EditAnywhere)
	bool bCanMove = true;

	UPROPERTY(EditAnywhere)
	bool bFixedCamera;

	//���� ���� ���ݿ� ���
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACSubAction_RangeSkill_BP_Base> SpawnAttackableBlueprint;

	UPROPERTY(EditAnywhere)
	UFXSystemAsset* SpawnGroundDecal;


public:
	void DoAction(class ACharacter* InOwner, FName sectionName = "");
	void SpawnAttackableSkillTimer(FVector Location, ACharacter* InOwner);
};

USTRUCT(BlueprintType)
struct FHitData
{
	GENERATED_BODY()

public:

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float Power;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	int Launch = 100;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float StopTime;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	TArray<USoundWave*> SoundWaves;
	
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UAnimMontage* HitMontage;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	class UFXSystemAsset* ParticleEffect;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector EffectLocation = FVector::ZeroVector;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector EffectScale = FVector::OneVector;




public:
	void SendDamage(class ACharacter* InAttaker, AActor* InAttackCauser, class ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent);
	void PlayHitStop(UWorld* InWorld);
	void PlaySountWave(class ACharacter* InOwner);
	void PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator);
};

USTRUCT(BlueprintType)
struct FActionDamageEvent : public FDamageEvent
{
	GENERATED_BODY()

public:
	FHitData* HitData = nullptr;
	const FHitResult* SweepResult = nullptr;
	TWeakObjectPtr<UPrimitiveComponent> OverlapComponent;


};

UCLASS()
class UNREAL_PT_KYB_API UCWeaponStructure : public UObject
{
	GENERATED_BODY()

};
