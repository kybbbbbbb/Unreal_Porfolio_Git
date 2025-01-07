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

  @ ��  ��: ACAIController
  @ ��  ��: AI�� ���������� ������ �Ǵ� AI��Ʈ�ѷ�.
  @ �ݷ�Ʈ:
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
	AActor* CurrentPlayerSave;//ť��?

public:
	bool bPlayerAttackable = false;


protected:
	virtual void BeginPlay() override;

protected:
	//@ ����: �������� ����
	//index 0 = �Ϲ� �þ߰�(wait, Approach)
	//index 1 = �׼ǽ� �þ߰�(Action)
	//index 2 = ������ Ư�� �þ߰�
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
