#include "Characters/CMonsters.h"
#include "Components/CMovementComponent.h"
#include "Characters/CAnimInstance.h"
#include "../Components/CWeaponComponent_reset.h"
#include "Components/CStateComponent.h"
#include "Components/CHealthComponent.h"
#include "Weapons/CWeaponStructure.h"
#include "Kismet/KismetMathLibrary.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "Materials/MaterialInstance.h"
#include "../Weapons/AddOns/CArrow.h"
#include "../Weapons/CDoAction.h"
#include "Perception/AISenseEvent_Damage.h"
#include "../GameManager/CUIManager_Game.h"

ACMonsters::ACMonsters()
{
 	
	MovementComponent = CreateDefaultSubobject<UCMovementComponent>("MovementComponent");
	
	StateComponent = CreateDefaultSubobject<UCStateComponent>("StateComponent");
	HealthComponent = CreateDefaultSubobject<UCHealthComponent>("HealthComponent");
	DeadMaterial = ConstructorHelpers::FObjectFinder<UMaterialInstance>(L"/Script/Engine.MaterialInstanceConstant'/Game/Characters/02_Monster/Empty_Inst.Empty_Inst'").Object;

	GetMesh()->SetRelativeLocation(FVector(0, 0, -90));
	GetMesh()->SetRelativeRotation(FRotator(0, -90, 0));



	StateComponent->OnStateTypeChanged.AddDynamic(this, &ACMonsters::OnStateTypeChange);

}

void ACMonsters::BeginPlay()
{
	Super::BeginPlay();
	UCUIManager_Game* PlayerUI = UCUIManager_Game::GetInstance(GetWorld());
	
	if (IsBoss == true)
		neutralizeGage = 100.0f;
}

//FHitdata�� ���� ��ä�� ���޹���
float ACMonsters::TakeDamage(float Damage, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	Damage = Super::TakeDamage(Damage, DamageEvent, EventInstigator, DamageCauser);

	//-------------24�� 11�� 10�� �߰� ��� (���Ͻ� ������ó��)-------------//
	if (StateComponent->IsStunMode())
	{
		HealthComponent->Damage(Damage * 2);

		if (HealthComponent->IsDead())
		{
			StateComponent->SetDeadMode();
			UCUIManager_Game* PlayerUI = UCUIManager_Game::GetInstance(GetWorld());
			PlayerUI->MonsterDeadDelegate();

			return Damage;
		}

		this->Damage();

		return Damage;
	}






	//-------------24�� 10�� 14�� �߰� ��� (AI���� ������ ���� Ȱ��ȭ)-------------//
	FVector HitLocation = DamageCauser ? DamageCauser->GetActorLocation() : GetActorLocation();
	
	
	UAISense_Damage::ReportDamageEvent(GetWorld(), this, EventInstigator->GetPawn(), Damage, GetActorLocation(), HitLocation);
	//---------------------------------------------------------------------------//





	ACArrow* arrow = Cast<ACArrow>(DamageCauser);

	if (arrow != nullptr && IsBoss == true)
	{
		if (ArrowMontage_for_Boss.Num() > 0)
		{
			int8 maxindex = ArrowMontage_for_Boss.Num() - 1;

			int8 randomint = FMath::RandRange(0, maxindex);
			if(StateComponent->IsIdleMode() == true)
				PlayAnimMontage(ArrowMontage_for_Boss[randomint], 1.0f,"start");
		}
		return Damage;
	}


	DamageData.Power = Damage;

	if (EventInstigator != nullptr)
	{
		APawn* pawn = EventInstigator->GetPawn();
		if (pawn != nullptr)
		{
			ACharacter* aCharacter = Cast<ACharacter>(pawn);
			if (aCharacter != nullptr)
			{
				DamageData.Attacker = aCharacter; // ������ ����
			}
		}
	}
	DamageData.Causer = DamageCauser;
	DamageData.Event = (FActionDamageEvent*)&DamageEvent;
	
	StateComponent->SetDamagedMode();

	return Damage;
}

void ACMonsters::OnStateTypeChange(EStateType PrevType, EStateType NewType)
{
	switch (NewType)
	{
	case EStateType::Idle:
		break;
	case EStateType::Evade:
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
	case EStateType::Stun:

		break;
	case EStateType::Max:
		break;
	default:
		break;
	}

}

