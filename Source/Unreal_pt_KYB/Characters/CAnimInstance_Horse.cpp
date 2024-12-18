#include "Characters/CAnimInstance_Horse.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "CHorse.h"



void UCAnimInstance_Horse::SetForwardSpeed(float Axis)
{
	speed = Axis;
}

void UCAnimInstance_Horse::SetRightSpeed(float Axis)
{
	Direction = Axis;
}

void UCAnimInstance_Horse::NativeBeginPlay()
{

	OwnerCharacter = Cast<ACharacter>(TryGetPawnOwner());

	if (OwnerCharacter == nullptr)
		return;
}

void UCAnimInstance_Horse::NativeUpdateAnimation(float DeltaSeconds)
{

	if (OwnerCharacter == nullptr)
		return;
	
}
