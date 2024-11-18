#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CAIBehaviorComponent.generated.h"

UENUM(BlueprintType)
enum class EAIStateType : uint8
{
	Wait = 0, Approach, Action, Patrol, Hitted, Avoid, Around, Dead, Stun, Max,
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAIStateTypeChange, EAIStateType, InPrevType, EAIStateType, InNewType);


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCAIBehaviorComponent : public UActorComponent
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, Category = "Key")
	FName AIStateTypeKey = "AIState";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName TargetKey = "Target";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PatrolLocationKey = "PatrolLocation";

	UPROPERTY(EditAnywhere, Category = "Key")
	FName PhaseKey = "Phase";


private:
	EAIStateType GetType();

public:	
	UCAIBehaviorComponent();

public:
	FORCEINLINE void SetBlackboard(class UBlackboardComponent* InBlackboard) { Blackboard = InBlackboard; }
	FORCEINLINE bool GetbAttackToken() { return isGetAttackTokken; }
	void SetPatrolLocation(const FVector& InLocation);
	void SetPhase(int8 Innumber);
	FVector GetPatrolLocation();

public:
	class ACharacter* GetTarget();


private:
	class ACPlayer* CurrentTarget;
	

protected:
	virtual void BeginPlay() override;


public:
	bool IsWaitMode();
	bool IsApproachMode();
	bool IsActionMode();
	bool IsPatrolMode();
	bool IsHittedMode();
	bool IsArroundMode();
	bool IsAvoidMode();
	bool IsDeadMode();
	bool IsStunMode();

	int8 GetPhaseNumber();

public:
	void SetWaitMode();
	void SetApproachMode();
	void SetActionMode();
	void SetPatrolMode();
	void SetHittedMode();
	void SetArroundMode();
	void SetAvoidMode();
	void SetDeadMode();
	void SetStunMode();
private:
	bool isGetAttackTokken = false;
	bool bTokenOn = false;

private:
	void ChangeType(EAIStateType InType);

public:
	FAIStateTypeChange OnAIStateTypeChanged;


private:
	class UBlackboardComponent* Blackboard;


};
