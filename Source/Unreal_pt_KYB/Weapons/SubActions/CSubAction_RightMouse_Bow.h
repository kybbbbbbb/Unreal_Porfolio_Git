#pragma once

#include "CoreMinimal.h"
#include "Weapons/CSubAction_RightMouse.h"
#include "Components/TimelineComponent.h"
#include "Components/SplineComponent.h"
#include "Kismet/GameplayStaticsTypes.h"
#include "Components/SplineMeshComponent.h"

#include "CSubAction_RightMouse_Bow.generated.h"

USTRUCT()
struct FAimData
{
	GENERATED_BODY()

public:
	UPROPERTY()
	bool bEnableCameraLag;

	UPROPERTY()
	FVector CameraLocation;


};

UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCSubAction_RightMouse_Bow : public UCSubAction_RightMouse
{
	GENERATED_BODY()

public:
	UCSubAction_RightMouse_Bow();

private:
	FKey KeyName = EKeys::RightMouseButton;
	ESubActionEnum SkillNumber = ESubActionEnum::RightMouseButton;

public:
	void ChangedKey();
	FORCEINLINE FKey GetInSubAction_KeyNumber() override 
	{
		return KeyName; 
	}
	FORCEINLINE ESubActionEnum GetInSubAction_SkillNumber() override 
	{
		return SkillNumber;
	}
	

public:
	void BeginPlay(class ACharacter* InOwner, class ACAttachment* InAttachment, class UCDoAction* InDoAction) override;
	void Tick(float InDeltaTime) override;
private:
	UPROPERTY()
	class USpringArmComponent* SpringArm;
	UPROPERTY()
	class UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	UCurveVector* curve_View_Bending_Arm;

	UPROPERTY(EditAnywhere)
	UCurveVector* curve_socketOffset;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AimStartAnim;

	UPROPERTY()
	UStaticMesh* Mesh;

	UPROPERTY()
	UStaticMeshComponent* LastAimSphere;


private:
	UPROPERTY(EditAnywhere)
	FAimData AimData;

	UFUNCTION()
	void OnAiming_Camera(FVector Output);
	UFUNCTION()
	void OnAiming_socket(FVector Output);
public:
	void Pressed() override;
	void Released() override;
	void StopSubAction() override;

	bool CanBeInCluster() const override { return false; }

private:
	void UpdateSplineMesh(const TArray<FPredictProjectilePathPointData>& Points);

private:
	FAimData OriginData;
	UPROPERTY()
	TArray<USplineMeshComponent*> splines;
private:
	FTimeline Timeline;

private:
	float* Bending;
	
};
