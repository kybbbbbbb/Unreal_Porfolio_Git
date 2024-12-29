#include "Characters/CPlayer.h"
#include <Camera/CameraComponent.h>
#include <GameFramework/SpringArmComponent.h>
#include "Components/CMovementComponent.h"
#include "Characters/CAnimInstance.h"
#include "Components/CStateComponent.h"
#include "Components/CTargetComponent.h"
#include "Components/CInteractiveComponent.h"
#include "GameFramework/PlayerInput.h"
#include "../GameManager/CUIManager_Game.h"
#include "../Components/CHealthComponent.h"
#include "../Weapons/CWeaponStructure.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Weapons/CDoAction.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Item/InventorySystem.h"
#include "../GameManager/CCameraManagerComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "../Components/CFootIKComponent.h"
#include "CHorse.h"
#include "../GameManager/CSoundManager.h"
#include "../Components/CNaviPathSystem.h"

ACPlayer::ACPlayer()
{
	SpringArm = CreateDefaultSubobject<USpringArmComponent>("SpringArm");
	Camera = CreateDefaultSubobject<UCameraComponent>("CameraComponent");
	MovementComponent = CreateDefaultSubobject<UCMovementComponent>("MovementComponent");

	WeaponComponent = CreateDefaultSubobject<UCWeaponComponent_reset>("WeaponComponent_Reset");
	StateComponent = CreateDefaultSubobject<UCStateComponent>("StateComponent");
	TargetComponent = CreateDefaultSubobject<UCTargetComponent>("Targeting");
	InteractiveComponent = CreateDefaultSubobject<UCInteractiveComponent>("InteractiveComponent");
	HealthComponent = CreateDefaultSubobject<UCHealthComponent>("HealthComponent");

	Inventory2 = CreateDefaultSubobject<UInventorySystem>("InventorySystem");
	FootIKComponent = CreateDefaultSubobject<UCFootIKComponent>("FootIKComponent");

	CameraManagerComponent = CreateDefaultSubobject<UCCameraManagerComponent>("CameraManagerComponent");
	CNaviPathSystem = CreateDefaultSubobject<UCNaviPathSystem>("CNaviPathSystem");

	SpringArm->SetupAttachment(GetMesh());
	Camera->SetupAttachment(SpringArm);


	USkeletalMesh* mesh;
	mesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(L"/Script/Engine.SkeletalMesh'/Game/Characters/Mannequins/Meshes/SKM_Quinn_Simple.SKM_Quinn_Simple'").Object;
	GetMesh()->SetSkeletalMesh(mesh);
	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));
	
	SpringArm->SetRelativeRotation(FRotator(0, 90, 0));
	SpringArm->SetRelativeLocation(FVector(0, 0, 140));
	SpringArm->TargetArmLength = 300;
	SpringArm->bDoCollisionTest = false;
	SpringArm->bUsePawnControlRotation = true;
	SpringArm->bEnableCameraLag = true;


	GetMesh()->SetAnimInstanceClass(ConstructorHelpers::FClassFinder<UCAnimInstance>(L"/Script/Engine.AnimBlueprint'/Game/Characters/01_Player/Animation/Sword/ABP_CAniminstance1.ABP_CAniminstance1_C'").Class);


	StateComponent->OnStateTypeChanged.AddDynamic(this, &ACPlayer::OnStateTypeChange);
	PrimaryActorTick.bCanEverTick = true;


}

void ACPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	if (UIManager != nullptr)
	{
		UIManager->Tick(DeltaTime);
	}

	if (bOnDash == true)
	{
		if (GetCharacterMovement()->IsFalling() == false)
		{
			StopAirDash();
			return;
		}
		float DistanceTravelled = FVector::Dist(DashStartLocation, GetActorLocation());
		if (DistanceTravelled >= DesiredDashDistance)
		{
			StopAirDash_Late();
		}

	}

}

bool ACPlayer::RequestAttackToken()
{
	if (CurrentAttackToken < MaxAttackToken)
	{
		CurrentAttackToken++;
		
		return true;
	}
	return false;
}

