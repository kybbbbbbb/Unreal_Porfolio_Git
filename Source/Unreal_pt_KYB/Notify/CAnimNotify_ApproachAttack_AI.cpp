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
	EndLocation.Z -= 10000.0f; // 충분히 큰 값으로 설정하여 아래로 트레이스

	FHitResult HitResult;
	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(ownerCharacter);

	if (ownerCharacter->GetWorld()->LineTraceSingleByChannel(HitResult, StartLocation, EndLocation, ECC_Visibility, CollisionParams))
	{
		// 디버그 라인 그리기
		//DrawDebugLine(GetWorld(), StartLocation, HitResult.Location, FColor::Green, false, 2.0f, 0, 1.0f);
		return HitResult.Location;
	}

	// 바닥을 찾지 못한 경우 원래 위치 반환
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



	// 오너 캐릭터와 타겟 캐릭터의 위치 계산
	FVector ownerLocation = ownerCharacter->GetActorLocation();
	FirstLocation = ownerLocation;
	FVector targetLocation = target->GetActorLocation();

	if (targetLocation.Z > 10.0f)
	{
		targetLocation = GetGroundLocation(targetLocation);
	}


	FVector directionToTarget = targetLocation - ownerLocation;
	directionToTarget.Z = 0.0f; // 수평 방향만 고려
	directionToTarget.Normalize(); // 방향 벡터 정규화
	float offsetDistance = 80.0f;

	FVector finalTargetLocation = targetLocation - (directionToTarget * offsetDistance);
	// 오너 캐릭터가 이동할 방향 계산
	FVector direction = (finalTargetLocation - ownerLocation);
	float distance = direction.Size();  // 거리 계산
	CalDistance = distance;
	direction.Normalize();  // 방향 벡터 정규화

	
	//DrawDebugPoint(ownerCharacter->GetWorld(), finalTargetLocation, 10.0f, FColor::Blue, false, 5.0f);

	
	// 속도 계산 (거리 / 시간)
	launchVelocity = (direction * LaunchRate);


	//여기에 데칼시작
	FVector decallocation = finalTargetLocation;
	if (AttackDecal)
	{
		UGameplayStatics::SpawnDecalAtLocation(
			ownerCharacter->GetWorld(),
			AttackDecal,
			FVector(AttackDecal_Size, AttackDecal_Size, 500.0f), // 데칼 크기
			finalTargetLocation, // 데칼 위치
			FRotator(90, 0, 0), // 데칼 회전
			1.5f // 데칼 지속 시간
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
