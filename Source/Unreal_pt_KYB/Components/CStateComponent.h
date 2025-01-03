#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"
UENUM()
enum class EStateType : uint8
{
	Idle =0, Evade, Equip, Damaged, Action, Dead, Parring, Stun,  Max
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, PrevType, EStateType, NewType);

/*------------------------------------------------------------------------

  @ 이  름: UCStateComponent
  @ 설  명: 캐릭터의 상태를 가지고 있는 컴포넌트, 델리게이트로 상태변경 시 브로드캐스트
  @ 콜루트: Player-> UCStateComponent
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//외부에서 현재 상태를 확인하기 위한 함수들
	FORCEINLINE bool IsIdleMode() { return CurrentStateType == EStateType::Idle; }
	FORCEINLINE bool IsActionMode() { return CurrentStateType == EStateType::Action; }
	FORCEINLINE bool IsEvadeMode() { return CurrentStateType == EStateType::Evade; }
	FORCEINLINE bool IsEquipMode() { return CurrentStateType == EStateType::Equip; }
	FORCEINLINE bool IsDeadMode() { return CurrentStateType == EStateType::Dead; }
	FORCEINLINE bool IsParringMode() { return CurrentStateType == EStateType::Parring; }
	FORCEINLINE bool IsDamagedMode() { return CurrentStateType == EStateType::Damaged; }
	FORCEINLINE bool IsStunMode() { return CurrentStateType == EStateType::Stun; }
	FORCEINLINE bool IsSubActionMode() { return bInSubActionMode; }

public:
	//델리게이트 사용을 위한 함수
	FStateTypeChanged OnStateTypeChanged;
	//같은 레벨에서 상태를 확인하기 위해 사용, 델리게이트
	UFUNCTION()
	void GetCompareStateType(EStateType InType, bool& resultbool);
	UFUNCTION()
	void IsCheckSubAction(bool& resultbool);

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	//외부에서 상태를 조작하기 위한 함수
	void SetIdleMode();
	void SetEvadeMode();
	void SetEquipMode();
	void SetActionMode();
	void SetDamagedMode();
	void SetDeadMode();
	void SetParringMode();
	void SetStunMode();

	void OnSubActionMode();
	void OffSubActionMode();



private:
	//명령이 들어오면 내부적으로 상태를 변경하는 함수
	void ChangedType(EStateType InType);



private:
	UPROPERTY(VisibleAnywhere)//상태 확인을 위해
	EStateType CurrentStateType = EStateType::Idle;//현재 타입 저장

	bool bInSubActionMode =false;
		
};
