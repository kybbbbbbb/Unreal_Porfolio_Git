#include "Characters/CAnimInstance.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "Components/CMovementComponent.h"
#include "Components/CStateComponent.h"
#include "GameFramework/CharacterMovementComponent.h"

void UCAnimInstance::NativeBeginPlay()
{
	Super::NativeBeginPlay();

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	if (OwnerCharacter == NULL)
		return;

	WeaponComponent = Cast<UCWeaponComponent_reset>(OwnerCharacter->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));
	MovementComponent = Cast<UCMovementComponent>(OwnerCharacter->GetComponentByClass(UCMovementComponent::StaticClass()));
	StateComponent = Cast<UCStateComponent>(OwnerCharacter->GetComponentByClass(UCStateComponent::StaticClass()));
	bBow_Aiming = false;
}

void UCAnimInstance::NativeUpdateAnimation(float DeltaSeconds)
{
	Super::NativeUpdateAnimation(DeltaSeconds);

	if (OwnerCharacter == NULL)
		return;

	speed = OwnerCharacter->GetVelocity().Size2D();

	if(WeaponComponent != nullptr)
		Weapontype = WeaponComponent->GetType();
	
	Pitch = UKismetMathLibrary::FInterpTo(Pitch, OwnerCharacter->GetBaseAimRotation().Pitch, DeltaSeconds, 25);

	IsJump = OwnerCharacter->GetCharacterMovement()->IsFalling();
	IsDamage = StateComponent->IsDamagedMode();
	
	if (WeaponComponent->GetType() != EWeaponType::Max && StateComponent->IsIdleMode())
	{
		if (speed < 0.05f && bBow_Aiming == false)
			return;

		FRotator CameraRotation = OwnerCharacter->GetControlRotation();
		CameraRotation.Pitch = 0.0f;
		CameraRotation.Roll = 0.0f;

		PrevRotation = CameraRotation;
		FRotator CharacterRotation = OwnerCharacter->GetActorRotation();
		CharacterRotation.Pitch = 0.0f;
		CharacterRotation.Roll = 0.0f;
		FRotator TargetRotation = FMath::RInterpTo(CharacterRotation, CameraRotation, DeltaSeconds, 15.0f);
		OwnerCharacter->SetActorRotation(TargetRotation);

		//Calculate Direction using input
		FVector CharacterForward = CameraRotation.Vector().GetSafeNormal2D();
		FVector InputVector = OwnerCharacter->GetLastMovementInputVector().GetSafeNormal2D();
		float DotProduct = FVector::DotProduct(CharacterForward, InputVector);

		float CrossProductZ = FVector::CrossProduct(CharacterForward, InputVector).Z;
		float TargetDirection = FMath::Atan2(CrossProductZ, DotProduct) * (180.0f / PI);

	
		// Interpolate Direction
		Direction = FMath::FInterpTo(Direction, TargetDirection, DeltaSeconds, 15.0f);
	}


}

void UCAnimInstance::ChangedAiming(bool bInbool)
{
	bBow_Aiming = bInbool;
}

void UCAnimInstance::IsRiding(bool Inbool)
{
	isRiding = Inbool;
}

void UCAnimInstance::SetRiderVertical(float infloat)
{
	Vertical = infloat;
}

void UCAnimInstance::SetRiderHorizontal(float infloat)
{
	Horizontal = infloat;
}

//void UCAnimInstance::OnJump()
//{
//	IsJump = true;
//
//}
//
//void UCAnimInstance::OffJump()
//{
//	IsJump = false;
//
//}
