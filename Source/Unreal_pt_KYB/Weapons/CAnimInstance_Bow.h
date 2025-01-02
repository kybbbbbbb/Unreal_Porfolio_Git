#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: CAniminstance_Bow
  @ 설  명: 플레이어로 부터 bending값을 받아 활 애니메이션 연동
  @ 콜루트: CDoAction_Bow에 내부 변수 Bending 주소를 넘겨 전달받음
  @ TODO: X

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UCAnimInstance_Bow : public UAnimInstance
{
	GENERATED_BODY()

protected:
	UPROPERTY(BlueprintReadOnly, EditAnywhere)
	float Bending;

public:
	FORCEINLINE float* GetBending() { return &Bending; }

public:
	void NativeBeginPlay() override;
	void NativeUpdateAnimation(float DeltaSeconds) override;

};
