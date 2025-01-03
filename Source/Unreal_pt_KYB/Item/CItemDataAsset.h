#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItemDataAsset.generated.h"



UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon, Portion, Gold
};

/*------------------------------------------------------------------------

  @ 이  름: UCItemDataAsset
  @ 설  명: 무기, 물약 등 아이템을 총괄로 관리, 아이템ID를 부여
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UCItemDataAsset : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MMItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = "base")
	EItemType ItemType;

	UPROPERTY(EditAnywhere, Category = "base")
	UTexture2D* Itemicon;

	UPROPERTY(EditAnywhere, Category = "base")
	FString ItemName;


};
