#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CItemDataAsset.h"
#include "InventorySystem.generated.h"

/*------------------------------------------------------------------------

  @ ��  ��: UInventorySystem
  @ ��  ��: Player�� ���ӵǸ� ������ �������� ����
  @ �ݷ�Ʈ: item -> interactive -> InventorySystem
  @ TODO: ���� ���� �� ������ �����ϴ� ���� ����

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UInventorySystem : public UActorComponent
{
	GENERATED_BODY()
	
public:
	UInventorySystem();

public:
	void AddItem(FPrimaryAssetId itemID);
	void Beginplay();

private:
	UPROPERTY(EditAnywhere)
	TArray<FPrimaryAssetId> items;
	UPROPERTY(EditAnywhere)
	TArray<UCItemDataAsset*> ItemDataAssets;
};
