#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GeometryCollection/GeometryCollectionComponent.h"
#include "CPortionItemData.h"
#include "CDestructibleObject.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: ACDestructibleObject
  @ 설  명: 부숴지는 오브젝트, 일정 데미지 이상 받으면 부숴짐
  @ 콜루트:
  @ TODO: 지오메트리 컬렉션의 슬립과 웨이크업을 다시 조정해야할 듯

------------------------------------------------------------------------*/

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
