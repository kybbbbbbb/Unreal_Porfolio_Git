#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/CInteractableObject.h"
#include "CPortion.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: ACPortion
  @ 설  명: 실제 월드에 스폰되고, 캐릭터와 상호작용하여 인벤토리로 이동하는 오브젝트
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API ACPortion : public AActor, public ICInteractableObject
{
	GENERATED_BODY()
	
public:	
	ACPortion();

protected:
	virtual void BeginPlay() override;

public:
	void Interact() override;
	void SetitemID(FPrimaryAssetId InitemID);
	FPrimaryAssetId GetitemID() { return itemID; }

private:
	FPrimaryAssetId itemID;

};
