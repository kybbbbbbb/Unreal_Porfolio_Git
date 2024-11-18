#include "Notify/CAnimNotify_ApproachAttack_AI.h"
#include "../Components/CAIBehaviorComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "../Characters/CMonsters_AI.h"
#include "Kismet/GameplayStatics.h"

FString UCAnimNotify_ApproachAttack_AI::GetNotifyName_Implementation() const
{
	return "ApproachAttack";
}

FVector UCAnimNotify_ApproachAttack_AI::GetGroundLocation(FVector StartLocation)
{
	FVector EndLocation = StartLocation;
	EndLocation.Z -= 10000.0f; // ����� ū ������ �����Ͽ� �Ʒ��� Ʈ���̽�

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(ownerCharacter);

	if (ownerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		// ����� ���� �׸���
		//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Green, false, 2.0f, 0, 1.0f);
		return HitResult.Location;
	}

	// �ٴ��� ã�� ���� ��� ���� ��ġ ��ȯ
	return StartLocation;

}

void UCAnimNotify_ApproachAttack_AI::NotifyBegin(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float TotalDuration, const FAnimNotifyEventReference& EventReference)
{
	if (MeshComp == nullptr || MeshComp->GetOwner() == nullptr)
		return;
	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;

	UCAIBehaviorComponent* behavior = Cast<UCAIBehaviorComponent>(MeshComp->GetOwner()->GetComponentByClass(UCAIBehaviorComponent::StaticClass()));

	if (behavior == nullptr)
		return;
	ACharacter* target = behavior->GetTarget();
	if (target == nullptr)
		return;

	ownerCharacter = Cast<ACharacter>(MeshComp->GetOwner());
	if (ownerCharacter == nullptr)
		return;



	// ���� ĳ���Ϳ� Ÿ�� ĳ������ ��ġ ���
	FVector ownerLocation = ownerCharacter->GetActorLocation();
	FirstLocation = ownerLocation;
	FVector targetLocation = target->GetActorLocation();

	if (targetLocation.Z > 10.0f)
	{
		targetLocation = GetGroundLocation(targetLocation);
	}


	FVector directionToTarget = targetLocation - ownerLocation;
	directionToTarget.Z = 0.0f; // ���� ���⸸ ���
	directionToTarget.Normalize(); // ���� ���� ����ȭ
	float offsetDistance = 80.0f;

	FVector finalTargetLocation = targetLocation - (directionToTarget * offsetDistance);
	// ���� ĳ���Ͱ� �̵��� ���� ���
	FVector direction = (finalTargetLocation - ownerLocation);
	float distance = direction.Size();  // �Ÿ� ���
	CalDistance = distance;
	direction.Normalize();  // ���� ���� ����ȭ

	
	//DrawDebugPoint(ownerCharacter->GetWorld(), finalTargetLocation, 10.0f, FColor::Blue, false, 5.0f);

	
	// �ӵ� ��� (�Ÿ� / �ð�)
	launchVelocity = (direction * LaunchRate);


	//���⿡ ��Į����
	FVector decallocation = finalTargetLocation;
	if (AttackDecal)
	{
		UGameplayStatics::SpawnDecalAtLocation(
			ownerCharacter->GetWorld(),
			AttackDecal,
			FVector(AttackDecal_Size, AttackDecal_Size, 500.0f), // ��Į ũ��
			finalTargetLocation, // ��Į ��ġ
			FRotator(90, 0, 0), // ��Į ȸ��
			1.5f // ��Į ���� �ð�
		);
	}
	
}

void UCAnimNotify_ApproachAttack_AI::NotifyTick(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, float FrameDeltaTime, const FAnimNotifyEventReference& EventReference)
{



}



void UCAnimNotify_ApproachAttack_AI::NotifyEnd(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	ACMonsters_AI* monster_ai = Cast<ACMonsters_AI>(ownerCharacter);
	if (monster_ai == nullptr)
		return;
	monster_ai->StartLaunch(launchVelocity, CalDistance);

}
