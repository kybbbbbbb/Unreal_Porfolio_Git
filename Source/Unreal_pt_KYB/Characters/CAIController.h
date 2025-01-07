#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AISenseConfig.h"
#include "CAIController.generated.h"

USTRUCT(BlueprintType)
struct FSightConfig
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float SightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float LoseSightRadius;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float PeripheralVisionAngleDegrees;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectEnemies;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectNeutrals;

	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	bool bDetectFriendlies;
};

/*------------------------------------------------------------------------

  @ 이  름: ACAIController
  @ 설  명: AI는 공통적으로 가지게 되는 AI컨트롤러.
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API ACAIController : public AAIController
{
	GENERATED_BODY()

public:
	ACAIController();
	
private:
	UPROPERTY(VisibleAnywhere)
	class UAIPerceptionComponent* Perception;

private:
	AActor* CurrentPlayerSave;//큐로?

public:
	bool bPlayerAttackable = false;


protected:
	virtual void BeginPlay() override;

protected:
	//@ 설명: 감지조건 설정
	//index 0 = 일반 시야각(wait, Approach)
	//index 1 = 액션시 시야각(Action)
	//index 2 = 보스전 특수 시야각
	UPROPERTY(EditAnywhere, Category = "AI|Perception")
	TArray<FSightConfig> SightConfigs;


protected:
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:
	UPROPERTY()
	class ACMonsters_AI* Monster;
	UPROPERTY()
	class UAISenseConfig_Sight* Sight;
	UPROPERTY()
	class UCAIBehaviorComponent* AIBehavior;
	UPROPERTY()
	class UAISenseConfig_Damage* DamageConfigs;

private:
	UFUNCTION()
	void OnPerceptionUpdate(const TArray<AActor*>& UpdatedActors);
	
public:
	void SetSightConfigByIndex(int32 Index);
	void Dead();

};
