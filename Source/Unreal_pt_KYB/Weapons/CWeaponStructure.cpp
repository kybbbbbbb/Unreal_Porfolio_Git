#include "Weapons/CWeaponStructure.h"
#include "GameFramework/Character.h"
#include "Components/CapsuleComponent.h"
#include "Components/CMovementComponent.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "NiagaraSystem.h"
#include "NiagaraFunctionLibrary.h"
#include "GameFramework/SpringArmComponent.h"
#include "../Components/CTargetComponent.h"
#include "SubActions/CSubAction_RangeSkill_BP_Base.h"
#include "../Characters/CPlayer.h"
#include "../GameManager/CCameraManagerComponent.h"
#include "../Components/CWeaponComponent_reset.h"
#include "../Characters/CMonsters_AI.h"

void FDoActionData::DoAction(ACharacter* InOwner, FName sectionName)
{
	UCMovementComponent* movement;

	movement = Cast<UCMovementComponent>(InOwner->GetComponentByClass(UCMovementComponent::StaticClass()));
	if (!!movement)
	{
		if (bCanMove == false)
			movement->Stop();
	}

	UCTargetComponent* TargetComponent = Cast<UCTargetComponent>(InOwner->GetComponentByClass(UCTargetComponent::StaticClass()));
	if (TargetComponent != nullptr)
	{
		if (TargetComponent->IsExistTarget() == false)
		{
			FRotator CameraRotation = InOwner->GetControlRotation();
			CameraRotation.Pitch = 0.0f;
			CameraRotation.Roll = 0.0f;
			InOwner->SetActorRotation(CameraRotation);
		}
		else
		{
			if (IsValid(TargetComponent->GetExistTarget()))
			{
				FRotator LookAtRotation = UKismetMathLibrary::FindLookAtRotation(InOwner->GetActorLocation(), TargetComponent->GetExistTarget()->GetActorLocation());
				InOwner->SetActorRotation(LookAtRotation);
			}
		}
	}
	

	if (!!Montage)
	{
		if (sectionName != "")
		{
			//InOwner->PlayAnimMontage(Montage, PlayRate, sectionName);
			UAnimInstance* AnimInstance = InOwner->GetMesh()->GetAnimInstance();

			// ��Ÿ�� ���
			AnimInstance->Montage_Play(Montage, PlayRate);

			// �ش� �������� �̵�
			AnimInstance->Montage_JumpToSection(sectionName, Montage);

			// ���� ���� ���Ŀ� �ٸ� ������ ������� �ʵ��� ����
			AnimInstance->Montage_SetNextSection(sectionName, FName("None"), Montage);
		}
		else
		{
			InOwner->PlayAnimMontage(Montage, PlayRate);
		}
	}



	//-------------24�� 10�� 5�� �߰� ��� (�ܺ� ���� �ߵ� ��ų)-------------//
	APlayerController* PlayerController;
	PlayerController = InOwner->GetController<APlayerController>();

	if (PlayerController == nullptr)
	{
		return;
	}

	//TODO: ���� ���콺 ���� ���ݽ� bool �� ����ü�� �߰��Ͽ� ���� �����
	//if(MouseFree == true)
	//FHitResult hitResult;
	//PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);


	FVector2D ScreenCenter = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY() / 2);


	FHitResult hitResult;
	PlayerController->GetHitResultAtScreenPosition(ScreenCenter, ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	//TODO: �ִ� ��ų �Ÿ��� ���� ���� ���� ���� �ʿ�
	//float SkillRange = 10.0f <-���� ������ ����ü�� ����
	//float distance = FVector::Distance(hitResult.Location, InOwner->GetActorLocation());
	//if(distance > SkillRange) return;


	if (hitResult.bBlockingHit == false)
		return;

	FVector spawnLocation = hitResult.Location;
	if (SpawnAttackableBlueprint != nullptr)
	{
		FTimerHandle TimerHandle; 
		InOwner->GetWorld()->GetTimerManager().SetTimer(TimerHandle, [spawnLocation, InOwner, this]()
			{
				SpawnAttackableSkillTimer(spawnLocation, InOwner);
			}, 1.0f, false); 
	}

	if (SpawnGroundDecal != nullptr)
	{
		UNiagaraSystem* NiagaraSystem_DoAction = Cast<UNiagaraSystem>(SpawnGroundDecal);
		if (NiagaraSystem_DoAction)
		{
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(InOwner->GetWorld(), NiagaraSystem_DoAction, hitResult.Location + FVector(0,0,5));
		}

		UParticleSystem* ParticleSystem_DoAction = Cast<UParticleSystem>(SpawnGroundDecal);
		if (ParticleSystem_DoAction)
		{
			UGameplayStatics::SpawnEmitterAtLocation(InOwner->GetWorld(), ParticleSystem_DoAction, hitResult.Location + FVector(0, 0, 5));
		}
	}
}

