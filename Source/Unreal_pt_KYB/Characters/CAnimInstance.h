#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Components/CWeaponComponent_reset.h"
#include "CAnimInstance.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCAnimInstance
  @ 설  명: 플레이어의 애니메이션 재생을 위한 애님인스턴스클래스
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCAnimInstance : public UAnimInstance
{
	GENERATED_BODY()


private:
	class ACharacter* OwnerCharacter;
	class UCWeaponComponent_reset* WeaponComponent;
	class UCMovementComponent* MovementComponent;
	class UCStateComponent* StateComponent;

protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bFalling;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Pitch;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Yaw;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsJump = false;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsDamage = false;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	float LFootIKlocation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	float RFootIKlocation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	float pelvisOffset;

	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	FRotator LFRotation;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "IK")
	FRotator RFRotation;

	//활 관련
protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere, Category = "Animation")
	bool bBow_Aiming;



protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeaponType Weapontype = EWeaponType::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	void ChangedAiming(bool bInbool);
	void IsRiding(bool Inbool);
	void SetRiderVertical(float infloat);
	void SetRiderHorizontal(float infloat);

	FORCEINLINE void SetLFootIK(float LFLocation, float RFLocation, float pelvis, FRotator LFootRotation, FRotator RFootRotation)
	{
		LFootIKlocation = LFLocation;
		RFootIKlocation = RFLocation;
		pelvisOffset = pelvis;
		LFRotation = LFootRotation;
		RFRotation = RFootRotation;
	}
	FORCEINLINE bool GetIsJump() { return IsJump; }
	FORCEINLINE void SetHorseJump(bool Inbool) { HorseJump = Inbool; }
	
private:
	FRotator PrevRotation;

protected:
	UPROPERTY(BlueprintReadOnly)
	bool isRiding = false;
	UPROPERTY(BlueprintReadOnly)
	float Vertical = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	float Horizontal = 0.0f;
	UPROPERTY(BlueprintReadOnly)
	bool HorseJump = false;

};
