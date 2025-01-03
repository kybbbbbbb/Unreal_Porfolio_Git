#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "NavigationSystem.h"
#include "NavigationSystemTypes.h"
#include "CNaviPathSystem.generated.h"

class USplineComponent;

/*------------------------------------------------------------------------

  @ 이  름: UCNaviPathSystem
  @ 설  명: 네비게이션 기능을 가진 컴포넌트
  @ 콜루트: Player-> Press N -> UCNaviPathSystem On
  @ TODO:

------------------------------------------------------------------------*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCNaviPathSystem : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCNaviPathSystem();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void PlayNavigation(FVector targetLocation);
	void NaviOn();
	FORCEINLINE void SetTargetLocation(FVector Location) { targetLocation = Location; }

private:
	ACharacter* owner;

	UPROPERTY(VisibleAnywhere)
	USplineComponent* SplineComponent;
	TArray<class USplineMeshComponent*> SplineMesh;
	FPathFindingQuery Query;

	void OnPathFoundAsync(uint32 QueryID, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path);
	void EndNav();

	FVector targetLocation = FVector::ZeroVector;
	bool bNavOn = false;
	float LastUpdateTime = 0.0f;

public:
	UPROPERTY(EditAnywhere)
	UStaticMesh* SplineMeshAsset;

	UPROPERTY(EditAnywhere)
	UMaterialInstance* SplineMeshMaterial;
		
};
