#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "../Interface/CInteractableObject.h"
#include "CPortion.generated.h"

/*------------------------------------------------------------------------

  @ ��  ��: ACPortion
  @ ��  ��: ���� ���忡 �����ǰ�, ĳ���Ϳ� ��ȣ�ۿ��Ͽ� �κ��丮�� �̵��ϴ� ������Ʈ
  @ �ݷ�Ʈ:
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
