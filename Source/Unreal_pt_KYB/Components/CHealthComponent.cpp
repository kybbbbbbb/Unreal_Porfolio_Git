#include "Components/CHealthComponent.h"

UCHealthComponent::UCHealthComponent()
{
	PrimaryComponentTick.bCanEverTick = true; 
}


void UCHealthComponent::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;
}

void UCHealthComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (CurrentMP == MaxMana)
		return;
	if (bCanRegenMP == false)
		return;
	// MP 회복 로직
	if (CurrentMP < MaxMana)
	{
		float MPRegen = 5 * DeltaTime;
		CurrentMP = FMath::Clamp(CurrentMP + MPRegen, 0.0f, MaxMana);

		if (MPDelegate.IsBound())
			MPDelegate.Broadcast(CurrentMP, -MPRegen, MaxMana);
	}
}

void UCHealthComponent::Damage(float InAmount)
{
	CurrentHealth += -InAmount;
	CurrentHealth = FMath::Clamp(CurrentHealth, 0.0f, MaxHealth);

	

	if (HPDelegate.IsBound())
		HPDelegate.Broadcast(CurrentHealth, InAmount, MaxHealth);
}

void UCHealthComponent::UsedMP(float InAmount)
{
	CurrentMP += -InAmount;
	CurrentMP = FMath::Clamp(CurrentMP, 0.0f, MaxMana);



	if (MPDelegate.IsBound())
		MPDelegate.Broadcast(CurrentMP, InAmount, MaxMana);

	bCanRegenMP = false;

	GetWorld()->GetTimerManager().ClearTimer(MPRegenTimerHandle);  
	GetWorld()->GetTimerManager().SetTimer(MPRegenTimerHandle, this, &UCHealthComponent::EnableMPRegen, 1.0f, false);
}

void UCHealthComponent::EnableMPRegen()
{
	bCanRegenMP = true;  
}

bool UCHealthComponent::IsDead()
{
	if (CurrentHealth == 0.0f)
		return true;
	else
		return false;
}





