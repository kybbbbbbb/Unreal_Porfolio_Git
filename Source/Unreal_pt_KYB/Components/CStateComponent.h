#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CStateComponent.generated.h"
UENUM()
enum class EStateType : uint8
{
	Idle =0, Evade, Equip, Damaged, Action, Dead, Parring, Stun,  Max
};

//�ڵ� ���յ��� ���߱����� ��������Ʈ ���, ������ ����
//���� Ÿ���� ����Ǹ� ����� �ܺ� �Լ����� ��� ȣ��ǰ� ����
//���̳���(����ȭ,�������� ���ε� ����), ��Ƽĳ��Ʈ(�Լ��� ������ ���� �� ����)
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FStateTypeChanged, EStateType, PrevType, EStateType, NewType);


UCLASS()
class UNREAL_PT_KYB_API UCStateComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	//�ܺο��� ���� ���¸� Ȯ���ϱ� ���� �Լ���
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
	//��������Ʈ ����� ���� �Լ�
	FStateTypeChanged OnStateTypeChanged;
	//���� �������� ���¸� Ȯ���ϱ� ���� ���, ��������Ʈ
	UFUNCTION()
	void GetCompareStateType(EStateType InType, bool& resultbool);
	UFUNCTION()
	void IsCheckSubAction(bool& resultbool);

public:	
	UCStateComponent();

protected:
	virtual void BeginPlay() override;

public:
	//�ܺο��� ���¸� �����ϱ� ���� �Լ�
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
	//����� ������ ���������� ���¸� �����ϴ� �Լ�
	void ChangedType(EStateType InType);



private:
	UPROPERTY(VisibleAnywhere)//���� Ȯ���� ����
	EStateType CurrentStateType = EStateType::Idle;//���� Ÿ�� ����

	bool bInSubActionMode =false;
		
};
