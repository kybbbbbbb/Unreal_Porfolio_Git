#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CHealthComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FHPDelegate, float, RemainHP, float, DamageAmount, float, MaxHP);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FMPDelegate, float, RemainMP, float, UseMPAmount, float, MaxMP);

/*------------------------------------------------------------------------

  @ 이  름: UCHealthComponent
  @ 설  명: 기본 체력과 마나를 관리하는 컴포넌트
  @ 콜루트: Damage->HealthComponent, HealthComponent -> UI
  @ TODO:

------------------------------------------------------------------------*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCHealthComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCHealthComponent();

protected:
	virtual void BeginPlay() override;
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	void Damage(float InAmount);
	void UsedMP(float InAmount);

	bool IsDead();
	FORCEINLINE float GetCurrentMp() { return CurrentMP; }
	FORCEINLINE float GetCurrentHp() { return CurrentHealth; }
	FORCEINLINE float GetMaxHp() { return MaxHealth; }


private:
	UPROPERTY(EditAnywhere, Category = "HealthPoint")
	float MaxHealth = 100;

	UPROPERTY(EditAnywhere, Category = "HealthPoint")
	float MaxMana = 100;

	float CurrentHealth;
	float CurrentMP;

	bool bCanRegenMP = true;
	FTimerHandle MPRegenTimerHandle;

	void EnableMPRegen();

public:
	FHPDelegate HPDelegate;
	FMPDelegate MPDelegate;

		
};