void ACPlayer::ReleaseAttackToken()
{
	if (CurrentAttackToken > 0)
	{
		CurrentAttackToken--;
	}
}



void ACPlayer::BeginPlay()
{
	Super::BeginPlay();

	MovementComponent->OnRun();
	MovementComponent->DisableControlRotation();
	UIManager = UCUIManager_Game::GetInstance(GetWorld());
	UCSoundManager* soundManager = UCSoundManager::GetInstance();
	SoundManager = soundManager;
	//SoundManager->PlayBGM("BGM_Full", GetWorld());
	Inventory2->Beginplay();

	GetCapsuleComponent()->OnComponentHit.AddDynamic(this, &ACPlayer::OnCharacterHit);
}



float ACPlayer::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	if (StateComponent->IsEvadeMode())
	{
		return Damage;
	}
	if (StateComponent->IsParringMode())
	{
		WeaponComponent->SubAction_Parrying(EventInstigator);
		
		return Damage;
	}

	DamageData.Power = Damage;

	if (EventInstigator != nullptr)
	{
		// EventInstigator의 Pawn을 가져옵니다.
		APawn* pawn = EventInstigator->GetPawn();
		if (pawn != nullptr)
		{
			// Pawn이 ACharacter로 캐스팅 가능한지 확인
			ACharacter* aCharacter = Cast<ACharacter>(pawn);
			if (aCharacter != nullptr)
			{
				DamageData.Attacker = aCharacter; // 공격자 저장
			}
		}
	}
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;

	
	//데미지를 받아 데미지모드로 전환
	//연결된 함수인 몬스터클래스의 OnStateTypeChange가 호출된다.
	StateComponent->SetDamagedMode();


	return Damage;
}

void ACPlayer::OnStateTypeChange(EStateType PrevType, EStateType NewType)
{
	switch (NewType)
	{
	case EStateType::Idle:
		break;
	case EStateType::Evade:
		//ignoreDamage();
		break;
	case EStateType::Equip:
		break;
	case EStateType::Damaged:
		Damage();
		break;
	case EStateType::Action:
		break;
	case EStateType::Dead:
		Dead();
		break;
	case EStateType::Max:
		break;
	default:
		break;
	}

}







void ACPlayer::Damage()
{

	if (HealthComponent->IsDead())
		return;

	CameraManagerComponent->OnDamage();
	HealthComponent->Damage(DamageData.Power);
	MovementComponent->Stop();
	DamageData.Power = 0;



	//몽타주 재생(스킬을 맞으면 달라지게)
	//PlayAnimMontage()

	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;


		if (hitData->ParticleEffect != nullptr)
		{
			if (DamageData.Event->SweepResult->ImpactPoint == FVector::Zero())
				hitData->PlayEffect(GetWorld(), GetActorLocation(), DamageData.Event->SweepResult->ImpactNormal.Rotation());

			hitData->PlayEffect(GetWorld(), DamageData.Event->SweepResult->ImpactPoint, DamageData.Event->SweepResult->ImpactNormal.Rotation());
		}
		hitData->PlaySountWave(this);

		if (HealthComponent->IsDead() == false)
		{

			FVector start = GetActorLocation();
			start.Z = 0.0f;
			FVector target = DamageData.Attacker->GetActorLocation();
			target.Z = 0.0f;
			FVector direction = target - start;
			direction.Normalize();
			//LaunchCharacter(-direction * (hitData->Launch), false, false);

			FVector launchVelocity = -direction * hitData->Launch;
			launchVelocity.Z = 0.0f; // 수직 방향의 힘 제거

			LaunchCharacter(launchVelocity, false, false);

			UAnimMontage* montage = hitData->HitMontage;
			if (montage == nullptr)
			{
				montage = StandardDamageMontage;
			}
			
			PlayAnimMontage(montage, 1.0f, "start");
			SoundManager->PlaySound2D("SC_Player_Hit", GetWorld());

			

			SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		}
		
		hitData->PlayHitStop(GetWorld());
	}

	if (HealthComponent->IsDead())
	{
		StateComponent->SetDeadMode();
		SoundManager->PlaySound2D("SC_Player_Dead", GetWorld());
		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;
}

