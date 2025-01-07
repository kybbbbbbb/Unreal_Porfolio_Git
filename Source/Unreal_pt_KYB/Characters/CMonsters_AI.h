#pragma once

#include "CoreMinimal.h"
#include "Characters/CMonsters.h"
#include "GenericTeamAgentInterface.h"
#include "../Item/CPortionItemData.h"
#include "../Item/CWeaponItemData.h"
#include "CMonsters_AI.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: ACMonsters_AI
  @ 설  명: 몬스터의 AI관련 로직을 수행하기 위한 자식클래스, 이걸로 액터 제작 필요
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API ACMonsters_AI : public ACMonsters, public IGenericTeamAgentInterface
{
	GENERATED_BODY()

	ACMonsters_AI();
	
private:
	UPROPERTY(VisibleAnywhere)
	class UCWeaponComponent_reset* WeaponComponent;

	UPROPERTY(VisibleAnywhere)
	class UWidgetComponent* WidgetComponent;

	UPROPERTY(VisibleAnywhere)
	class UCAIBehaviorComponent* AIBehavior;

	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
	uint8 TeamID = 2;

	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
	FText MonsterName;


	UPROPERTY(EditDefaultsOnly, Category = "TeamID")
	class UBehaviorTree* BehaviorTree;

	UPROPERTY(EditAnywhere, Category = "Patrol")
	class ACPatrolPath* PatrolPath;

	UPROPERTY(EditAnywhere, Category = "EvadeMontage")
	UAnimMontage* Evade_Back;

	UPROPERTY(EditAnywhere, Category = "ParringMontage")
	UAnimMontage* ParringMontage;

	UPROPERTY(EditAnywhere, Category = "ItemDrop")
	TArray<UCPortionItemData*> DropPortionItem;

	UPROPERTY(EditAnywhere, Category = "ItemDrop")
	TArray<UCWeaponItemData*> DropWeaponItem;

	
public:
	FORCEINLINE uint8 GetTeamID() { return TeamID; }
	FORCEINLINE FText GetMonsterName() { return MonsterName; }
	FORCEINLINE class UBehaviorTree* GetBeheviorTree() { return BehaviorTree; }
	FORCEINLINE class UCAIBehaviorComponent* GetAIBehevior() { return AIBehavior; }
	FORCEINLINE class ACPatrolPath* GetPatrolPath() { return PatrolPath; }
	FGenericTeamId GetGenericTeamId() const override { return FGenericTeamId(TeamID); }
	FORCEINLINE void SetNuetralizeGage(float InNum) { neutralizeGage = InNum; }
	FORCEINLINE bool IsMonsterBoss() { return IsBoss; }

	void ParringAction();

protected:
	virtual void BeginPlay() override;
	void Damage() override;
	void End_Damaged() override;
	void End_Dead() override;

public:
	virtual void Tick(float DeltaTime) override;
	void StartEvade();

	void CheckLaunchDistance();
	void StartLaunch(FVector InLaunchVelocity, float InDistance);

private:
	UFUNCTION()
	void OnHealthPointChange(float RemainHP, float DamageAmount, float MaxHP);

	void Tick_LabelRenderScale();
	bool bOnAirDash = false;
	float targetDistance = 0.0f;
	FVector FirstLocation;


};
