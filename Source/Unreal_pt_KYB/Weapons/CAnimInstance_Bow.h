#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "CAnimInstance_Bow.generated.h"

/*------------------------------------------------------------------------

  @ ��  ��: CAniminstance_Bow
  @ ��  ��: �÷��̾�� ���� bending���� �޾� Ȱ �ִϸ��̼� ����
  @ �ݷ�Ʈ: CDoAction_Bow�� ���� ���� Bending �ּҸ� �Ѱ� ���޹���
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