void ACPlayer::Dead()
{
	PlayAnimMontage(DeadMontage);

}

void ACPlayer::SpawnHorse()
{
	if (OwnHorse == nullptr)
	{
		// 스폰할 말의 클래스가 설정되어 있는지 확인
		if (HorseClass)
		{
			// 월드 컨텍스트 가져오기
			UWorld* World = GetWorld();
			if (World)
			{
				// 플레이어 위치 근처에 말 스폰
				FVector ForwardVector = GetActorForwardVector(); // 캐릭터 전방 벡터
				FVector rightVector = GetActorRightVector();
				FVector SpawnLocation = GetActorLocation() - (ForwardVector * 500.0f) + FVector(0.0f, 0.0f, 50.0f) + rightVector*150; // 캐릭터 뒤쪽으로 스폰
				FRotator SpawnRotation = GetActorRotation();

				FHitResult HitResult;
				FVector TraceStart = SpawnLocation + FVector(0.0f, 0.0f, 500.0f); // 위에서 시작
				FVector TraceEnd = SpawnLocation - FVector(0.0f, 0.0f, 500.0f);   // 아래로 끝

				FCollisionQueryParams QueryParams;
				QueryParams.AddIgnoredActor(this); 

				if (World->LineTraceSingleByChannel(HitResult, TraceStart, TraceEnd, ECC_Visibility, QueryParams))
				{
					SpawnLocation = HitResult.ImpactPoint + FVector(0.0f, 0.0f, 50.0f); // 약간 위로 올려 스폰
				}

				FActorSpawnParameters SpawnParams;
				SpawnParams.Owner = this;
				SpawnParams.Instigator = GetInstigator();

				OwnHorse = World->SpawnActor<ACHorse>(HorseClass, SpawnLocation, SpawnRotation, SpawnParams);
			}
		}
	}
	else
	{
		OwnHorse->Destroy();
		OwnHorse = nullptr;

	}

}

void ACPlayer::End_Damaged()
{
	StateComponent->SetIdleMode();
	if(WeaponComponent->GetDoAction() != nullptr)
		WeaponComponent->GetDoAction()->End_DoAction();
	MovementComponent->Move();
}

void ACPlayer::SetWeaponUnVisible()
{
	WeaponComponent->SetWeaponUnVisible();

}

void ACPlayer::SetWeaponOnVisible()
{
	WeaponComponent->SetWeaponOnVisible();

}



void ACPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);


	PlayerInputComponent->BindAxis("MoveForward", this, &ACPlayer::OnMoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ACPlayer::OnMoveRight);
	PlayerInputComponent->BindAxis("VerticalLook", MovementComponent, &UCMovementComponent::OnVerticalLook);
	PlayerInputComponent->BindAxis("HorizontalLook", MovementComponent, &UCMovementComponent::OnHorizontalLook);
	
	
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Pressed, MovementComponent, &UCMovementComponent::OnSprint);
	PlayerInputComponent->BindAction("Sprint", EInputEvent::IE_Released, MovementComponent, &UCMovementComponent::OnRun);
	
	PlayerInputComponent->BindAction("Sword", EInputEvent::IE_Pressed, WeaponComponent, &UCWeaponComponent_reset::SetSwordMode);
	PlayerInputComponent->BindAction("Fist", EInputEvent::IE_Pressed, WeaponComponent, &UCWeaponComponent_reset::SetFistMode);
	PlayerInputComponent->BindAction("Bow", EInputEvent::IE_Pressed, WeaponComponent, &UCWeaponComponent_reset::SetBowMode);
	
	PlayerInputComponent->BindAction("Action", EInputEvent::IE_Pressed, this, &ACPlayer::MouseLeftPressed);
	
	PlayerInputComponent->BindAction("W", EInputEvent::IE_Pressed, this, &ACPlayer::PressW);
	PlayerInputComponent->BindAction("W", EInputEvent::IE_Released, this, &ACPlayer::ReleaseW);
	PlayerInputComponent->BindAction("S", EInputEvent::IE_Pressed, this, &ACPlayer::PressS);
	PlayerInputComponent->BindAction("D", EInputEvent::IE_Pressed, this, &ACPlayer::PressD);
	PlayerInputComponent->BindAction("A", EInputEvent::IE_Pressed, this, &ACPlayer::PressA);
	PlayerInputComponent->BindAction("NaviTest", EInputEvent::IE_Pressed, CNaviPathSystem, &UCNaviPathSystem::NaviOn);

	PlayerInputComponent->BindAction("DashAttack_shift", EInputEvent::IE_Pressed, this, &ACPlayer::DashAttack_shift_On);
	PlayerInputComponent->BindAction("DashAttack_shift", EInputEvent::IE_Released, this, &ACPlayer::DashAttack_shift_Off);

	PlayerInputComponent->BindAction("JumporEvade", EInputEvent::IE_Pressed, this, &ACPlayer::OnJump);
	PlayerInputComponent->BindAction("Evade", EInputEvent::IE_Pressed, this, &ACPlayer::OnEvade);

	PlayerInputComponent->BindAction("Targeting", EInputEvent::IE_Pressed, TargetComponent, &UCTargetComponent::Toggle);
	PlayerInputComponent->BindAction("Interaction", EInputEvent::IE_Pressed, InteractiveComponent, &UCInteractiveComponent::CheckInteractive);
	
	PlayerInputComponent->BindAction("SubAction_Q", EInputEvent::IE_Pressed, this, &ACPlayer::SubActionQ_Pressed);
	PlayerInputComponent->BindAction("SubAction_Q", EInputEvent::IE_Released, this, &ACPlayer::SubActionQ_Released);
	
	PlayerInputComponent->BindAction("SubAction_E", EInputEvent::IE_Pressed, this, &ACPlayer::SubActionE_Pressed);
	PlayerInputComponent->BindAction("SubAction_E", EInputEvent::IE_Released, this, &ACPlayer::SubActionE_Released);

	PlayerInputComponent->BindAction("SubAction_MR", EInputEvent::IE_Pressed, this, &ACPlayer::SubActionMR_Pressed);
	PlayerInputComponent->BindAction("SubAction_MR", EInputEvent::IE_Released, this, &ACPlayer::SubActionMR_Released);

	PlayerInputComponent->BindAction("Inventory", EInputEvent::IE_Pressed, this, &ACPlayer::OnInventory);
	PlayerInputComponent->BindAction("Horse", EInputEvent::IE_Pressed, this, &ACPlayer::SpawnHorse);
}



void ACPlayer::MouseLeftPressed()
{
	if (StateComponent->IsEvadeMode())
		return;
	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsDamagedMode())
		return;
	if (WeaponComponent->GetType() == EWeaponType::Max)
		return;

	UCharacterMovementComponent* Movement = GetCharacterMovement();

	//TODO: 추후 공중공격 두액션에 넣어야함(공격중 상태로 변경해야하기 때문)
	if (Movement->IsFalling() && bOnDash == false)
	{
		if (bInDash_shift && bPressW && StateComponent->IsActionMode() == false)
		{
			MovementComponent->Stop();
			FRotator ControlRotation = GetControlRotation();
			ControlRotation.Pitch = 0.0f;
			ControlRotation.Roll = 0.0f;
			SetActorRotation(ControlRotation);
			GetCharacterMovement()->GravityScale = 0.0f;
			DashStartLocation = GetActorLocation();

			//공중 대쉬 어택
			if (WeaponComponent->GetType() == EWeaponType::Sword)
			{
				CameraManagerComponent->DefaultCameraChange(ECameraEnum::DefaultShort_AirAttack);
				
				GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn_NoPawnBlock"));

				PlayAnimMontage(SwordAirDashMontage);
				
				LaunchCharacter(GetControlRotation().Vector()*3000,false,false);
				
				StateComponent->SetActionMode();
			}
			else if (WeaponComponent->GetType() == EWeaponType::Fist)
			{
				PlayAnimMontage(FistAirDashMontage);

				LaunchCharacter(GetControlRotation().Vector() * 3000, false, false);

				StateComponent->SetActionMode();

			}

			bOnDash = true;
			return;
		}

		if(bInDash_shift == false)
		{
			SetActorRotation(GetControlRotation());
			WeaponComponent->DoAction();
		}
		return;
	}


	WeaponComponent->DoAction();


}

