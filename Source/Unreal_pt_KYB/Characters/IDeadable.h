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

  @ 이  름: IIDeadable
  @ 설  명: 죽을 수 있는 오브젝트 인터페이스로 확장 구현
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
class UNREAL_PT_KYB_API IIDeadable
{
	GENERATED_BODY()

public:
	virtual void End_Dead() {};
	virtual void Opacity_zero() {};
};
