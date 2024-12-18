#include "GameManager/CCameraManagerComponent.h"
#include "GameFramework/Character.h"
#include "Camera/CameraComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Components/CMovementComponent.h"

UCCameraManagerComponent::UCCameraManagerComponent()
{
	PrimaryComponentTick.bCanEverTick = true;

}


void UCCameraManagerComponent::BeginPlay()
{
	Super::BeginPlay();

	// 레벨에서 포스트 프로세스 볼륨 찾기
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), APostProcessVolume::StaticClass(), FoundActors);
	if (FoundActors.Num() > 0)
	{
		PostProcessVolume = Cast<APostProcessVolume>(FoundActors[0]);
	}


	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter)
	{
		// 컴포넌트 찾기
		MovementComponent = OwnerCharacter->FindComponentByClass<UCMovementComponent>();
		SpringArmComponent = OwnerCharacter->FindComponentByClass<USpringArmComponent>();
		CameraComponent = OwnerCharacter->FindComponentByClass<UCameraComponent>();
	}

	if (CameraPresets.Num() == 0)
	{
		// 기본 상태
		FCameratransform DefaultPreset;
		DefaultPreset.ArmLength = 200.0f;
		DefaultPreset.FOV = 90.0f;
		DefaultPreset.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		DefaultPreset.TransitionSpeed = 5.0;
		DefaultPreset.ReturnTransitionSpeed = 5.0f;
		DefaultPreset.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		CameraPresets.Add(ECameraEnum::Default, DefaultPreset);

		// 패링 상태
		FCameratransform ParryingPreset;
		ParryingPreset.ArmLength = 120.0f;
		ParryingPreset.SocketOffset = FVector(0.0f, 100.0f, 0.0f); // 오른쪽으로 이동
		ParryingPreset.FOV = 110.0f;
		ParryingPreset.RelativeRotation = FRotator(0.0f, -35.0f, 10.0f); // 플레이어 오른쪽에서 비스듬히
		ParryingPreset.TransitionSpeed = 5.0;
		DefaultPreset.ReturnTransitionSpeed = 5.0f;
		CameraPresets.Add(ECameraEnum::Parrying, ParryingPreset);

		// 피격 상태
		FCameratransform DamagedPreset;
		DamagedPreset.ArmLength = 220.0f;
		DamagedPreset.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		DamagedPreset.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DamagedPreset.FOV = 95.0f;
		DamagedPreset.TransitionSpeed = 30.0f;
		DefaultPreset.ReturnTransitionSpeed = 5.0f;
		CameraPresets.Add(ECameraEnum::Damaged, DamagedPreset);

		// 스턴 상태
		FCameratransform StunnedPreset;
		StunnedPreset.ArmLength = 500.0f;
		StunnedPreset.SocketOffset = FVector(0.0f, 0.0f, 200.0f);
		StunnedPreset.FOV = 110.0f;
		StunnedPreset.RelativeRotation = FRotator(-20.0f, 0.0f, 0.0f);
		StunnedPreset.TransitionSpeed = 8.0f;
		CameraPresets.Add(ECameraEnum::Stunned, StunnedPreset);

		// Evade 상태
		FCameratransform EvadePreset;
		EvadePreset.ArmLength = 400.0f;
		EvadePreset.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		EvadePreset.FOV = 100.0f;
		EvadePreset.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		EvadePreset.TransitionSpeed = 5.5f;
		EvadePreset.ReturnTransitionSpeed = 5.5f;
		CameraPresets.Add(ECameraEnum::Evade, EvadePreset);

		// Fist_Attack 상태
		FCameratransform Attack_Fist;
		Attack_Fist.ArmLength = 100.0f;
		Attack_Fist.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		Attack_Fist.FOV = 110.0f;
		Attack_Fist.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		Attack_Fist.TransitionSpeed = 20.0f;
		Attack_Fist.ReturnTransitionSpeed = 2.0f;
		CameraPresets.Add(ECameraEnum::Attack_Fist, Attack_Fist);

		// Sword_Attack 상태
		FCameratransform Attack_Sword;
		Attack_Sword.ArmLength = 130.0f;
		Attack_Sword.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		Attack_Sword.FOV = 105.0f;
		Attack_Sword.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		Attack_Sword.TransitionSpeed = 20.0f;
		Attack_Sword.ReturnTransitionSpeed = 2.0f;
		CameraPresets.Add(ECameraEnum::Attack_Sword, Attack_Sword);


		//디폴트 카메라 설정
		FCameratransform DefaultStandard;
		DefaultStandard = CameraPresets[ECameraEnum::Default];
		CameraPresets.Add(ECameraEnum::DefaultStandard, DefaultStandard);

		FCameratransform DefaultStandardLong;
		DefaultStandardLong.ArmLength = 400.0f;
		DefaultStandardLong.SocketOffset = FVector(0.0f, 30.0f, 0.0f);
		DefaultStandardLong.FOV = 90.0f;
		DefaultStandardLong.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultStandardLong.TransitionSpeed = 10.0f;
		DefaultStandardLong.ReturnTransitionSpeed = 10.0f;
		DefaultStandardLong.bfixedCamera = true;
		CameraPresets.Add(ECameraEnum::DefaultStandardLong, DefaultStandardLong);

		FCameratransform DefaultStandardMiddleLong;
		DefaultStandardMiddleLong.ArmLength = 350.0f;
		DefaultStandardMiddleLong.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		DefaultStandardMiddleLong.FOV = 90.0f;
		DefaultStandardMiddleLong.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultStandardMiddleLong.TransitionSpeed = 10.0f;
		DefaultStandardMiddleLong.ReturnTransitionSpeed = 10.0f;
		DefaultStandardMiddleLong.bfixedCamera = true;
		CameraPresets.Add(ECameraEnum::DefaultStandardMiddleLong_soketoffset60, DefaultStandardMiddleLong);

		FCameratransform DefaultLong;
		DefaultLong.ArmLength = 2000.0f;
		DefaultLong.SocketOffset = FVector(0.0f, 0.0f, 0.0f);
		DefaultLong.FOV = 90.0f;
		DefaultLong.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultLong.TransitionSpeed = 1.0f;
		DefaultLong.ReturnTransitionSpeed = 1.0f;
		DefaultLong.bfixedCamera = true;
		CameraPresets.Add(ECameraEnum::DefaultLong, DefaultLong);

		FCameratransform DefaultShort;
		DefaultShort.ArmLength = 100.0f;
		DefaultShort.SocketOffset = FVector(0.0f, 60.0f, -60.0f);
		DefaultShort.FOV = 110.0f;
		DefaultShort.RelativeRotation = FRotator(0.0f, 45.0f, 0.0f);
		DefaultShort.TransitionSpeed = 2.0f;
		DefaultShort.ReturnTransitionSpeed = 5.0f;
		CameraPresets.Add(ECameraEnum::DefaultShort, DefaultShort);

		FCameratransform DefaultShort_soketoffset6;
		DefaultShort_soketoffset6.ArmLength = 50.0f;
		DefaultShort_soketoffset6.SocketOffset = FVector(0.0f, 60.0f, 0.0f);
		DefaultShort_soketoffset6.FOV = 95.0f;
		DefaultShort_soketoffset6.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultShort_soketoffset6.TransitionSpeed = 10.0f;
		DefaultShort_soketoffset6.ReturnTransitionSpeed = 10.0f;
		CameraPresets.Add(ECameraEnum::DefaultShort_soketoffset6, DefaultShort_soketoffset6);

		FCameratransform DefaultShort_AirAttack;
		DefaultShort_AirAttack.ArmLength = 30.0f;
		DefaultShort_AirAttack.SocketOffset = FVector(0.0f, 60.0f, -15.0f);
		DefaultShort_AirAttack.FOV = 100;
		DefaultShort_AirAttack.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultShort_AirAttack.TransitionSpeed = 25.0f;
		DefaultShort_AirAttack.ReturnTransitionSpeed = 10.0f;
		CameraPresets.Add(ECameraEnum::DefaultShort_AirAttack, DefaultShort_AirAttack);


		FCameratransform DefaultUpDownFix;
		DefaultUpDownFix.ArmLength = 1000.0f;
		DefaultUpDownFix.SocketOffset = FVector(0.0f, 0.0f, 0.0f);
		DefaultUpDownFix.FOV = 90.0f;
		DefaultUpDownFix.RelativeRotation = FRotator(0.0f, 0.0f, 0.0f);
		DefaultUpDownFix.TransitionSpeed = 5.0f;
		DefaultUpDownFix.ReturnTransitionSpeed = 5.0f;
		CameraPresets.Add(ECameraEnum::DefaultUpDownFix, DefaultUpDownFix);
	}



}


void UCCameraManagerComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (StopCameraAction == true)
		return;

	if(PostProcessVolume == nullptr)
		//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "volume noexist");

	if (SpringArmComponent && CameraComponent)
	{
		const FCameratransform& CurrentPreset = CameraPresets[CurrentCameraState];

		if(CurrentCameraState != ECameraEnum::Default)
			CameraPresets[ECameraEnum::Default].TransitionSpeed = CurrentPreset.ReturnTransitionSpeed;

		FRotator PlayerRotation = OwnerCharacter->GetActorRotation();

		// 부드러운 전환
		SpringArmComponent->TargetArmLength = FMath::FInterpTo(
			SpringArmComponent->TargetArmLength,
			CurrentPreset.ArmLength,
			DeltaTime,
			CurrentPreset.TransitionSpeed
		);

		SpringArmComponent->SocketOffset = FMath::VInterpTo(
			SpringArmComponent->SocketOffset,
			CurrentPreset.SocketOffset,
			DeltaTime,
			CurrentPreset.TransitionSpeed
		);

		FRotator TargetRotation = CurrentPreset.bfixedCamera ?
			CurrentPreset.RelativeRotation :
			CurrentPreset.RelativeRotation + PlayerRotation;

		SpringArmComponent->SetRelativeRotation(FMath::RInterpTo(
			SpringArmComponent->GetRelativeRotation(),
			TargetRotation,
			DeltaTime,
			CurrentPreset.TransitionSpeed
		));

		

		CameraComponent->SetFieldOfView(FMath::FInterpTo(
			CameraComponent->FieldOfView,
			CurrentPreset.FOV,
			DeltaTime,
			CurrentPreset.TransitionSpeed
		));


	}


}







