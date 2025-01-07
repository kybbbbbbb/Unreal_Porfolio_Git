#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "../Interface/CInteractableObject.h"
#include "CHorse.generated.h"

USTRUCT()
struct FSpringArmposition
{
	GENERATED_BODY()

public:
	FVector Location;
	FRotator rotation;
	float armLength;
	FVector Socketoffset;
};

/*------------------------------------------------------------------------

  @ 이  름: ACHorse
  @ 설  명: 말에 탑승하기 위해 제작한 말 클래스, posses를 통해 빙의 가능
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API ACHorse : public ACharacter, public ICInteractableObject
{
	GENERATED_BODY()

public:
	ACHorse();

private:
	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	class UCAnimInstance_Horse* animinstance;
	class UCAnimInstance* playeraniminstance;

	UPROPERTY(EditAnywhere)
	UAnimMontage* RightRiding;
	UPROPERTY(EditAnywhere)
	UAnimMontage* LeftRiding;
	UPROPERTY(EditAnywhere)
	UAnimMontage* JumpMontage;

	UPROPERTY(EditAnywhere)
	UAnimMontage* OffLeft;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	ACharacter* rider;
	AActor* Owner;


protected:
	UPROPERTY(BlueprintReadWrite)
	bool CanRiding = false;
	UPROPERTY(BlueprintReadWrite)
	bool isRiding = false;
protected:
	virtual void BeginPlay() override;

public:	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;


	void Interact() override;

private:
	void OnMoveForward(float InAxis);
	void OnMoveRight(float InAxis);
	void OnJump();
	void OffRidingSwitch();
	void offRiding();
	void MouseLeftPressed();
	float forwardAxis = 0.0f;
	float RightAxis = 0.0f;
	bool IsJump = false;
	bool Isfalling = false;
	bool IsDismounting = false;
	FTimerHandle InterpTimerHandle;

	FSpringArmposition defaultposition;
	FSpringArmposition SetRightRun;
	FSpringArmposition SetLeftRun;

public:
	void ComplateRiding();

};
