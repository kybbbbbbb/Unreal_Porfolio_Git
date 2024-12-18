#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CSubAction_RightMouse.generated.h"

UENUM()
enum class ESubActionEnum : uint8
{
	NoExistSkill = 9,RightMouseButton = 10, Q,E
};

UCLASS()
class UNREAL_PT_KYB_API UCSubAction_RightMouse : public UObject
{
	GENERATED_BODY()

public:
	UCSubAction_RightMouse();
	
public:
	virtual void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction);
	virtual void Tick(float InDeltaTime) {};
public:
	virtual ESubActionEnum GetInSubAction_SkillNumber() { return ESubActionEnum::NoExistSkill; }
	virtual FKey GetInSubAction_KeyNumber() { return EKeys::P; }
	FORCEINLINE bool GetInSubAction_RightMouse() { return bInAction; }

	virtual bool CanBeInCluster() const override { return false; }
	
public:
	virtual void Pressed();
	virtual void Released();
	FORCEINLINE UTexture2D* GetSkillImage() { return SkillTexture; }
	FORCEINLINE float GetMaxCoolTime() { return CoolTime; }
	FORCEINLINE FTimerHandle* GetHandler() { return &CoolTimehandler; }

public:
	FORCEINLINE virtual void StopSubAction();

protected:
	bool bInAction = false;

	UPROPERTY()
	class ACharacter* Owner;

	UPROPERTY()
	class ACAttachment* Attachment;

	UPROPERTY()
	class UCDoAction* DoAction;

	UPROPERTY()
	class UCStateComponent* State;

	UPROPERTY()
	class UCMovementComponent* Movement;

public:
	UPROPERTY(EditAnywhere)
	class UTexture2D* SkillTexture;

protected:
	UPROPERTY(EditAnywhere)
	float CoolTime = 0.0f;
	FTimerHandle CoolTimehandler;
};
