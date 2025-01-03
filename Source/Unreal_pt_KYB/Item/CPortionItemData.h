#pragma once

#include "CoreMinimal.h"
#include "Item/CItemDataAsset.h"
#include "CPortionItemData.generated.h"

UENUM(BlueprintType)
enum class EPotionType : uint8
{
	HP,
	MP,
};

USTRUCT(BlueprintType)
struct FPortionData
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = Portion)
	EPotionType PotionType;

	UPROPERTY(EditAnywhere, Category = Portion)
	float Percent;

	UPROPERTY(EditAnywhere, Category = Portion)
	TSubclassOf<class ACPortion> portionBP;
};

/*------------------------------------------------------------------------

  @ 이  름: UCPortionItemData
  @ 설  명: 포션아이템의 데이터가 들어있는 데이터 에셋
  @ 콜루트:
  @ TODO: 

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API UCPortionItemData : public UCItemDataAsset
{
	GENERATED_BODY()
	

public:
	UCPortionItemData();

	virtual FPrimaryAssetId GetPrimaryAssetId() const override
	{
		return FPrimaryAssetId("MMItemData", GetFName());
	}

public:
	UPROPERTY(EditAnywhere, Category = Portion)
	FPortionData PortionData;
};