void ACPlayer::OnCharacterHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,UPrimitiveComponent* OtherComp, FVector NormalImpulse,const FHitResult& Hit)
{
	if (bOnDash)
	{
		StopAirDash();
	}
}

void ACPlayer::SubActionQ_Pressed()
{

	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsEvadeMode())
		return;
	
	TArray<UCSubAction_RightMouse*>* subActions = WeaponComponent->GetSubAction();
	if (subActions == nullptr)
		return;
	if (subActions->Num() < 1)
		return;

	for (UCSubAction_RightMouse* subAction : *subActions)
	{
		if (subAction->GetInSubAction_KeyNumber() == EKeys::Q)
			continue;

		if (subAction->GetInSubAction_RightMouse() == true)
			return;
	}

	
	WeaponComponent->SubAction_Pressed(EKeys::Q);
}

void ACPlayer::SubActionQ_Released()
{

}






void ACPlayer::SubActionMR_Pressed()
{

	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsSubActionMode())
		return;
	if (StateComponent->IsEvadeMode())
		return;


	TArray<UCSubAction_RightMouse*>* subActions = WeaponComponent->GetSubAction();
	if (subActions == nullptr)
		return;
	if (subActions->Num() < 1)
		return;

	for (UCSubAction_RightMouse* subAction : *subActions)
	{
		if (subAction->GetInSubAction_KeyNumber() == EKeys::RightMouseButton)
			continue;

		if (subAction->GetInSubAction_RightMouse() == true)
			return;
	}

	isOnSubAction = true;
	WeaponComponent->SubAction_Pressed(EKeys::RightMouseButton);
}

void ACPlayer::SubActionMR_Released()
{
	if (isOnSubAction == true && StateComponent->IsSubActionMode())
	{

		WeaponComponent->SubAction_Released(EKeys::RightMouseButton);
		isOnSubAction = false;
	}
}

void ACPlayer::SubActionE_Pressed()
{
	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsEvadeMode())
		return;

	TArray<UCSubAction_RightMouse*>* subActions = WeaponComponent->GetSubAction();
	if (subActions == nullptr)
		return;
	if (subActions->Num() < 1)
		return;

	for (UCSubAction_RightMouse* subAction : *subActions)
	{
		if (subAction->GetInSubAction_KeyNumber() == EKeys::E)
			continue;

		if (subAction->GetInSubAction_RightMouse() == true)
			return;
	}


	WeaponComponent->SubAction_Pressed(EKeys::E);
}

void ACPlayer::SubActionE_Released()
{
}

void ACPlayer::OnInventory()
{
	if(UIManager == nullptr)
		UIManager = UCUIManager_Game::GetInstance(GetWorld());

	UIManager->ShowItemUI();
}





void ACPlayer::OnMoveForward(float InAxis)
{

	MovementComponent->OnMoveForward(InAxis);
}

void ACPlayer::OnMoveRight(float InAxis)
{

	MovementComponent->OnMoveRight(InAxis);
}

