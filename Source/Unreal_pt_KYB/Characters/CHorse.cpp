#include "Characters/CHorse.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "CPlayer.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "CAnimInstance_Horse.h"
#include "CAnimInstance.h"
#include "Kismet/KismetMathLibrary.h"

ACHorse::ACHorse()
{
	PrimaryActorTick.bCanEverTick = true;

	Camera = CreateDefaultSubobject<UCameraComponent>("Camera");

	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpingArm");
	
	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm);

	SetRightRun.Location	 = FVector(0,0,120);
	SetRightRun.rotation	 = FRotator(0,120,0);
	SetRightRun.armLength	 = 300.0f;
	SetRightRun.Socketoffset = FVector(0,0,100);

	SetLeftRun.Location		 =  FVector(0,0,120);
	SetLeftRun.rotation		 = FRotator(0, 60, 0);
	SetLeftRun.armLength	 =  300.0f;
	SetLeftRun.Socketoffset  =  FVector(0,0,100);

}

void ACHorse::BeginPlay()
{
	Super::BeginPlay();
	animinstance = Cast<UCAnimInstance_Horse>(GetMesh()->GetAnimInstance());
	bUseControllerRotationYaw = false;
	Owner = GetOwner();

	defaultposition.Location = SpringArm->GetRelativeLocation();
	defaultposition.rotation = SpringArm->GetRelativeRotation();
	defaultposition.armLength = SpringArm->TargetArmLength;
	defaultposition.Socketoffset = SpringArm->SocketOffset;
	
	GetCharacterMovement()->bOrientRotationToMovement = true; // �̵� �������� ȸ��
}

void ACHorse::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	if (isRiding == false)
	{
		if (Owner != nullptr)
		{
			FVector OwnerLocation = Owner->GetActorLocation() + Owner->GetActorRightVector()*150.0f;
			FVector HorseLocation = GetActorLocation();
			float DistanceToOwner = FVector::Dist(OwnerLocation, HorseLocation);
			float StopRadius = 300.0f;  // ���� �ݰ�
			float ChaseRadius = 3000.0f; // ���� �ݰ�

			if (DistanceToOwner <= StopRadius)
			{
				// ���� �ݰ� ���� �����Ƿ� ����
				animinstance->SetForwardSpeed(0.0f);
				animinstance->SetRightSpeed(0.0f);

				GetCharacterMovement()->StopMovementImmediately(); // �̵� ����
				return;
			}
			else if (DistanceToOwner <= ChaseRadius)
			{
				FVector DirectionToPlayer = (OwnerLocation - HorseLocation).GetSafeNormal(); // �÷��̾� ����
				FVector HorseForward = GetActorForwardVector(); // ���� ���� ����
				FVector HorseRight = GetActorRightVector();     // ���� ���� ����

				// ���� ��ġ�� ���
				float ForwardSpeed = FVector::DotProduct(HorseForward, DirectionToPlayer); // ���� �� ��ġ
				float RightSpeed = FVector::DotProduct(HorseRight, DirectionToPlayer);     // ���� �� ��ġ

				// �ִϸ��̼ǿ� �� ����
				animinstance->SetForwardSpeed(ForwardSpeed*4);
				animinstance->SetRightSpeed(RightSpeed);
			}

		}
		return;
	}
	if (IsDismounting == true)
	{
		forwardAxis = 0.0f;
		RightAxis = 0.0f;
		animinstance->SetForwardSpeed(0.0f);
		animinstance->SetRightSpeed(0.0f);
		playeraniminstance->SetRiderVertical(0.0f);
		playeraniminstance->SetRiderHorizontal(0.0f);
		return;
	}

	animinstance->SetForwardSpeed(forwardAxis);
	if (playeraniminstance != nullptr)
		playeraniminstance->SetRiderVertical(forwardAxis);
	animinstance->SetRightSpeed(RightAxis);
	if (playeraniminstance != nullptr)
		playeraniminstance->SetRiderHorizontal(RightAxis);

	if (isRiding == true)
	{
		if (RightAxis > 0.2)
		{
			SpringArm->SetRelativeLocation(FMath::VInterpTo(SpringArm->GetRelativeLocation(), SetRightRun.Location, DeltaTime, 3.0f));
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), SetRightRun.rotation, DeltaTime, 3.0f));
			SpringArm->TargetArmLength = (FMath::FInterpTo(SpringArm->TargetArmLength, SetRightRun.armLength, DeltaTime, 3.0f));
			SpringArm->SocketOffset = (FMath::VInterpTo(SpringArm->SocketOffset, SetRightRun.Socketoffset, DeltaTime, 3.0f));
		}
		else if (RightAxis < -0.2)
		{
			SpringArm->SetRelativeLocation(FMath::VInterpTo(SpringArm->GetRelativeLocation(), SetLeftRun.Location, DeltaTime, 3.0f));
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), SetLeftRun.rotation, DeltaTime, 3.0f));
			SpringArm->TargetArmLength = (FMath::FInterpTo(SpringArm->TargetArmLength, SetLeftRun.armLength, DeltaTime, 3.0f));
			SpringArm->SocketOffset = (FMath::VInterpTo(SpringArm->SocketOffset, SetLeftRun.Socketoffset, DeltaTime, 3.0f));
		}
		else
		{
			SpringArm->SetRelativeLocation(FMath::VInterpTo(SpringArm->GetRelativeLocation(), defaultposition.Location, DeltaTime, 3.0f));
			SpringArm->SetRelativeRotation(FMath::RInterpTo(SpringArm->GetRelativeRotation(), defaultposition.rotation, DeltaTime, 3.0f));
			SpringArm->TargetArmLength = (FMath::FInterpTo(SpringArm->TargetArmLength, defaultposition.armLength, DeltaTime, 3.0f));
			SpringArm->SocketOffset = (FMath::VInterpTo(SpringArm->SocketOffset, defaultposition.Socketoffset, DeltaTime, 3.0f));

		}


	}
}