void FDoActionData::SpawnAttackableSkillTimer(FVector Location, ACharacter* InOwner)
{
	if (SpawnAttackableBlueprint != nullptr)
	{
		FActorSpawnParameters SpawnParams;
		ACSubAction_RangeSkill_BP_Base* RangeSkill = InOwner->GetWorld()->SpawnActor<ACSubAction_RangeSkill_BP_Base>(SpawnAttackableBlueprint, Location, FRotator::ZeroRotator, SpawnParams);
		RangeSkill->SetInstigator_RangeSkill(InOwner->GetController(), InOwner);
	}
}



//TODO: ���� ����ü�� ������ Ÿ�� �߰� �ؾ���, ������ ���� �߰��ϰ� �� ���� �´� ������ ���� �ʿ�
void FHitData::SendDamage(ACharacter* InAttaker, AActor* InAttackCauser, ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent)
{

	if (ACPlayer* Player = Cast<ACPlayer>(InAttaker))
	{
		// ���� �������� �ٸ� ī�޶� ȿ��
		if (UCCameraManagerComponent* CameraManager = Player->FindComponentByClass<UCCameraManagerComponent>())
		{
			UCWeaponComponent_reset* weaponcomp = Player->FindComponentByClass<UCWeaponComponent_reset>();
			UCSoundManager* soundManager = UCSoundManager::GetInstance();

			if (weaponcomp == nullptr)
				return;
			EWeaponType type = weaponcomp->GetType();

			if (type == EWeaponType::Fist)
			{
				CameraManager->OnFistAttack();
				soundManager->PlaySound3D("SC_Player_Fist_Hit", InOther->GetActorLocation(), Player->GetWorld());
			}
			else if (type == EWeaponType::Sword)
			{
				CameraManager->OnSwordAttack();
				soundManager->PlaySound3D("SC_Player_Sword_Fresh", InOther->GetActorLocation(), Player->GetWorld());
			}
			else if (type == EWeaponType::Bow)
			{
				soundManager->PlaySound3D("SC_Player_Bow_Hit", InOther->GetActorLocation(), Player->GetWorld());
			}
		}
	}
	else if(ACMonsters_AI* monster = Cast<ACMonsters_AI>(InAttaker))
	{
		UCSoundManager* soundManager = UCSoundManager::GetInstance();
		{
			soundManager->PlaySound3D("SC_Player_Sword_Fresh", InOther->GetActorLocation(), monster->GetWorld());
		}
	}

	FActionDamageEvent e;
	e.SweepResult = &SweepResult;
	e.HitData = this;
	e.OverlapComponent = OverlapComponent;
	InOther->TakeDamage(Power, e, InAttaker->GetController(), InAttackCauser);
}



void FHitData::PlayHitStop(UWorld* InWorld)
{
	if (FMath::IsNearlyZero(StopTime) == true)
		return;
	
	TArray<ACharacter*> characters;
	for (AActor* actor : InWorld->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);
		
		if (character)
		{
			character->CustomTimeDilation = 0.05f;
			characters.Add(character);
		}
	}
	
	UGameplayStatics::SetGlobalTimeDilation(InWorld, 0.2f);

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([=]()
		{
			for (ACharacter* character : characters)
			{
				if (!!character)
				{
					character->CustomTimeDilation = 1.0f;
					
				}
			}
			UGameplayStatics::SetGlobalTimeDilation(InWorld, 1.0f);
			

		});
	
	FTimerHandle timerHandle;
	InWorld->GetTimerManager().SetTimer(timerHandle, timerDelegate, StopTime*0.2f, false);
}


void FHitData::PlaySountWave(ACharacter* InOwner)
{
	if (SoundWaves.Num() < 1)
		return;

	int randomCount = FMath::RandRange(0, SoundWaves.Num() - 1);
	UGameplayStatics::SpawnSoundAtLocation(InOwner->GetWorld(), SoundWaves[randomCount], InOwner->GetActorLocation());
}

void FHitData::PlayEffect(UWorld* InWorld, const FVector& InLocation, const FRotator& InRotator)
{
	if (ParticleEffect == nullptr)
		return;
	UNiagaraSystem* NiagaraSystem = Cast<UNiagaraSystem>(ParticleEffect);
	if (NiagaraSystem)
	{
		UNiagaraFunctionLibrary::SpawnSystemAtLocation(InWorld, NiagaraSystem, InLocation + EffectLocation, InRotator, EffectScale);
	}

	UParticleSystem* ParticleSystem = Cast<UParticleSystem>(ParticleEffect);
	if (ParticleSystem)
	{
		UGameplayStatics::SpawnEmitterAtLocation(InWorld, ParticleSystem, InLocation + EffectLocation, InRotator, EffectScale);
	}
}
