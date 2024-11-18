#include "Notify/CAnimNotify_FootStep.h"
#include "../GameManager/CSoundManager.h"

FString UCAnimNotify_FootStep::GetNotifyName_Implementation() const
{
	return "FootStep";
}

void UCAnimNotify_FootStep::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{

	if (MeshComp == nullptr)
		return;
	if (MeshComp->GetOwner() == nullptr)
		return;
	AActor* actor = MeshComp->GetOwner();
	FVector location = actor->GetActorLocation() + FVector(0,0,50);
	

	FName SurfaceType = GetSurfaceType(location, actor);

	FName SoundName;

	if (SurfaceType == "Concrete")
		SoundName = "SF_Concrete";
	else if (SurfaceType == "Mud")
		SoundName = "SF_Mud";

	UWorld* world = MeshComp->GetOwner()->GetWorld();
	UCSoundManager::GetInstance()->PlaySound2D(SoundName, world);
}

FName UCAnimNotify_FootStep::GetSurfaceType(const FVector& Location, const AActor* Actor)
{
	FHitResult HitResult;
	FVector Start = Location;
	FVector End = Start - FVector(0, 0, 300.0f);

	FCollisionQueryParams QueryParams;
	QueryParams.bReturnPhysicalMaterial = true;
	QueryParams.AddIgnoredActor(Actor);

	bool bHit = Actor->GetWorld()->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, QueryParams);



	if (bHit)
	{
		if (HitResult.GetActor())
		{
	
		}
		if (HitResult.PhysMaterial.IsValid())
		{
			EPhysicalSurface SurfaceType = HitResult.PhysMaterial->SurfaceType;



			switch (SurfaceType)
			{
			case SurfaceType1:
				return "Concrete";
			case SurfaceType2:
				return "Mud";
			default:
				break;
			}
		}
		
	}
	
	return "Concrete"; 
}
