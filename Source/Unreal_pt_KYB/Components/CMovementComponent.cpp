#include "Components/CMovementComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UCMovementComponent::UCMovementComponent()
{
	
}


void UCMovementComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
}

void UCMovementComponent::SetSpeed(ESpeedType speedtype)
{
	OwnerCharacter->GetCharacterMovement()->MaxWalkSpeed = Speed[(uint8)speedtype];
}


void UCMovementComponent::OnSprint()
{
	if (isEquipWeapon)
		SetSpeed(ESpeedType::WeaponSpeed);
	else
		SetSpeed(ESpeedType::Sprint);
}

void UCMovementComponent::WeaponEquipRunSpeed(bool InisEquip)
{
	isEquipWeapon = InisEquip;
	if (isEquipWeapon)
		SetSpeed(ESpeedType::Walk);
	else
		SetSpeed(ESpeedType::Run);
}

void UCMovementComponent::OnRun()
{
	if (isEquipWeapon == false)
	{
		SetSpeed(ESpeedType::Run);
	}
	else
	{
		SetSpeed(ESpeedType::Walk);
	}
}

void UCMovementComponent::OnWalk()
{
	SetSpeed(ESpeedType::Walk);
}



void UCMovementComponent::OnMoveForward(float InAxis)
{
	if (bCanMove == false)
		return;

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetForwardVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);

}

void UCMovementComponent::OnMoveRight(float InAxis)
{
	if (bCanMove == false)
		return;

	FRotator rotator = FRotator(0, OwnerCharacter->GetControlRotation().Yaw, 0);
	FVector direction = FQuat(rotator).GetRightVector();

	OwnerCharacter->AddMovementInput(direction, InAxis);
}

void UCMovementComponent::OnHorizontalLook(float InAxis)
{
	if (bFixCamera == true)
		return;

	OwnerCharacter->AddControllerYawInput(InAxis * HorizontalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::OnVerticalLook(float InAxis)
{
	if (bFixCamera == true)
		return;
	OwnerCharacter->AddControllerPitchInput(InAxis * VerticalLook * GetWorld()->GetDeltaSeconds());
}

void UCMovementComponent::EnableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
}

void UCMovementComponent::DisableControlRotation()
{
	OwnerCharacter->bUseControllerRotationYaw = false;
	OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = true;
}

void UCMovementComponent::SetFixedCamera(bool Inbool)
{
	bFixCamera = Inbool;
}

void UCMovementComponent::Move()
{
	bCanMove = true;
}

void UCMovementComponent::Stop()
{
	bCanMove = false;
}