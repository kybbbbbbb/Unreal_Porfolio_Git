#include "Weapons/CAttachment.h"
#include "GameFramework/Character.h"
#include "Components/ShapeComponent.h"
#include "Components/CapsuleComponent.h"
#include "DrawDebugHelpers.h"
#include "GenericTeamAgentInterface.h"

ACAttachment::ACAttachment()
{
	PrimaryActorTick.bCanEverTick = true;
	Root = CreateDefaultSubobject<USceneComponent>("Root");
}


//���기Ʈ���� ���� �������� �����.
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
			//�̸��� ���� ��, shape�� ��������Ʈ������(���� ��������Ʈ)�� ����ġ�� ��������Ʈ������(�Լ�)�� ���ε��Ѱ��̴�.
			//�׸��� �ش� �Լ��ȿ��� �츮�� ���� ��������Ʈ�� ȣ���Ѵ�.
			//��, shape���� �������� ȣ��Ǿ ��ε�ĳ��Ʈ �Ǹ� �츮�� ���ε��� �Լ��� ȣ��Ǹ鼭 �츮�� ��������Ʈ�� ��ε�ĳ��Ʈ �ȴ�.
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
	//Ÿ�� �߽߰� ���,�̹߽߰� ����
	FColor DrawColor = bResult ? FColor::Green : FColor::Red;
	//������ �����Ǳ������ �ð�
	float DebugLifeTime = 5.0f;


	//ĸ�� ����� �޽��׸���
	DrawDebugCapsule(GetWorld(), Start, capsule->GetScaledCapsuleHalfHeight(), capsule->GetScaledCapsuleRadius(), capsule->GetComponentQuat(), DrawColor, false, DebugLifeTime);
#endif

}



void ACAttachment::OnCollision(FName InName)
{
	if (InName.IsNone())
	{
		for (UShapeComponent* Collision : Collisions)
		{
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

void ACAttachment::OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
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


