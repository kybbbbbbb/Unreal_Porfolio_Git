#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "../Characters/CAnimInstance.h"
#include "CFootIKComponent.generated.h"


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
