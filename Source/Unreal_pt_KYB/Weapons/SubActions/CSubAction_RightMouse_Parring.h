#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction_RightMouse.h"
#include "Components/CWeaponComponent_reset.h"
#include "CSubAction_RightMouse_Parring.generated.h"

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCSubAction_RightMouse_Parring : public UCSubAction_RightMouse
{
	GENERATED_BODY()
	

private:
	UPROPERTY(EditAnywhere)
	FKey KeyName = EKeys::RightMouseButton;
	UPROPERTY(EditAnywhere)
	ESubActionEnum SkillNumber = ESubActionEnum::RightMouseButton;
	UPROPERTY(EditAnywhere, Category = "Parring_Montage")
	UAnimMontage* SwordParringMontage;
	UPROPERTY(EditAnywhere, Category = "Parring_Montage")
	UAnimMontage* FistParringMontage;



public:
	void ChangedKey();
	FORCEINLINE FKey GetInSubAction_KeyNumber() override { return KeyName; }
	FORCEINLINE ESubActionEnum GetInSubAction_SkillNumber() override { return SkillNumber; }


public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float InDeltaTime) override;

public:
	void Pressed() override;
	void Released() override;
	void StopSubAction() override;
	void OnParring(EWeaponType Intype, AController* InEventInstigator);


	bool CanBeInCluster() const override { return false; }
private:
	void StartParryingTimer();
	void EndParrying();
	bool bisParring = false;


};
