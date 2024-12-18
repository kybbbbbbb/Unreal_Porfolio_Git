#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Horse.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCAnimInstance_Horse : public UAnimInstance
{
	GENERATED_BODY()

private:
	class ACharacter* OwnerCharacter;



protected:
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float speed;

	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	float Direction;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsJump = false;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool IsDamage = false;
	UPROPERTY(BlueprintReadOnly, Category = "Animation")
	bool Isfalling = false;


public:
	void SetForwardSpeed(float Axis);
	void SetRightSpeed(float Axis);
	FORCEINLINE void SetIsJump(bool inbool) { IsJump = inbool; };
public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;
	
};
