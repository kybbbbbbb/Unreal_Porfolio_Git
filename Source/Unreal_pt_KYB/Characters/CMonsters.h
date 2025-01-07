#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Characters/IDeadable.h"
#include "CMonsters.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnMonsterDead);

/*------------------------------------------------------------------------

  @ 이  름: ACMonsters
  @ 설  명: 몬스터 부모 액터 클래스, 기본 상태를 지님
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API ACMonsters : public ACharacter, public IIDeadable
{
	GENERATED_BODY()

public:
	ACMonsters();

protected:
	UPROPERTY(VisibleAnywhere)
	class UCStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
	class UCHealthComponent* HealthComponent;

private:
	UPROPERTY(VisibleAnywhere)
	class UCMovementComponent* MovementComponent;


	UPROPERTY(EditAnywhere)
	class UAnimMontage* StandardDamageMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* DeadAnimMontage;

	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontageRight;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontageLeft;
	UPROPERTY(EditAnywhere)
	class UAnimMontage* HitMontageFront;


	UPROPERTY(EditAnywhere)
	class UMaterialInstance* DeadMaterial;

	UPROPERTY(EditAnywhere)
	TArray<UAnimMontage*> ArrowMontage_for_Boss;

protected:
	UPROPERTY(EditAnywhere)
	class UAnimMontage* StunMontage;


protected:
	UPROPERTY(EditAnywhere)
	bool IsBoss = false;

protected:
	virtual void BeginPlay() override;
	virtual void Damage();

public:
	UFUNCTION(BlueprintCallable)
	virtual float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);


private:
	UFUNCTION()
	void OnStateTypeChange(EStateType PrevType, EStateType NewType);
	void Dead();

protected:
	float neutralizeGage = 0.0f;

public:
	virtual void End_Damaged();
	virtual void End_Dead() ;
	void Opacity_zero() override;

	FOnMonsterDead OnMonsterDead;

protected:
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;

	}DamageData;
};
