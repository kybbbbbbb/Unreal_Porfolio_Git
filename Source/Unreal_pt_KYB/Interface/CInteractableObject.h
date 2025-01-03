#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInteractableObject.generated.h"

UINTERFACE(MinimalAPI)
class UCInteractableObject : public UInterface
{
	GENERATED_BODY()
};

/*------------------------------------------------------------------------

  @ 이  름: ICInteractableObject
  @ 설  명: 상호작용 인터페이스
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/

class UNREAL_PT_KYB_API ICInteractableObject
{
	GENERATED_BODY()


public:
	virtual void Interact() = 0;
};
