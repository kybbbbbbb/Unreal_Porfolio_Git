#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GenericTeamAgentInterface.h"
#include "../Item/CDestructibleObject.h"

ACAttachment::ACAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
}


//블루브린트에서 비긴시 콜
void ACAttachment::AttachTo(FName InSocketName)
{
	AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules(EAttachmentRule::KeepRelative,true), InSocketName);
}

void ACAttachment::AttachToCillision(FName InCollisionName)
{
	for (UShapeComponent* temp : Collisions)
	{
		if (temp->GetName() == InCollisionName)
		{
			temp->AttachToComponent(OwnerCharacter->GetMesh(), FAttachmentTransformRules::KeepRelativeTransform, InCollisionName);
			return;
		}
	}
}


void ACAttachment::BeginPlay()
{
	OwnerCharacter = Cast<ACharacter>(GetOwner());

	TArray<USceneComponent*> Childs;
	Root->GetChildrenComponents(true, Childs);

	for (USceneComponent* Child : Childs)
	{
		UShapeComponent* shape = Cast<UShapeComponent>(Child);

		if (shape != nullptr)
		{
			//이름만 같은 것, shape의 온컴포넌트오버랩(원본 델리게이트)에 어태치의 온컴포넌트오버랩(함수)을 바인딩한것이다.
			//그리고 해당 함수안에서 우리가 만든 델리게이트를 호출한다.
			//즉, shape에서 오버랩이 호출되어서 브로드캐스트 되면 우리가 바인딩한 함수가 호출되면서 우리의 델리게이트도 브로드캐스트 된다.
			shape->OnComponentBeginOverlap.AddDynamic(this, &ACAttachment::OnComponentBeginOverlap);
			shape->OnComponentEndOverlap.AddDynamic(this, &ACAttachment::OnComponentEndOverlap);
			Collisions.Add(shape);
		}
	}

	OffCollision(FName());

	Super::BeginPlay();
	
}

void ACAttachment::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);


	if (bSweepOn == false)
		return;
	OnWeaponSweepTraceOverlap();

}

//콜리전 검사의 불확실성을 해결하고자 제작
//모양은 콜리전의 모양을 가져다가 트레이스 검사 시작
void ACAttachment::OnWeaponSweepTraceOverlap()
{
	if (bSweepOn == false)
		return;
	if (Collisions.Num() < 1)
		return;
	if (CurrentSweepTraceCollision == nullptr)
		return;

	UCapsuleComponent* capsule = Cast<UCapsuleComponent>(CurrentSweepTraceCollision);
	FHitResult HitResult;
	FVector Start = capsule->GetComponentLocation();
	FVector End = Start + capsule->GetRightVector()*10.0f;

	FCollisionQueryParams Params(NAME_None, false, this);
	Params.AddIgnoredActor(this);
	Params.AddIgnoredActor(OwnerCharacter);

	bool bResult = GetWorld()->SweepSingleByChannel(HitResult, Start, End, capsule->GetComponentQuat(), ECollisionChannel::ECC_EngineTraceChannel2, FCollisionShape::MakeCapsule(capsule->GetScaledCapsuleRadius(), capsule->GetScaledCapsuleHalfHeight()), Params);
	
	if (HitResult.GetActor() == nullptr)
		return;
	if (HitResult.GetActor()->IsA(ACDestructibleObject::StaticClass()))
	{
		ACDestructibleObject* DestructibleObject = Cast<ACDestructibleObject>(HitResult.GetActor());
		if (DestructibleObject != nullptr)
		{
			DestructibleObject->BreakObject(HitResult.ImpactPoint, (HitResult.TraceEnd - HitResult.TraceStart).GetSafeNormal());
			return;
		}
	}

	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(HitResult.GetActor());

	
	if (OtherTeamAgent == nullptr || OwnerTeamAgent == nullptr)
		return;


	if (OwnerTeamAgent->GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId())
		return;

	if (bResult)
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter, this, Cast<ACharacter>(HitResult.GetActor()), HitResult, nullptr);
	}



#if ENABLE_DRAW_DEBUG
\
	//타겟 발견시 녹색,미발견시 빨강
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//생성후 삭제되기까지의 시간
	float DebugLifeTime = 5.0f;


	//캡슐 디버그 메쉬그리기
	DrawDebugCapsule(GetWorld(), Start, capsule->GetScaledCapsuleHalfHeight(), capsule->GetScaledCapsuleRadius(), capsule->GetComponentQuat(), DrawColor, false, DebugLifeTime);
#endif

}


//필요시 콜리전 이름으로 활성화 가능
void ACAttachment::OnCollision(FName InName)
{
	if (InName.IsNone())
	{
		for (UShapeComponent* Collision : Collisions)
		{
			//NormalAttack은 기본 공격 콜리전
			if (Collision->GetName() == "NormalAttack")
			{
				Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
			}
		}
	}
	else
	{
		for (UShapeComponent* Collision : Collisions)
		{
			if (Collision->GetName() == InName.ToString())
			{
				Collision->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
				
				break; 
			}
		}

	}

}

void ACAttachment::OffCollision(FName InName)
{
	if (InName.IsNone())
	{
		for (UShapeComponent* Collision : Collisions)
		{
			
			Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}
	}
	else
	{
		for (UShapeComponent* Collision : Collisions)
		{
			Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		}

	}

}

void ACAttachment::OnSweepTrace(FName InName)
{
	bSweepOn = true;

	if (InName.IsNone())
	{
		for (UShapeComponent* Collision : Collisions)
		{
			if (Collision->GetName() == "NormalAttack")
			{
				CurrentSweepTraceCollision = Collision;
				return;
			}
		}
	}
	else
	{
		for (UShapeComponent* Collision : Collisions)
		{
			if (Collision->GetName() == InName.ToString())
			{
				CurrentSweepTraceCollision = Collision;
				return;
			}
		}
	}

}

void ACAttachment::OffSweepTrace(FName InName)
{
	bSweepOn = false;
	CurrentSweepTraceCollision = nullptr;
}

//무기 오버랩 시 동작 로직
void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	//프랙처 오브젝츠 파괴
	ACDestructibleObject* DestructibleObject = Cast<ACDestructibleObject>(OtherActor);
	if (DestructibleObject != nullptr)
	{
		DestructibleObject->BreakObject(SweepResult.ImpactPoint, (SweepResult.TraceEnd - SweepResult.TraceStart).GetSafeNormal());
	}

	if (OwnerCharacter == OtherActor)
		return;

	IGenericTeamAgentInterface* OwnerTeamAgent = Cast<IGenericTeamAgentInterface>(OwnerCharacter);
	IGenericTeamAgentInterface* OtherTeamAgent = Cast<IGenericTeamAgentInterface>(OtherActor);
	if (OtherTeamAgent == nullptr || OwnerTeamAgent == nullptr)
		return;

	if (OwnerTeamAgent->GetGenericTeamId() == OtherTeamAgent->GetGenericTeamId())
		return;

	if (OnAttachmentBeginOverlap.IsBound())
	{
		OnAttachmentBeginOverlap.Broadcast(OwnerCharacter,this,Cast<ACharacter>(OtherActor), SweepResult, OverlappedComponent);

	}

}

void ACAttachment::OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (OwnerCharacter == OtherActor)
		return;

	if (OnAttachmentEndOverlap.IsBound())
		OnAttachmentEndOverlap.Broadcast(OwnerCharacter, Cast<ACharacter>(OtherActor));
}


