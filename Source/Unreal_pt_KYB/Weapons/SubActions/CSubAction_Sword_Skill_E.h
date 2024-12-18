#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction_RightMouse.h"
#include "Kismet/KismetSystemLibrary.h"
#include "CSubAction_Sword_Skill_E.generated.h"

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCSubAction_Sword_Skill_E : public UCSubAction_RightMouse
{
	GENERATED_BODY()
	
private:
	UPROPERTY(EditAnywhere)
	FKey KeyName = EKeys::E;
	UPROPERTY(EditAnywhere)
	ESubActionEnum SkillNumber = ESubActionEnum::E;

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
	UPROPERTY()
	class APlayerController* PlayerController;
	UPROPERTY(EditAnywhere, Category = "Debug")
	TEnumAsByte<EDrawDebugTrace::Type> DrawDebug;

	ACharacter* GetNealyFromAngle(const TArray<FHitResult>& Inresult);
	bool bIsMoving = false;
	FVector finalLocation;
	AActor* target = nullptr;
	AActor* prevTarget = nullptr;

	void CoolTimeOn();
	void CoolTimeOver();
	bool bCoolTimeOn = false;

	int8 count = 5;
};
