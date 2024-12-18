#include "Weapons/SubActions/CSubAction_Sword_Skill_E.h"
#include "../CDoAction_Combo.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "Characters/CMonsters_AI.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameManager/CCameraManagerComponent.h"
#include "GameManager/CUIManager_Game.h"

void UCSubAction_Sword_Skill_E::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
}

void UCSubAction_Sword_Skill_E::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (bIsMoving) // 이동 여부를 체크
	{
		FVector NewLocation = FMath::VInterpTo(Owner->GetActorLocation(), finalLocation, InDeltaTime, 10.0f);
		Owner->SetActorLocation(NewLocation, true);
		if (target != nullptr)
		{
			FRotator rotator = UKismetMathLibrary::FindLookAtRotation(Owner->GetActorLocation(), target->GetActorLocation());
			Owner->SetActorRotation(rotator);
			AController* controller = Owner->GetController();

			FRotator calculateRotator = FRotator(controller->GetControlRotation().Pitch, rotator.Yaw, 0);
			FRotator final = FMath::RInterpTo(controller->GetControlRotation(), calculateRotator,InDeltaTime,7.0f);
			controller->SetControlRotation(final);
		}
		// 목표 위치에 도달하면 이동 중지
		if (FVector::Dist(NewLocation, finalLocation) < 10.0f)
		{
			prevTarget = target;
			target = nullptr;
			bIsMoving = false;
		}
	}
}

void UCSubAction_Sword_Skill_E::Pressed()
{
	Super::Pressed();
	if (bInAction == true)
		return;
	if (bCoolTimeOn == true)
		return;
	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Debug");
	bInAction = true;

	//스피어 트레이스로 추적
	FVector location = Owner->GetActorLocation();

	TArray<AActor*> ignore;
	ignore.Add(Owner);

	TArray<FHitResult> hitResult;
	UKismetSystemLibrary::SphereTraceMultiByProfile(Owner->GetWorld(), location, location, 2000.0f, "Targeting", false, ignore, DrawDebug, hitResult, true);
	target = GetNealyFromAngle(hitResult);

	GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("%d"), hitResult.IsEmpty()));
	
	if (target == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, "Noactor");
		bInAction = false;
		StopSubAction();
		return;
	}
	else
	{
		UCDoAction_Combo* DoActionCombo = Cast<UCDoAction_Combo>(DoAction);
		if (DoActionCombo != nullptr)
			DoActionCombo->DoAction();
		FVector gap = Owner->GetActorLocation() - target->GetActorLocation();
		gap = gap.GetSafeNormal2D();
		gap = gap * 100.0f;
		finalLocation = gap + target->GetActorLocation();

		Owner->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn_NoPawnBlock"));
		bIsMoving = true;

		UCCameraManagerComponent* Camera = Owner->GetComponentByClass<UCCameraManagerComponent>();
		Camera->SetbCameraAttack(true);

		count--;
		if (count == 0)
		{
			CoolTimeOn();
		}
	}


	//-----------------------------




}

void UCSubAction_Sword_Skill_E::Released()
{

}

void UCSubAction_Sword_Skill_E::StopSubAction()
{
	Super::StopSubAction();
	bInAction = false;
	bIsMoving = false;
	Owner->GetCapsuleComponent()->SetCollisionProfileName(TEXT("Pawn"));
	State->OffSubActionMode();
	UCCameraManagerComponent* Camera = Owner->GetComponentByClass<UCCameraManagerComponent>();
	Camera->SetbCameraAttack(false);
}

ACharacter* UCSubAction_Sword_Skill_E::GetNealyFromAngle(const TArray<FHitResult>& Inresult)
{
	float angle = -1.0f;
	ACharacter* candidate = nullptr;
	if (Inresult.Num() == 1)
		prevTarget = nullptr;

	for (const FHitResult& result : Inresult)
	{
		if (result.GetActor() == nullptr)
			continue;

		UCStateComponent* TargetState = Cast<UCStateComponent>(result.GetActor()->GetComponentByClass(UCStateComponent::StaticClass()));
		if (TargetState == nullptr)
			continue;
		if (TargetState->IsDeadMode() == true)
			continue;
		ACMonsters_AI* monster = Cast<ACMonsters_AI>(result.GetActor());
		if (monster == nullptr)
			continue;
		if (monster->GetTeamID() == 1)
			continue;
		if (prevTarget != nullptr)
		{
			if (prevTarget == result.GetActor())
			{
				continue;
			}
		}

		FVector targetLocation = result.GetActor()->GetActorLocation();
		FVector direction = targetLocation - Owner->GetActorLocation();
		direction.Normalize();

		FRotator rotator = Owner->GetActorRotation();
		FVector forward = FQuat(rotator).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);

		if (dot >= angle)
		{
			angle = dot;
			candidate = Cast<ACharacter>(result.GetActor());
		}
	}
	return candidate;
}

void UCSubAction_Sword_Skill_E::CoolTimeOn()
{
	bCoolTimeOn = true;
	Owner->GetWorld()->GetTimerManager().SetTimer(CoolTimehandler, this, &UCSubAction_Sword_Skill_E::CoolTimeOver, CoolTime, false);
	UCUIManager_Game::GetInstance(Owner->GetWorld())->SetSkillCoolTime(CoolTime, &CoolTimehandler, KeyName);
}

void UCSubAction_Sword_Skill_E::CoolTimeOver()
{
	count = 5;
	bCoolTimeOn = false;
}