//���� ����ü�� ����� ��Ʈ�����͸� �̿�
void ACMonsters::Damage()
{
	if (HealthComponent->IsDead())
		return;
	if (StateComponent->IsStunMode())
		return;
	
	//������ ����
	HealthComponent->Damage(DamageData.Power);
	DamageData.Power = 0;

	//��Ÿ�� ���(��ų�� ������ �޶�����)
	//PlayAnimMontage()

	if (!!DamageData.Event && !!DamageData.Event->HitData)
	{
		FHitData* hitData = DamageData.Event->HitData;


		if (hitData->ParticleEffect != nullptr)
		{
			if(DamageData.Event->SweepResult->ImpactPoint == FVector::Zero())
				hitData->PlayEffect(GetWorld(), GetActorLocation(), DamageData.Event->SweepResult->ImpactNormal.Rotation());

			hitData->PlayEffect(GetWorld(), DamageData.Event->SweepResult->ImpactPoint, DamageData.Event->SweepResult->ImpactNormal.Rotation());
		}
		hitData->PlaySountWave(this);

		if (HealthComponent->IsDead() == false)
		{
			if (IsBoss == true)
			{



				UCWeaponComponent_reset* weaponcomponent = GetComponentByClass<UCWeaponComponent_reset>();
				if(weaponcomponent != nullptr)
				{
					if (weaponcomponent->GetDoAction()->GetInAction() == false)
					{
						FVector start = GetActorLocation();
						start.Z = 0.0f;
						FVector target = DamageData.Attacker->GetActorLocation();
						target.Z = 0.0f;
						FVector direction = target - start;
						direction.Normalize();
						LaunchCharacter(-direction * (hitData->Launch), false, false);

						UAnimMontage* montage = hitData->HitMontage;
						if (montage == nullptr)
						{
							montage = StandardDamageMontage;
						}
						PlayAnimMontage(montage, 1.0f, "start");
					}
				}
			}
			else
			{
				

				FVector forward = GetActorForwardVector();
				FVector actorlocation = GetActorLocation();
				actorlocation.Z = 0.0f;
				
				FVector CauserLocation = DamageData.Causer->GetActorLocation();
				if (DamageData.Event->OverlapComponent != nullptr)
				{
					CauserLocation = DamageData.Event->OverlapComponent->GetComponentLocation();
				}
				CauserLocation.Z = 0.0f;
				forward.Z = 0.0f;

				FVector hitdirection = CauserLocation - actorlocation;


				float forwardDot = FVector::DotProduct(forward, hitdirection.GetSafeNormal());

				FVector crossProduct = FVector::CrossProduct(forward, hitdirection.GetSafeNormal());
				float crossZ = crossProduct.Z;

				UAnimMontage* montage = hitData->HitMontage;
				if (montage == nullptr)
				{
					if (forwardDot > 0.8f)
					{
						// ���鿡�� ������ ���� ��� (��Ʈ ���δ�Ʈ ���� 0.7 �̻�)
						montage = HitMontageFront;
					}
					else
					{
						// ���鿡�� ������ ���� ��� (��Ʈ ���δ�Ʈ ���� -0.7 ~ 0.7 ����)
						if (crossZ > 0)
						{
							// �����ʿ��� ������ ���� ���
							montage = HitMontageRight;
						}
						else
						{
							// ���ʿ��� ������ ���� ���
							montage = HitMontageLeft;
						}
					}
				}



				if (montage == nullptr)
				{
					montage = StandardDamageMontage;
				}

				PlayAnimMontage(montage, 1.0f, "start");

				if (DamageData.Attacker != nullptr)
				{
					FVector start = GetActorLocation();
					start.Z = 0.0f;
					FVector target = DamageData.Attacker->GetActorLocation();
					target.Z = 0.0f;
					FVector direction = target - start;
					direction.Normalize();



					LaunchCharacter(-direction * (hitData->Launch), false, false);


					SetActorRotation(UKismetMathLibrary::FindLookAtRotation(start, target));
				}

			}
			
		}

		hitData->PlayHitStop(GetWorld());
	}

	if (HealthComponent->IsDead())
	{
		StateComponent->SetDeadMode();
		UCUIManager_Game* PlayerUI = UCUIManager_Game::GetInstance(GetWorld());
		PlayerUI->MonsterDeadDelegate();

		return;
	}

	DamageData.Attacker = nullptr;
	DamageData.Causer = nullptr;
	DamageData.Event = nullptr;

}

void ACMonsters::Dead()
{
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PlayAnimMontage(DeadAnimMontage, 1.2f);
	

}

void ACMonsters::End_Damaged()
{
	StateComponent->SetIdleMode();

}

void ACMonsters::End_Dead()
{
	
}

void ACMonsters::Opacity_zero()
{
	if (DeadMaterial == nullptr)
		return;

	GetMesh()->SetVisibility(false);

}



