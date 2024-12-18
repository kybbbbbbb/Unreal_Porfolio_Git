#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Engine/DamageEvents.h"
#include "Particles/ParticleSystem.h"
#include "CWeaponStructure.generated.h"

/*------------------------------------------------------------------------
 
  @ 이  름: WeaponStructure
  @ 설  명: 모든 무기 데이터와 액션 정보를 가지고 있음, 실질적인 액션도 여기서 실행
  @ 콜루트: Player->WeaponComponent->WeaponAsset->DoAction->WeaponData(해당 구조체)
  @ TODO: 구조체 내부 값을 추가하거나 제거하고 컴파일시 언리얼상의 구조체 데이터가 사라짐
  구조체 내부구조가 바뀌면 언리얼에서 다시 구조 검사를 진행해서 사라진다고 함, 추후 구조체 데이터를 따로 저장할 수 있는 방법 필요
 
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

	//범위 지정 공격에 사용
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
