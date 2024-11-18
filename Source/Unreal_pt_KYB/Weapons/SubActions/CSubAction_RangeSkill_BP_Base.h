#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Weapons/CWeaponStructure.h"
#include "CSubAction_RangeSkill_BP_Base.generated.h"


UCLASS()
class UNREAL_PT_KYB_API ACSubAction_RangeSkill_BP_Base : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ACSubAction_RangeSkill_BP_Base();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	

	virtual void Tick(float DeltaTime) override;
	void SetInstigator_RangeSkill(AController* InController, ACharacter* InOwnerCharacter);
	

	UFUNCTION(BlueprintCallable)
	void SendDamage_BP(FHitResult HitResult, FHitData HitData, ACharacter* InOther);


protected:
	UPROPERTY(BlueprintReadOnly)
	AController* Instigator1;

	UPROPERTY(BlueprintReadOnly)
	ACharacter* OwnerCharacter;

	UPROPERTY(EditAnywhere)
	float LifeTime = 5.0f;
};
