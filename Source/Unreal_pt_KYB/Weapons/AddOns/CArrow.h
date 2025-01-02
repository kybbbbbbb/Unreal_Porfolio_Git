#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CArrow.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FArrowHit, class AActor*, InCauser, class ACharacter*, InOtherCharacter, const FHitResult&, Hit);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FProjectileEndPlay, class ACArrow*, InDestoryer);

/*------------------------------------------------------------------------

  @ ��  ��: ACArrow
  @ ��  ��: DoAction_Bow���� ���� ����� ����, ������Ÿ��.
  @ �ݷ�Ʈ: DoAction_bow -> ACArrow����
  @ TODO: X

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API ACArrow : public AActor
{
	GENERATED_BODY()
	
public:	
	ACArrow();
private:
	UPROPERTY(EditDefaultsOnly, Category = "LifeSpawn")
	float LifeSpawnAfterCollision = 1;

	UPROPERTY(VisibleAnywhere)
	class UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere)
	class UProjectileMovementComponent* Projectile;


public:
	FORCEINLINE void AddIgnoreActor(AActor* InActor) { Ignores.Add(InActor); }
	void Shoot(const FVector& InForward);

	FORCEINLINE void OnVisibility() { staticMesh->SetVisibility(true); }
	FORCEINLINE void OffVisibility() { staticMesh->SetVisibility(false); }
	FORCEINLINE bool IsShoot() { return isShoot; }

protected:
	void BeginPlay() override;
	void EndPlay(const EEndPlayReason::Type EndPlayReason) override;

private:
	UFUNCTION()
	void OnComponentHit(UPrimitiveComponent* HitComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, FVector NormalImpulse, const FHitResult& Hit);

public:
	FArrowHit OnHit;
	FProjectileEndPlay OnEndPlay;

private:
	TArray<AActor*> Ignores;
	class UStaticMeshComponent* staticMesh;

private:
	bool isShoot = false;
};
