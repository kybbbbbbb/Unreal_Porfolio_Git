#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"
UENUM()
enum class EStateType : uint8
{
	Idle =0, Evade, Equip, Damaged, Action, Dead, Parring, Stun,  Max
};

//코드 결합도를 낮추기위한 델리게이트 사용, 느슨한 결합
//내부 타입이 변경되면 연결된 외부 함수들이 모두 호출되게 제작
//다이나믹(직렬화,블프에서 바인드 가능), 멀티캐스트(함수를 여러개 받을 수 있음)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, PrevType, EStateType, NewType);


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
