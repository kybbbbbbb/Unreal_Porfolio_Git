#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "IDeadable.generated.h"

UINTERFACE(MinimalAPI)
class UIDeadable : public UInterface
{
	GENERATED_BODY()
};
/*------------------------------------------------------------------------

  @ ��  ��: IIDeadable
  @ ��  ��: ���� �� �ִ� ������Ʈ �������̽��� Ȯ�� ����
  @ �ݷ�Ʈ:
  @ TODO:

------------------------------------------------------------------------*/
class UNREAL_PT_KYB_API IIDeadable
{
	GENERATED_BODY()

public:
	virtual void End_Dead() {};
	virtual void Opacity_zero() {};
};