void UCCameraManagerComponent::OnParry()
{
	if (bIsCameraAction == true)
		return;
	
	
	GetWorld()->GetTimerManager().ClearTimer(RestoreTimeHandler);
	bIsCameraAction = true;
	SetCameraState(ECameraEnum::Parrying, 0.2f);
}

void UCCameraManagerComponent::OnDamage()
{
	if (bIsCameraAction == true)
		return;
	
	GetWorld()->GetTimerManager().ClearTimer(RestoreTimeHandler);
	bIsCameraAction = true;
	SetCameraState(ECameraEnum::Damaged, 0.1f);
}

void UCCameraManagerComponent::OnEvade()
{
	if (bIsCameraAction == true)
		return;

	GetWorld()->GetTimerManager().ClearTimer(RestoreTimeHandler);
	bIsCameraAction = true;
	SetCameraState(ECameraEnum::Evade, 0.4f);

}

void UCCameraManagerComponent::OnFistAttack()
{
	if (bIsCameraAction == true)
		return;
	if (StopAttackAction == true)
		return;

	GetWorld()->GetTimerManager().ClearTimer(RestoreTimeHandler);
	bIsCameraAction = true;
	SetCameraState(ECameraEnum::Attack_Fist, 0.1f);

}

void UCCameraManagerComponent::OnSwordAttack()
{
	if (bIsCameraAction == true)
		return;
	if (StopAttackAction == true)
		return;

	GetWorld()->GetTimerManager().ClearTimer(RestoreTimeHandler);
	bIsCameraAction = true;
	SetCameraState(ECameraEnum::Attack_Sword, 0.1f);
}