void ACHorse::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerInputComponent->BindAxis("MoveForward", this, &ACHorse::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACHorse::OnMoveRight);
	PlayerInputComponent->BindAction("JumporEvade", EInputEvent::IE_Pressed, this, &ACHorse::OnJump);
	//������ ������ Ű
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, this, &ACHorse::OffRidingSwitch);
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACHorse::MouseLeftPressed);
}

void ACHorse::Interact()
{
	if (CanRiding == false)
		return;
	if (isRiding == true)
		return;
	if (IsDismounting == true)
		return;
	APawn* pawn = GetWorld()->GetFirstPlayerController()->GetPawn();
	ACPlayer* player = Cast<ACPlayer>(pawn);

	if(player != nullptr)
	{
		rider = player;
		player->OnRiding(this);
		FVector playerlocation = player->GetActorLocation();
		FVector horseLocation = GetActorLocation();
		
		FVector horseForwardVector = GetActorForwardVector();
		FVector toPlayerVector = playerlocation - horseLocation;
		toPlayerVector.Normalize();

		FVector crossProduct = UKismetMathLibrary::Cross_VectorVector(horseForwardVector, toPlayerVector);



		playeraniminstance = Cast<UCAnimInstance>(player->GetMesh()->GetAnimInstance());
		player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		
		FName socketName = (crossProduct.Z > 0.0f) ? "MountPoint_Rride" : "MountPoint_Lride";
		FVector socketlocation;
		FRotator socketrotation;
		GetMesh()->GetSocketWorldLocationAndRotation(socketName,socketlocation, socketrotation);
		
		socketlocation.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		player->SetActorLocation(socketlocation);
		player->SetActorRotation(this->GetActorRotation());




		if (crossProduct.Z > 0.0f)
		{
			player->PlayAnimMontage(RightRiding);
		}
		else
		{
			player->PlayAnimMontage(LeftRiding);
		}

		player->SetActorRotation(this->GetActorRotation());

		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			// ī�޶� ���� ����
			playerController->SetViewTargetWithBlend(this, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, true);
			player->SetActorRotation(this->GetActorRotation());
			// ���� �ð� �Ŀ� ������ ��ȯ
			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [playerController, this]()
				{
					playerController->Possess(this);

				}, 1.0f, false); // 1.0f�� ���� �ð��� ��ġ
		}
	}
}