void ACPlayer::OnJump()
{
	if (StateComponent->IsEvadeMode())
		return;
	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsSubActionMode())
		return;
	if (StateComponent->IsDamagedMode())
		return;

	if (StateComponent->IsIdleMode() && WeaponComponent->GetType() != EWeaponType::Bow)
	{

		UCharacterMovementComponent* Movement = GetCharacterMovement();
		if (Movement->IsFalling())
			return;

		Movement->AirControl = 1.0f;              // 공중 조작성 최대
	
		Jump();
		SoundManager->PlaySound2D("SC_Player_Jump",GetWorld());

		

		return;
	}


	


	//float ForwardVector = GetInputAxisValue("MoveForward");
	//float RightVector = GetInputAxisValue("MoveRight");

	//FVector InputVector = FVector(ForwardVector, RightVector, 0.0f);
	//if (InputVector.Size() > 0.0f)
	//{
	//	// 카메라의 회전 각도 기준으로 입력 벡터 변환
	//	FRotator CameraRotation = GetControlRotation();
	//	FRotator YawRotation(0, CameraRotation.Yaw, 0);

	//	FVector Direction = FRotationMatrix(YawRotation).TransformVector(InputVector);

	//	if (bInDash_w == true && bInDash_shift == true)
	//	{
	//		SoundManager->PlaySound2D("SC_Player_Jump", GetWorld());

	//		PlayAnimMontage(SwordDashMontage);
	//		LaunchCharacter(Direction*500, true, true);
	//		return;
	//	}

	//	Evade(Direction.GetSafeNormal());

	//}
}

void ACPlayer::OnEvade()
{
	if (StateComponent->IsEvadeMode())
		return;
	if (StateComponent->IsDeadMode())
		return;
	if (StateComponent->IsEquipMode())
		return;
	if (StateComponent->IsSubActionMode())
		return;
	if (StateComponent->IsDamagedMode())
		return;
	if (WeaponComponent->GetType() == EWeaponType::Max)
		return;

	if (GetCharacterMovement()->IsFalling())
		return;

	float ForwardVector = GetInputAxisValue("MoveForward");
	float RightVector = GetInputAxisValue("MoveRight");

	FVector InputVector = FVector(ForwardVector, RightVector, 0.0f);

	if (InputVector.Size() > 0.0f)
	{
		FRotator CameraRotation = GetControlRotation();
		FRotator YawRotation(0, CameraRotation.Yaw, 0);

		FVector Direction = FRotationMatrix(YawRotation).TransformVector(InputVector);

		Evade(Direction.GetSafeNormal());
	}
}



void ACPlayer::Evade(FVector direction)
{
	// 카메라의 정면 벡터를 기준으로 방향을 계산
	if (HealthComponent->GetCurrentMp() <= 30.0f)
		return;

	FRotator CameraRotation = GetControlRotation();

	FVector CameraForward = CameraRotation.Vector().GetSafeNormal2D();
	FVector CameraRight = FRotationMatrix(CameraRotation).GetUnitAxis(EAxis::Y).GetSafeNormal2D();

	float ForwardDot = FVector::DotProduct(CameraForward, direction);
	float RightDot = FVector::DotProduct(CameraRight, direction);

	CameraRotation.Pitch = 0.0f;
	CameraRotation.Roll = 0.0f;
	SetActorRotation(CameraRotation);

	SoundManager->PlaySound2D("SC_Player_Jump", GetWorld());

	MovementComponent->Stop();
	StateComponent->SetEvadeMode();

	if (ForwardDot > 0.7f)
	{
		// 정면 회피
		if (bInDash_shift == true)
		{

			PlayAnimMontage(SwordDashMontage);
			LaunchCharacter(direction * 500, true, true);
			return;
		}

		PlayAnimMontage(Evade_Front);
	}
	else if (ForwardDot < -0.7f)
	{
		// 후면 회피
		PlayAnimMontage(Evade_Back);
	}
	else if (RightDot > 0.7f)
	{
		// 우측 회피
		PlayAnimMontage(Evade_Right, 1.1f);
	}
	else if (RightDot < -0.7f)
	{
		// 좌측 회피
		PlayAnimMontage(Evade_Left, 1.1f);
	}


	
	
	CameraManagerComponent->OnEvade();
	HealthComponent->UsedMP(20.0f);
}

