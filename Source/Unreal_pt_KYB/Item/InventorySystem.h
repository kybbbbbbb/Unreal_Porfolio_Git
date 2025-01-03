#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CItemDataAsset.h"
#include "InventorySystem.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UInventorySystem
  @ 설  명: Player에 종속되며 습득한 아이템을 관리
  @ 콜루트: item -> interactive -> InventorySystem
  @ TODO: 게임 종료 시 데이터 저장하는 로직 구현

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
