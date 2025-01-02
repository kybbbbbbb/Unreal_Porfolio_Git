#include "Weapons/AddOns/CArrow.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"
#include "../../../../Plugins/FX/Niagara/Source/Niagara/Public/NiagaraComponent.h"
#include "GenericTeamAgentInterface.h"
#include "GameManager/CSoundManager.h"

ACArrow::ACArrow()
{
	Capsule = CreateDefaultSubobject<UCapsuleComponent>("Capsule");
	Projectile = CreateDefaultSubobject<UProjectileMovementComponent>("Projectile");

	Projectile->ProjectileGravityScale = 0.0f;

	//�ٵ�ùķ��̼� Ʈ��
	Capsule->BodyInstance.bNotifyRigidBodyCollision = true;
	Capsule->SetCollisionProfileName("BlockAll");
}

void ACArrow::BeginPlay()
{
	Super::BeginPlay();

	UNiagaraComponent* niagaraEffect = Cast<UNiagaraComponent>(GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (niagaraEffect != nullptr)
	{
		niagaraEffect->SetActive(false);
	}

	Projectile->SetActive(false);

	//��� �� �ݸ����� ��� ����
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->OnComponentHit.AddDynamic(this, &ACArrow::OnComponentHit);

	
	staticMesh = Cast<UStaticMeshComponent>(FindComponentByClass(UStaticMeshComponent::StaticClass()));
	OffVisibility();
	
}

void ACArrow::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	if (OnEndPlay.IsBound())
		OnEndPlay.Broadcast(this);
}

void ACArrow::Shoot(const FVector& InForward)
{
	isShoot = true;
	UNiagaraComponent* niagaraEffect = Cast<UNiagaraComponent>(GetComponentByClass(UNiagaraComponent::StaticClass()));
	if (niagaraEffect != nullptr)
	{
		niagaraEffect->SetActive(true);
	}

	Projectile->Velocity = InForward * Projectile->InitialSpeed;
	Projectile->SetActive(true);


	UCSoundManager::GetInstance()->PlaySound3D("SC_Player_Bow_Whosh",GetActorLocation(),GetWorld());

	//�߻� �� �� �ݸ��� On
	Capsule->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);

}

//arrow�� hit�� �� ����
void ACArrow::OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit)
{
	SetLifeSpan(LifeSpawnAfterCollision);

	for (AActor* actor : Ignores)
	{
		if(actor == OtherActor)
			return;
	}

	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	USkeletalMeshComponent* SkeletalMeshComp = Cast<USkeletalMeshComponent>(OtherActor->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	if (SkeletalMeshComp)
	{
		AttachToComponent(SkeletalMeshComp, FAttachmentTransformRules::KeepWorldTransform,Hit.BoneName);

	}
	
	IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherActor);

	if (OtherTeamAgent == nullptr)
		return;

	if (OtherTeamAgent->GetGenericTeamId() == FGenericTeamId(1))
		return;

	ACharacter* character = Cast<ACharacter>(OtherActor);

	if (!!character && OnHit.IsBound())
		OnHit.Broadcast(this, character, Hit);
}