#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "CInteractableObject.generated.h"

UINTERFACE(MinimalAPI)
class UCInteractableObject : public UInterface
{
	GENERATED_BODY()
};



class UNREAL_PT_KYB_API ICInteractableObject
{
	GENERATED_BODY()


public:
	virtual void Interact() = 0;
};