void ACPlayer::PressDirection(FVector Direction)
{
	if (bIsEvadePress && LastInputDirection.Equals(Direction, KINDA_SMALL_NUMBER))
	{
		OnEvade();
		bIsEvadePress = false;
	}
	else
	{
		bIsEvadePress = true;
		LastInputDirection = Direction;
		GetWorld()->GetTimerManager().SetTimer(DoubleTapTimerHandle, this, &ACPlayer::ResetEvadePress, 0.2f, false);
	}
}

void ACPlayer::PressW()
{
	PressDirection(FVector(1, 0, 0));
	bPressW = true;
}

void ACPlayer::ReleaseW()
{
	bPressW = false;
}

void ACPlayer::PressS()
{
	PressDirection(FVector(-1, 0, 0));

}

void ACPlayer::PressD()
{
	PressDirection(FVector(0, 1, 0));
}

void ACPlayer::PressA()
{
	PressDirection(FVector(0, -1, 0));
}


void ACPlayer::ResetEvadePress()
{
	bIsEvadePress = false;
	LastInputDirection = FVector::ZeroVector;
}


void ACPlayer::DashAttack_shift_On()
{
	bInDash_shift = true;
}

void ACPlayer::DashAttack_shift_Off()
{
	bInDash_shift = false;
}

void ACPlayer::StopAirDash()
{
	CameraManagerComponent->DefaultCameraChange(ECameraEnum::Default);
	// 캐릭터 멈춤
	GetCharacterMovement()->StopMovementImmediately();

	// 중력 다시 활성화
	GetCharacterMovement()->GravityScale = 2.0f;

	// 대쉬 상태 해제
	bOnDash = false;


}

void ACPlayer::StopAirDash_Late()
{
	CameraManagerComponent->DefaultCameraChange(ECameraEnum::Default);
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->GravityScale = 0.0f;
	bOnDash = false;
	
	
	if (GetWorld()->GetTimerManager().IsTimerActive(GravityEnableTimerHandle))
	{
		GetWorld()->GetTimerManager().ClearTimer(GravityEnableTimerHandle);
	}

	float GravityDelay = 0.5f;
	GetWorld()->GetTimerManager().SetTimer(GravityEnableTimerHandle, [this]()
		{
			GetCharacterMovement()->GravityScale = 2.0f;
		}, GravityDelay, false);
}

void ACPlayer::Landed(const FHitResult& Hit)
{
	Super::Landed(Hit);

	// 캐릭터를 똑바로 세우기 위해 피치와 롤을 0으로 설정
	FRotator CurrentRotation = GetActorRotation();
	CurrentRotation.Pitch = 0.0f;
	CurrentRotation.Roll = 0.0f;
	SetActorRotation(CurrentRotation);
	GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));

}

void ACPlayer::OnRiding(ACHorse* Inhorse)
{
	IsRiding = true;
	EWeaponType type = WeaponComponent->GetType();
	if(type != EWeaponType::Max)
		WeaponComponent->SetMode(type);

	currenthorse = Inhorse;
	FootIKComponent->IsRiding(IsRiding);
	MovementComponent->Stop();
	MovementComponent->SetFixedCamera(false);
	CameraManagerComponent->SetbCameraTick(false);
	UCAnimInstance* anim = Cast<UCAnimInstance>(GetMesh()->GetAnimInstance());
	anim->IsRiding(IsRiding);
}

void ACPlayer::OffRiding()
{
	IsRiding = false;
	currenthorse = nullptr;
	FootIKComponent->IsRiding(IsRiding);
	MovementComponent->Move();
	CameraManagerComponent->SetbCameraTick(true);

	MovementComponent->SetFixedCamera(false);
	
	UCAnimInstance* anim = Cast<UCAnimInstance>(GetMesh()->GetAnimInstance());
	anim->IsRiding(IsRiding);
}










