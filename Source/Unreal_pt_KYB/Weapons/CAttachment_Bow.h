#pragma once

#include "CoreMinimal.h"
#include "Weapons/CAttachment.h"
#include "CAttachment_Bow.generated.h"

UCLASS()
class UNREAL_PT_KYB_API ACAttachment_Bow : public ACAttachment
{
	GENERATED_BODY()


private:
	UPROPERTY(EditDefaultsOnly, Category = "View")
	FVector2D ViewPitchRange = FVector2D(-40, 30);

	UPROPERTY(VisibleAnywhere)
	class USkeletalMeshComponent* SkeletalMeshComponent;

	UPROPERTY(VisibleAnywhere)
	class UPoseableMeshComponent* PoseableMesh;

public:
	ACAttachment_Bow();

protected:
	void BeginPlay() override;

public:
	void OnBeginEquip_Implementation() override;

	void OnUnEquip_Implementation() override;

public:
	void Tick(float DeltaTime) override;

public:
	float* GetAniminstance_Bending();

private:
	FVector2D OriginViewPitchRange;
};
