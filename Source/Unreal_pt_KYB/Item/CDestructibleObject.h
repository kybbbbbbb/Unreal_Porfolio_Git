#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "CPortionItemData.h"
#include "CDestructibleObject.generated.h"

UCLASS()
class UNREAL_PT_KYB_API ACDestructibleObject : public AActor
{
	GENERATED_BODY()
	
public:	
	ACDestructibleObject();

protected:
	virtual void BeginPlay() override;

public:
	UPROPERTY(EditAnywhere)
	float Health;

	UPROPERTY(EditAnywhere, Category = "Destructible")
	class UGeometryCollectionComponent* GeometryCollection;

	bool bIsDestroyed;

	void BreakObject(FVector HitLocation, FVector ImpulseDir);
	void DropItem();
	void ChangeColor();

private:
	UPROPERTY(EditAnywhere, Category = "ItemDrop")
	TArray<UCPortionItemData*> DropPortionItem;

	UPROPERTY(EditAnywhere, Category = "ColorChange")
	FLinearColor OriginColor;

};
