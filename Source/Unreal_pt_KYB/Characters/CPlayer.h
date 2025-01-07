#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "GenericTeamAgentInterface.h"
#include "IDeadable.h"
#include "CPlayer.generated.h"

class ACHorse;
class UCSoundManager;
/*------------------------------------------------------------------------

  @ 이  름: ACPlayer
  @ 설  명: 플레이어 캐릭터 액터, 팀아이디를 가지고 있고 모든 인풋이 들어온다.
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API ACPlayer : public ACharacter, public IGenericTeamAgentInterface, public IIDeadable
{
	GENERATED_BODY()

public:
	ACPlayer();

private:
	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
	uint8 TeamID = 1;

private:
	int32 MaxAttackToken = 2;
	int32 CurrentAttackToken = 0;

protected:
	//UI매니저 틱 호출 용
	virtual void Tick(float DeltaTime) override;

public:
	bool RequestAttackToken();
	void ReleaseAttackToken();



public:
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }

private:
	UPROPERTY(VisibleAnywhere)
		class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
		class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
		class UCMovementComponent* MovementComponent;

	UPROPERTY(VisibleAnywhere)
		class UCWeaponComponent_reset* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
		class UCStateComponent* StateComponent;

	UPROPERTY(VisibleAnywhere)
		class UCTargetComponent* TargetComponent;

	UPROPERTY(VisibleAnywhere)
		class UCInteractiveComponent* InteractiveComponent;

	UPROPERTY(VisibleAnywhere)
	class UCHealthComponent* HealthComponent;


	UPROPERTY(VisibleAnywhere)
	class UCCameraManagerComponent* CameraManagerComponent;

	UPROPERTY(VisibleAnywhere)
	class UInventorySystem* Inventory2;


	UPROPERTY(VisibleAnywhere)
	class UCFootIKComponent* FootIKComponent;

	UPROPERTY(VisibleAnywhere)
	class UCNaviPathSystem* CNaviPathSystem;



private:
	UPROPERTY(EditAnywhere, Category = "EvadeMontage")
	UAnimMontage* Evade_Back;
	UPROPERTY(EditAnywhere, Category = "EvadeMontage")
	UAnimMontage* Evade_Left;
	UPROPERTY(EditAnywhere, Category = "EvadeMontage")
	UAnimMontage* Evade_Right;
	UPROPERTY(EditAnywhere, Category = "EvadeMontage")
	UAnimMontage* Evade_Front;
	UPROPERTY(EditAnywhere, Category = "StandardDamageMontage")
	UAnimMontage* StandardDamageMontage;

	UPROPERTY(EditAnywhere, Category = "DashMontage")
	UAnimMontage* SwordDashMontage;

	UPROPERTY(EditAnywhere, Category = "jumpMontage")
	UAnimMontage* jumpMontage;

	UPROPERTY(EditAnywhere, Category = "jumpMontage")
	UCSoundManager* SoundManager;

	UPROPERTY(EditAnywhere, Category = "DeadMontage")
	UAnimMontage* DeadMontage;

	UPROPERTY(EditAnywhere, Category = "AirDashMontage")
	UAnimMontage* SwordAirDashMontage;

	UPROPERTY(EditAnywhere, Category = "AirDashMontage")
	UAnimMontage* FistAirDashMontage;
public:
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnJump();
	void OnEvade();
	void Evade(FVector Direction);


	void PressDirection(FVector Direction);

	void PressW();
	void ReleaseW();
	void PressS();
	void PressD();
	void PressA();

	void ResetEvadePress();


	void DashAttack_shift_On();
	void DashAttack_shift_Off();
	void StopAirDash();


	void MouseLeftPressed();
	void SubActionQ_Pressed();
	void SubActionQ_Released();	
	void SubActionMR_Pressed();
	void SubActionMR_Released();
	void SubActionE_Pressed();
	void SubActionE_Released();

	void OnInventory();

	private:
		UFUNCTION()
		void OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
			UPrimitiveComponent* OtherComp, FVector NormalImpulse,
			const FHitResult& Hit);
	
protected:
	virtual void BeginPlay() override;

private:
	bool isOnSubAction = false;
	bool bInDash_shift = false;
	bool bPressW = false;
	class UCUIManager_Game* UIManager = nullptr;

	FTimerHandle DoubleTapTimerHandle;
	FTimerHandle GravityEnableTimerHandle;

	bool bIsEvadePress = false;
	FVector LastInputDirection = FVector::ZeroVector;

	FVector DashStartLocation;
	float DesiredDashDistance = 1000.0f;
	bool bOnDash = false;

	void Damage();
	void Dead();

	bool IsRiding = false;

	UPROPERTY(EditAnywhere)
	TSubclassOf<ACHorse> HorseClass;

	void SpawnHorse();

	class ACHorse* OwnHorse;
	class ACHorse* currenthorse;

public:
	void End_Dead() override {};
	void End_Damaged();
	void Opacity_zero() override {};
	void StopAirDash_Late();
	virtual void Landed(const FHitResult& Hit) override;
	void OnRiding(ACHorse* Inhorse);
	void OffRiding();
	ACHorse* GetCurrentHorse() { return currenthorse; }
	UInventorySystem* GetInventory() { return Inventory2; }
	
	

	UFUNCTION(BlueprintCallable)
	void SetWeaponUnVisible();
	UFUNCTION(BlueprintCallable)
	void SetWeaponOnVisible();

public:
	UFUNCTION(BlueprintCallable)
	float TakeDamage(float Damage, struct FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser);

	UFUNCTION()
	void OnStateTypeChange(EStateType PrevType, EStateType NewType);

private:
	//데미지 전달을 위한 이벤트 구조체
	struct FDamageData
	{
		float Power;
		class ACharacter* Attacker;
		class AActor* Causer;

		struct FActionDamageEvent* Event;

	}DamageData;

};
