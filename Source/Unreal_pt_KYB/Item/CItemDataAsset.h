#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "CItemDataAsset.generated.h"



UENUM(BlueprintType)
enum class EItemType : uint8
{
	Weapon, Portion, Gold
};


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
