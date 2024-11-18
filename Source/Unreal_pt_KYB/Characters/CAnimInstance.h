#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "../Components/CWeaponComponent_reset.h"
#include "CAnimInstance.generated.h"

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

	//È° °ü·Ã
protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool bBow_Aiming;



protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	EWeaponType Weapontype = EWeaponType::Max;

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	void ChangedAiming(bool bInbool);
	//void OnJump();
	//void OffJump();
	
private:
	FRotator PrevRotation;
};
