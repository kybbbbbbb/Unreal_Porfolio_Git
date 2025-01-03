#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Characters/CAnimInstance.h"
#include "CFootIKComponent.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCFootIKComponent
  @ 설  명: Foot IK 기능을 가진 컴포넌트, 발 위치, 회전 각을 애님BP에 전달하는 역할
  @ 콜루트: 
  @ TODO:

------------------------------------------------------------------------*/
UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCFootIKComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCFootIKComponent();

private:
	ACharacter* OwnerCharacter;
	USkeletalMeshComponent* Skeletal;
	UCAnimInstance* AnimBP;

protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;
	void IsRiding(bool InBool);

private:
	void FootTrace();
	void SendAnimBluprint();
	FRotator NormalToRotator(FVector NormalVector, FName boneName);

private:
	UPROPERTY(EditAnywhere)
	FName LFootSocketName;
	UPROPERTY(EditAnywhere)
	FName RFootSocketName;
	UPROPERTY(EditAnywhere)
	float IKSpeed = 10.0f;

	FVector originSkeletalLocation;

	float LFootIKoffset;
	float RFootIKoffset;
	float pelvisIKoffset;
	FRotator LFootIKRotator;
	FRotator RFootIKRotator;

	bool isRiding = false;
};
