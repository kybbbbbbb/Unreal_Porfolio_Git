#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CPatrolPath.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: ACPatrolPath
  @ 설  명: AI에서 참조하는 패트롤 기능, 위치 찾아 반환, 루프 등
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API ACPatrolPath : public AActor
{
	GENERATED_BODY()
	
public:	
	ACPatrolPath();
private:
	UPROPERTY(EditAnywhere, Category = "Path")
	bool bLoop;

	UPROPERTY(EditAnywhere, Category = "Path")
	int32 Index;

	UPROPERTY(EditAnywhere, Category = "Path")
	bool bReverse;

private:
	UPROPERTY(VisibleDefaultsOnly)
	class USceneComponent* Root;

	UPROPERTY(VisibleDefaultsOnly)
	class USplineComponent* Spline;

	UPROPERTY(VisibleDefaultsOnly)
	class UTextRenderComponent* Text;

public:
	FORCEINLINE class USplineComponent* GetSpline() { return Spline; }

public:
	void OnConstruction(const FTransform& Transform) override;

protected:
	virtual void BeginPlay() override;

public:
	FVector GetMoveTo();
	void UpdateIndex();

};
