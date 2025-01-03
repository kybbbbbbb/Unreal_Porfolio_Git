#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CMovementComponent.generated.h"

UENUM(BlueprintType)
enum class ESpeedType : uint8
{
	Walk = 0, WeaponSpeed, Run, Sprint, Max,
};

/*------------------------------------------------------------------------

  @ 이  름: UCMovementComponent
  @ 설  명: 기본 이동 기능을 가진 컴포넌트
  @ 콜루트: Player-> InputBinding -> UCMovementComponent
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCMovementComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCMovementComponent();

protected:
	virtual void BeginPlay() override;

private:
	class ACharacter* OwnerCharacter;

private:
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float HorizontalLook = 45;
	UPROPERTY(EditAnywhere, Category = "CameraSpeed")
		float VerticalLook = 45;
	UPROPERTY(EditAnywhere, Category = "Speed")
		float Speed[(uint8)ESpeedType::Max] = {200, 500, 600, 800};


public:
	void SetSpeed(ESpeedType speedtype);
	void OnSprint();

	void WeaponEquipRunSpeed(bool InisEquip);
	void OnRun();
	
	void OnWalk();

	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnHorizontalLook(float InAxis);
	void OnVerticalLook(float InAxis);

	void EnableControlRotation();
	void DisableControlRotation();

	void SetFixedCamera(bool Inbool);

	void Move();
	void Stop();

	bool isEquipWeapon = false;





public:
	FORCEINLINE bool CanMove() { return bCanMove; }
	FORCEINLINE void EnableFixedCamera() { bFixCamera = true; }
	FORCEINLINE void DisableFixedCamera() { bFixCamera = false; }


	//@중요: AI에서 속도를 가져오고 설정을 할 수 있게 만드는 함수
	FORCEINLINE float GetWalkSpeed() { return Speed[(int8)ESpeedType::Walk]; }
	FORCEINLINE float GetRunSpeed() { return Speed[(int8)ESpeedType::Run]; }
	FORCEINLINE float GetSprintSpeed() { return Speed[(int8)ESpeedType::Sprint]; }

private:
	bool bCanMove = true;
	bool bFixCamera;
		
};