void UCCameraManagerComponent::OnBowAttack()
{

}



void UCCameraManagerComponent::DefaultCameraChange(ECameraEnum InCameraNum)
{
	switch (InCameraNum)
	{
		case ECameraEnum::DefaultStandard:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultStandard];
			break;
		}
		case ECameraEnum::DefaultLong:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultLong];

			break;
		}
		case ECameraEnum::DefaultShort:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultShort];

			break;
		}
		case ECameraEnum::DefaultShort_soketoffset6:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultShort_soketoffset6];

			break;
		}
		case ECameraEnum::DefaultStandardLong:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultStandardLong];

			break;
		}
		case ECameraEnum::DefaultStandardMiddleLong_soketoffset60:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultStandardMiddleLong_soketoffset60];

			break;
		}
		case ECameraEnum::DefaultUpDownFix:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultUpDownFix];
			break;
		}
		case ECameraEnum::DefaultShort_AirAttack:
		{
			CameraPresets[ECameraEnum::Default] = CameraPresets[ECameraEnum::DefaultShort_AirAttack];
			break;
		}
	}

}

void UCCameraManagerComponent::SetbCameraTick(bool InBool)
{
	StopCameraAction = InBool;
}

void UCCameraManagerComponent::SetbCameraAttack(bool InBool)
{
	StopAttackAction = InBool;
}


void UCCameraManagerComponent::SetCameraState(ECameraEnum NewState, float CustomDuration)
{
	if (!CameraPresets.Contains(NewState) || !SpringArmComponent || !CameraComponent)
		return;
	
	ECameraEnum PreviousState = CurrentCameraState;
	CurrentCameraState = NewState;

	

	if (NewState != ECameraEnum::Default)
	{
		if (MovementComponent)
		{
			MovementComponent->SetFixedCamera(true);
		}

		bOriginalInheritYaw = SpringArmComponent->bInheritYaw;
		bOriginalUsePawnControlRotation = SpringArmComponent->bUsePawnControlRotation;
		bOriginalUseControllerRotationYaw = OwnerCharacter->bUseControllerRotationYaw;
		bOriginalOrientRotationToMovement = OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement;

		SpringArmComponent->bInheritYaw = false;
		if (NewState == ECameraEnum::Damaged || NewState == ECameraEnum::Evade || NewState == ECameraEnum::Attack_Fist || NewState == ECameraEnum::Attack_Sword)
		{
			SpringArmComponent->bInheritYaw = true;
			SpringArmComponent->bUsePawnControlRotation = true;

			OwnerCharacter->bUseControllerRotationYaw = false;
		}
		else
		{
			SpringArmComponent->bUsePawnControlRotation = false;
			OwnerCharacter->bUseControllerRotationYaw = false;
			OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = false;
		}
		


		float Duration = CustomDuration > 0.0f ? CustomDuration : DefaultTransitionDuration;
		GetWorld()->GetTimerManager().SetTimer(
			RestoreTimeHandler,
			[this]() { 
				SetCameraState(ECameraEnum::Default);
				bIsCameraAction = false;
			},
			Duration,
			false
		);
	}
	else
	{

		if (MovementComponent)
		{
			MovementComponent->SetFixedCamera(false);
		}

		SpringArmComponent->bInheritYaw = bOriginalInheritYaw;
		SpringArmComponent->bUsePawnControlRotation = bOriginalUsePawnControlRotation;
		OwnerCharacter->bUseControllerRotationYaw = bOriginalUseControllerRotationYaw;
		OwnerCharacter->GetCharacterMovement()->bOrientRotationToMovement = bOriginalOrientRotationToMovement;
	}

}





