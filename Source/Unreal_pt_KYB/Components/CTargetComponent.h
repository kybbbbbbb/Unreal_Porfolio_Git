#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CTargetComponent.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCTargetComponent
  @ 설  명: 타겟팅 기능이 있는 컴폰넌트
  @ 콜루트: Player-> Press T -> TargetComponent On
  @ TODO:

------------------------------------------------------------------------*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCTargetComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCTargetComponent();

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;


private:
	class ACharacter* OwnerCharacter;
	class ACharacter* Target;
	class UWidgetComponent* TargetUI;

public:
	void Toggle();
	FORCEINLINE bool IsExistTarget() { return bExistTarget; }
	FORCEINLINE ACharacter* GetExistTarget() { return Target; }

private:
	ACharacter* GetNealyFrontAngle(const TArray<FHitResult>& InHitResult);
	void Start();
	void Change(ACharacter* InCandidate);
	void End();

private:
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;
	UPROPERTY(EditAnywhere, Category = "setting")
	float TraceDistance = 1000.0f;
	UPROPERTY(EditAnywhere, Category = "setting")
	class UParticleSystem* particleAsset;

private:
	float FinishAngle = 2.0f;
	bool bExistTarget = false;
};
