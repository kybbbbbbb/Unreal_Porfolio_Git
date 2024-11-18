#include "Characters/CMonsters_AI.h"
#include "../Components/CWeaponComponent_reset.h"
#include "Components/CAIBehaviorComponent.h"
#include "../Components/CStateComponent.h"
#include "Components/WidgetComponent.h"
#include "Components/TextBlock.h"
#include "Components/ProgressBar.h"
#include "../Components/CHealthComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Item/CPortion.h"
#include "../GameManager/CSoundManager.h"
#include "GameFramework/CharacterMovementComponent.h"



ACMonsters_AI::ACMonsters_AI()
{
	PrimaryActorTick.bCanEverTick = true;

	WeaponComponent = CreateDefaultSubobject<UCWeaponComponent_reset>("WeaponComponent_reset");
	AIBehavior = CreateDefaultSubobject<UCAIBehaviorComponent>("AIBehaviorComponent");

	WidgetComponent = CreateDefaultSubobject<UWidgetComponent>("Widget");
	WidgetComponent->SetupAttachment(GetMesh());

	TSubclassOf<UUserWidget> labelClass;
	
	labelClass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/EnemyUI.EnemyUI_C'").Class;
	
	WidgetComponent->SetWidgetClass(labelClass);
	WidgetComponent->SetRelativeLocation(FVector(0, 0, 220));
	WidgetComponent->SetDrawSize(FVector2D(120, 30));
	WidgetComponent->SetWidgetSpace(EWidgetSpace::Screen);


}

void ACMonsters_AI::ParringAction()
{
	if (IsBoss == true)
	{
		neutralizeGage -= 10.0f;
		neutralizeGage = FMath::Clamp(neutralizeGage, 0.0f, 100.0f);

		if (FMath::IsNearlyZero(neutralizeGage))
		{
			PlayAnimMontage(StunMontage);
			//AIBehavior->SetStunMode();
			StateComponent->SetStunMode();
			
		}
		else
		{
			PlayAnimMontage(ParringMontage);
			AIBehavior->SetHittedMode();
		}
		return;
	}

	PlayAnimMontage(ParringMontage);
	AIBehavior->SetHittedMode();
}

void ACMonsters_AI::BeginPlay()
{
	Super::BeginPlay();
	HealthComponent->HPDelegate.AddDynamic(this, &ACMonsters_AI::OnHealthPointChange);

	WidgetComponent->InitWidget();

	UUserWidget* label = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
	UTextBlock* textblock = Cast<UTextBlock>(label->GetWidgetFromName("Name"));
	if (!MonsterName.IsEmpty())
	{
		textblock->SetText(MonsterName);
	}
	else
	{
		textblock->SetText(FText::FromString("NoName"));
	}
	
}

void ACMonsters_AI::Damage()
{

	if (IsBoss == true)
	{


		if (StateComponent->IsStunMode() == false && HealthComponent->IsDead() == false)
		{
			neutralizeGage -= 2.0f;
			neutralizeGage = FMath::Clamp(neutralizeGage, 0.0f, 100.0f);

			if (FMath::IsNearlyZero(neutralizeGage))
			{
				PlayAnimMontage(StunMontage);
				StateComponent->SetStunMode();
				return;
			}
		}
	}

	Super::Damage();
	
	if (IsBoss == true)
	{
		if (HealthComponent->GetCurrentHp() < HealthComponent->GetMaxHp() / 2)
		{
			AIBehavior->SetPhase(2);
		}
	}

	if (StateComponent->IsDeadMode())
	{
		if (MonsterName.EqualTo(FText::FromString("Goblin")))
		{
			UCSoundManager::GetInstance()->PlaySound3D("SC_Monster_Goblin_Dead", GetActorLocation(), GetWorld());
		}
		else if (MonsterName.EqualTo(FText::FromString("Sevarog")))
		{
			UCSoundManager::GetInstance()->PlaySound3D("SC_Monster_Raith_Dead", GetActorLocation(), GetWorld());
		}
		else if (MonsterName.EqualTo(FText::FromString("Boss")))
		{

		}



		if (DropPortionItem.Num() > 0)
		{
			int32 randomportionitem = FMath::RandRange(0, DropPortionItem.Num() - 1);

			ACPortion* spawnitem = GetWorld()->SpawnActor<ACPortion>(DropPortionItem[randomportionitem]->PortionData.portionBP, GetActorLocation(), GetActorRotation());
			spawnitem->SetitemID(DropPortionItem[randomportionitem]->GetPrimaryAssetId());

			UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(spawnitem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
			if (MeshComponent)
			{
				// 물리 시뮬레이션 활성화
				MeshComponent->SetSimulatePhysics(true);

				// 위로 던져지는 임펄스 적용
				FVector Impulse = FVector(FMath::RandRange(-200.0f, 200.0f), FMath::RandRange(-200.0f, 200.0f), 1000.0f); // 랜덤한 방향으로 약간 튕기면서 위로 던짐
				MeshComponent->AddImpulse(Impulse);

				// 회전력(Torque) 추가하여 회전 효과
				FVector Torque = FVector(FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f), FMath::RandRange(-500.0f, 500.0f));
				MeshComponent->AddTorqueInRadians(Torque, NAME_None, true);

				// 중력 활성화 확인
				MeshComponent->SetEnableGravity(true);
			}
		}



		AIBehavior->SetDeadMode();
		
		return;
	}


	
	if (IsBoss == true)
	{
		
		return;
	}
	else
	{
		if (MonsterName.EqualTo(FText::FromString("Goblin")))
		{
			UCSoundManager::GetInstance()->PlaySound3D("SC_Monster_Goblin_Hit_Short", GetActorLocation(), GetWorld());
		}
		else if (MonsterName.EqualTo(FText::FromString("Sevarog")))
		{
			UCSoundManager::GetInstance()->PlaySound3D("SC_Monster_Raith_Hit_Short", GetActorLocation(), GetWorld());
		}

		AIBehavior->SetHittedMode();
	}
}


