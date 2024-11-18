#include "Weapons/SubActions/CSubAction_RightMouse_Parring.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Components/CWeaponComponent_reset.h"
#include "Components/CMovementComponent.h"
#include "Characters/CMonsters_AI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Kismet/GameplayStatics.h"
#include "Characters/CPlayer.h"
#include "GameManager/CCameraManagerComponent.h"


void UCSubAction_RightMouse_Parring::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSubAction_RightMouse_Parring::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
}


void UCSubAction_RightMouse_Parring::StartParryingTimer()
{
	if (State->IsDamagedMode() == true)
		return;

	bisParring = true;
	State->SetParringMode();
	FTimerHandle timer;
	Owner->GetWorld()->GetTimerManager().SetTimer(timer, this, &UCSubAction_RightMouse_Parring::EndParrying, 1.0, false);
}

void UCSubAction_RightMouse_Parring::EndParrying()
{
	bisParring = false;
	State->SetIdleMode();
}


void UCSubAction_RightMouse_Parring::Pressed()
{
	Super::Pressed();
	
	StartParryingTimer();


}

void UCSubAction_RightMouse_Parring::Released()
{
	Super::Released();
	//여기서 하면 안됌 따로 발동을 만들어줘야한다.
	//GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, "On Parring________");


}

void UCSubAction_RightMouse_Parring::StopSubAction()
{
}

void UCSubAction_RightMouse_Parring::OnParring(EWeaponType Intype, AController* InEventInstigator)
{
	Movement->Stop();
	ACMonsters_AI* monster = Cast<ACMonsters_AI>(InEventInstigator->GetPawn());
	ACPlayer* player = Cast<ACPlayer>(Owner);
	if (monster == nullptr)
		return;

	FVector start = Owner->GetActorLocation();
	start.Z = 0.0f;
	FVector target = monster->GetActorLocation();
	target.Z = 0.0f;
	FVector direction = target - start;
	direction.Normalize();
	Owner->LaunchCharacter(-direction * 100.0f, false, false);
	

	//Movement->camera 카메라 액션 추가하기

	UCCameraManagerComponent* camera = player->GetComponentByClass<UCCameraManagerComponent>();
	if (Intype == EWeaponType::Sword)
	{
		Owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		camera->OnParry();
		Owner->PlayAnimMontage(SwordParringMontage, 1.0f, "start");
		if (monster != nullptr)
		{
			monster->ParringAction();
		}
	}
	else if (Intype == EWeaponType::Fist)
	{
		Owner->SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
		Owner->PlayAnimMontage(FistParringMontage, 1.0f, "start");

		if (monster != nullptr && monster->IsMonsterBoss() == true)
		{
			monster->ParringAction();
		}
	}


	
	




}


