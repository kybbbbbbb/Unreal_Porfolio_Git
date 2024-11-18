#include "Weapons/SubActions/CSubAction_RangeSkill_BP_Base.h"
#include "GameFramework/Character.h"

ACSubAction_RangeSkill_BP_Base::ACSubAction_RangeSkill_BP_Base()
{
	PrimaryActorTick.bCanEverTick = true;


}

void ACSubAction_RangeSkill_BP_Base::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeTime);
}

void ACSubAction_RangeSkill_BP_Base::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ACSubAction_RangeSkill_BP_Base::SetInstigator_RangeSkill(AController* InController, ACharacter* InOwnerCharacter)
{
	Instigator1 = InController;
	OwnerCharacter = InOwnerCharacter;


}

void ACSubAction_RangeSkill_BP_Base::SendDamage_BP(FHitResult HitResult, FHitData HitData, ACharacter* InOther)
{
	FActionDamageEvent e;
	e.SweepResult = &HitResult;
	e.HitData = &HitData;

	InOther->TakeDamage(HitData.Power, e, OwnerCharacter->GetInstigatorController(), OwnerCharacter);
}