void ACMonsters_AI::End_Damaged()
{
	Super::End_Damaged();
	AIBehavior->SetWaitMode();
	
}

void ACMonsters_AI::End_Dead()
{
	Super::End_Dead();
	WeaponComponent->DestroyWeapon();
	Destroy();
}


void ACMonsters_AI::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	Tick_LabelRenderScale();

	if(bOnAirDash == true)
		CheckLaunchDistance();
}

void ACMonsters_AI::StartEvade()
{
	if (Evade_Back != nullptr)
	{
		PlayAnimMontage(Evade_Back);
		StateComponent->SetEvadeMode();

	}

}

void ACMonsters_AI::CheckLaunchDistance()
{
	if (bOnAirDash == false)
		return;
	float DistanceTravelled = FVector::Dist(FirstLocation, GetActorLocation());
	if (DistanceTravelled > targetDistance - 50)
	{
		GetCharacterMovement()->StopMovementImmediately();
		GetCharacterMovement()->GravityScale = 2.0f;
		GetCharacterMovement()->GroundFriction = 8.0f;
		bOnAirDash = false;
	}

}

void ACMonsters_AI::StartLaunch(FVector InLaunchVelocity, float InDistance)
{
	FirstLocation = GetActorLocation();
	targetDistance = InDistance;

	
	FVector HorizontalLaunchDirection = InLaunchVelocity;
	HorizontalLaunchDirection.Z = 0.0f; 

	FRotator LaunchRotation = HorizontalLaunchDirection.Rotation();
	SetActorRotation(LaunchRotation);
	

	GetCharacterMovement()->GravityScale = 0.0f;
	GetCharacterMovement()->GroundFriction = 0.0f;
	LaunchCharacter(InLaunchVelocity, true, true);
	bOnAirDash = true;
}

void ACMonsters_AI::OnHealthPointChange(float RemainHP, float DamageAmount, float MaxHP)
{
	UUserWidget* label = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
	UProgressBar* ProgressBar = Cast<UProgressBar>(label->GetWidgetFromName("HP"));

	if (ProgressBar != nullptr)
	{
		float percentage = (RemainHP / MaxHP);
		ProgressBar->SetPercent(percentage);
	}

}


void ACMonsters_AI::Tick_LabelRenderScale()
{
	UUserWidget* label = Cast<UUserWidget>(WidgetComponent->GetUserWidgetObject());
	if (label == nullptr)
		return;


	APlayerController* playerController = UGameplayStatics::GetPlayerController(GetWorld(), 0);
	if (playerController == nullptr)
		return;
	APawn* playerPawn = playerController->GetPawn();
	if (playerPawn == nullptr)
		return;
	
	// 플레이어와의 거리 계산
	float distance = FVector::Distance(playerPawn->GetActorLocation(), GetActorLocation());

	
	if (distance > 1500.0f)
	{
		label->SetVisibility(ESlateVisibility::Collapsed);
		return;
	}
	// 1000 이하일 때는 완전한 불투명도를 유지
	float opacity = 1.0f;

	// 1000~1500 사이에서는 서서히 투명해짐
	if (distance > 1000.0f)
	{
		opacity = FMath::Clamp(1.0f - ((distance - 1000.0f) / 500.0f), 0.0f, 1.0f);
	}

	// 위젯 오퍼시티 적용
	label->SetRenderOpacity(opacity);
	label->SetVisibility(ESlateVisibility::Visible);
}
