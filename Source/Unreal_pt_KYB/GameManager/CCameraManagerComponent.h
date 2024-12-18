#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CCameraManagerComponent.generated.h"

UENUM()
enum class ECameraEnum : uint8
{
	Default,
	Parrying,
	Damaged,
	Evade,
	Stunned,
	Attack_Fist,
	Attack_Sword,

	DefaultStandard,
	DefaultStandardLong,
	DefaultStandardMiddleLong_soketoffset60,
	DefaultLong,
	DefaultShort,
	DefaultShort_soketoffset6,
	DefaultShort_AirAttack,
	DefaultUpDownFix,
};

USTRUCT(BlueprintType)
struct FCameratransform
{
	GENERATED_BODY()


public:
	UPROPERTY(EditAnywhere, Category = "Camera")
	float ArmLength = 200.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FVector SocketOffset = FVector(0.0f, 60.0f, 0.0f);

	UPROPERTY(EditAnywhere, Category = "Camera")
	float FOV = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	FRotator RelativeRotation = FRotator::ZeroRotator;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float TransitionSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	float ReturnTransitionSpeed = 10.0f;

	UPROPERTY(EditAnywhere, Category = "Camera")
	bool bfixedCamera = false;
};

UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class UNREAL_PT_KYB_API UCCameraManagerComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	UCCameraManagerComponent();

protected:
	virtual void BeginPlay() override;

public:
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

private:
	UPROPERTY(EditAnywhere, Category = "Camera Presets")
	TMap<ECameraEnum, FCameratransform> CameraPresets;

	UPROPERTY()
	ECameraEnum CurrentCameraState = ECameraEnum::Default;

	UPROPERTY()
	ECameraEnum DefualtCamera = ECameraEnum::Default;

	UPROPERTY(EditAnywhere, Category = "Camera Presets")
	float DefaultTransitionDuration = 1.5f;



	UPROPERTY()
	USpringArmComponent* SpringArmComponent;

	UPROPERTY()
	UCameraComponent* CameraComponent;

	UPROPERTY()
	class APostProcessVolume* PostProcessVolume;

private:
	UPROPERTY()
	class UCMovementComponent* MovementComponent;

public:
	UPROPERTY(BlueprintReadWrite, EditAnywhere, Category = "Camera")
	FCameratransform cameraSettings_Parry;


public:
	void OnParry();
	void OnDamage();
	void OnEvade();
	void OnFistAttack();
	void OnSwordAttack();
	void OnBowAttack();

	void DefaultCameraChange(ECameraEnum InCameraNum);
	void SetbCameraTick(bool InBool);
	void SetbCameraAttack(bool InBool);

private:
	void SetCameraState(ECameraEnum NewState, float CustomDuration = -1.0f);

private:
	UPROPERTY()
	ACharacter* OwnerCharacter;


	FTimerHandle RestoreTimeHandler;

private:
	bool bOriginalInheritYaw;
	bool bOriginalUsePawnControlRotation;
	bool bOriginalUseControllerRotationYaw;
	bool bOriginalOrientRotationToMovement;

	bool bIsCameraAction = false;

	bool StopCameraAction = false;
	bool StopAttackAction = false;

};
