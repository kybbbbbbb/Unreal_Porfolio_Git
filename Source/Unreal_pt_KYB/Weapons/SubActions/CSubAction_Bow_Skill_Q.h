#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction_RightMouse.h"
#include "CSubAction_Bow_Skill_Q.generated.h"

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCSubAction_Bow_Skill_Q : public UCSubAction_RightMouse
{
	GENERATED_BODY()

public:
	UCSubAction_Bow_Skill_Q();
private:
	UPROPERTY(EditAnywhere)
	FKey KeyName = EKeys::Q;
	UPROPERTY(EditAnywhere)
	ESubActionEnum SkillNumber = ESubActionEnum::Q;

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

	bool CanBeInCluster() const override { return false; }
private:
	bool GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation);
private:
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY()
	class UDecalComponent* DecalComponent;
	bool bDecalOn = false;
	

};
