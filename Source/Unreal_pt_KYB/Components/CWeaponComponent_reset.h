#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Weapons/CSubAction_RightMouse.h"
#include "CWeaponComponent_reset.generated.h"




UENUM(BlueprintType)
enum class EWeaponType : uint8
{
	Sword, Fist, Bow, Max,
};

DECLARE_DYNAMIC_DELEGATE_TwoParams(FCheckStateType, EStateType, InType, bool&, resultbool);
DECLARE_DYNAMIC_DELEGATE_OneParam(FCheckSubAction, bool&, InType);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FWeaponTypeChanged, int8, InWeaponTypeNum);


UCLASS()
class UNREAL_PT_KYB_API UCWeaponComponent_reset : public UActorComponent
{
	GENERATED_BODY()

public:
	UCWeaponComponent_reset();

private:
	class ACharacter* OwnerCharacter = nullptr;


private:
	UPROPERTY(EditAnywhere, Category = "DataAsset")
	class UCWeaponAsset* weaponDataAsset[(uint8)EWeaponType::Max];

private:
	FCheckStateType IsSameStateType_Delegate;
	FCheckSubAction IsCheckSubAction_Delegate;

public:
	FWeaponTypeChanged WeaponTypeChanged;

protected:
	virtual void BeginPlay() override;

public:
	void SetMode(EWeaponType InType);
	void DestroyWeapon();

	void SetWeaponUnVisible();
	void SetWeaponOnVisible();

public:
	void SetSwordMode();
	void SetFistMode();
	void SetUnarmedMode();
	void SetBowMode();

	void DoAction();
	void DoAction_AI(int8 InAttackNumber);

	void SubAction_Pressed(FKey PressedKey);
	void SubAction_Released(FKey ReleasedKey);
	void SubAction_Parrying(AController* InEventInstigator);

	class UCEquipment* GetEquipment();
	class ACAttachment* GetAttachment();
	class UCDoAction* GetDoAction();
	TArray<UCSubAction_RightMouse*>* GetSubAction();

	FORCEINLINE EWeaponType GetType() { return CurrentWeaponType; }

private:
	void ChangeType(EWeaponType InType);

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	EWeaponType CurrentWeaponType = EWeaponType::Max;

private:
	UPROPERTY()
	class UCWeaponData* Datas[(int8)EWeaponType::Max];
};