void ACHorse::OnMoveForward(float InAxis)
{

	forwardAxis = InAxis*5.0f;
}

void ACHorse::OnMoveRight(float InAxis)
{

	RightAxis = InAxis;
}

void ACHorse::OnJump()
{
	if (isRiding == false)
		return;
	UCharacterMovementComponent* Movement = GetCharacterMovement();
	if (Movement->IsFalling())
		return;
	if (IsJump == true)
		return;
	if (forwardAxis < 0)
		return;
	
	Movement->AirControl = 0.1f; 
	Movement->SetMovementMode(EMovementMode::MOVE_Flying);
	playeraniminstance->SetHorseJump(true);
	animinstance->SetIsJump(true);
	IsJump = true;

	//PlayAnimMontage(JumpMontage);
	
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, [Movement, this]()
		{
			Movement->SetMovementMode(EMovementMode::MOVE_Falling);
			playeraniminstance->SetHorseJump(false);
			animinstance->SetIsJump(false);
			IsJump = false;
		}, 1.0f, false); 

	//SoundManager->PlaySound2D("SC_Player_Jump", GetWorld());
}

void ACHorse::OffRidingSwitch()
{
	if (CanRiding == true)
		return;
	if (isRiding == false)
		return;
	if (IsDismounting == true)
		return;
	FTimerHandle handle;
	if (forwardAxis < 1)
	{
		offRiding();
	}
	else
	{
		GetWorld()->GetTimerManager().SetTimer(handle, this, &ACHorse::offRiding, 0.5f, false);
	}

	IsDismounting = true;
}

void ACHorse::offRiding()
{
	if (rider != nullptr)
	{
		ACPlayer* player = Cast<ACPlayer>(rider);

		FName socketName = "MountPoint_Lride";
		FVector socketlocation;
		FRotator socketrotation;
		GetMesh()->GetSocketWorldLocationAndRotation(socketName, socketlocation, socketrotation);

		socketlocation.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		player->SetActorLocation(socketlocation);
		player->PlayAnimMontage(OffLeft);
		//player->SetActorRotation(this->GetActorRotation());

	}
}

void ACHorse::MouseLeftPressed()
{
	if (IsJump == true)
		return;
	if (IsDismounting == true)
		return;
	if (isRiding == false)
		return;
	if (CanRiding == true)
		return;
	ACPlayer* player = Cast<ACPlayer>(rider);
	player->PlayAnimMontage(AttackMontage);
}


//��Ÿ�� ȣ��
void ACHorse::ComplateRiding()
{
	if (isRiding == true)
	{
		ACPlayer* player = Cast<ACPlayer>(rider);
		rider->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);

		FName socketName = "MountPoint_Loff";
		FVector socketlocation;
		FRotator socketrotation;
		GetMesh()->GetSocketWorldLocationAndRotation(socketName, socketlocation, socketrotation);
		socketlocation.Z += player->GetCapsuleComponent()->GetScaledCapsuleHalfHeight();
		player->SetActorLocation(socketlocation);
		player->SetActorRotation(socketrotation);

		player->GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
		playeraniminstance->IsRiding(false);

		APlayerController* playerController = GetWorld()->GetFirstPlayerController();
		if (playerController)
		{
			playerController->SetViewTargetWithBlend(player, 1.0f, EViewTargetBlendFunction::VTBlend_Cubic, 0.0f, true);

			FTimerHandle TimerHandle;
			GetWorld()->GetTimerManager().SetTimer(TimerHandle, [playerController,player, this]()
				{
					playerController->Possess(rider);
					player->OffRiding();
					isRiding = false;
					IsDismounting = false;
					rider = nullptr;
				}, 1.0f, false);
		}


		return;
	}
	isRiding = true;
	rider->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, "MountPoint");
	rider->SetActorRotation(this->GetActorRotation());
	GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
}


