#include "Components/CFootIKComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/KismetMathLibrary.h"

UCFootIKComponent::UCFootIKComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	if (OwnerCharacter != nullptr)
	{
		Skeletal = OwnerCharacter->GetMesh();
	}
}


void UCFootIKComponent::BeginPlay()
{
	Super::BeginPlay();
	AnimBP = Cast<UCAnimInstance>(Skeletal->GetAnimInstance());
	originSkeletalLocation = Skeletal->GetRelativeLocation();
}



void UCFootIKComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (isRiding == true)
		return;
	if (AnimBP == nullptr)
		return;
	if (AnimBP->GetIsJump())
		return;

	FootTrace();


	SendAnimBluprint();
}

void UCFootIKComponent::IsRiding(bool InBool)
{
	isRiding = InBool;
}



void UCFootIKComponent::FootTrace()
{
	FVector LFootLocation = Skeletal->GetSocketLocation(LFootSocketName);
	FVector RFootLocation = Skeletal->GetSocketLocation(RFootSocketName);

	FVector LFstart = FVector(LFootLocation.X, LFootLocation.Y, Skeletal->GetSocketLocation("root").Z + 50);
	FVector LFend = FVector(LFootLocation.X, LFootLocation.Y, Skeletal->GetSocketLocation("root").Z - 80);

	FVector RFstart = FVector(RFootLocation.X, RFootLocation.Y, Skeletal->GetSocketLocation("root").Z + 50);
	FVector RFend = FVector(RFootLocation.X, RFootLocation.Y, Skeletal->GetSocketLocation("root").Z - 80);

	TArray<AActor*> ignore;
	ignore.Add(OwnerCharacter);
	FHitResult LFresult;
	FHitResult RFresult;

	bool bLFresult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), LFstart, LFend, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignore, EDrawDebugTrace::ForOneFrame, LFresult, true);
	bool bRFresult = UKismetSystemLibrary::LineTraceSingle(GetWorld(), RFstart, RFend, UEngineTypes::ConvertToTraceType(ECC_Visibility), true, ignore, EDrawDebugTrace::ForOneFrame, RFresult, true);

	float Loffset = Skeletal->GetSocketLocation("root").Z;

	if (bLFresult)
	{
		//왼발 위치 구하기
		Loffset = (LFresult.ImpactPoint.Z - Skeletal->GetSocketLocation("root").Z);
		LFootIKoffset = FMath::FInterpTo(LFootIKoffset, Loffset, GetWorld()->GetDeltaSeconds(), IKSpeed);
		
		//왼발 회전각 구하기
		FRotator LeftFootRot = NormalToRotator(LFresult.ImpactNormal, "LFootIKRocation");
		LFootIKRotator = FMath::RInterpTo(LFootIKRotator, LeftFootRot, GetWorld()->GetDeltaSeconds(), 30);
	}
	else
	{
		LFootIKoffset = FMath::FInterpTo(LFootIKoffset, 0.0f, GetWorld()->GetDeltaSeconds(), IKSpeed);
	}

	float Roffset = Skeletal->GetSocketLocation("root").Z;
	if (bRFresult)
	{
		//오른발 위치 구하기
		Roffset = (RFresult.ImpactPoint.Z- Skeletal->GetSocketLocation("root").Z);
		RFootIKoffset = FMath::FInterpTo(RFootIKoffset, Roffset, GetWorld()->GetDeltaSeconds(), IKSpeed);

		//오른발 회전각 구하기
		FRotator RightFootRot = NormalToRotator(RFresult.ImpactNormal, "RFootIKRocation");
		RFootIKRotator = FMath::RInterpTo(RFootIKRotator, RightFootRot, GetWorld()->GetDeltaSeconds(), 30);
	}
	else
	{
		RFootIKoffset = FMath::FInterpTo(RFootIKoffset, 0.0f, GetWorld()->GetDeltaSeconds(), IKSpeed);
	}

	float pelvisOffset = FMath::Min(Loffset, Roffset);
	pelvisIKoffset = FMath::FInterpTo(pelvisIKoffset, pelvisOffset, GetWorld()->GetDeltaSeconds(), IKSpeed);
}

void UCFootIKComponent::SendAnimBluprint()
{
	
	AnimBP->SetLFootIK(LFootIKoffset, RFootIKoffset, pelvisIKoffset, LFootIKRotator, RFootIKRotator);
	
}

FRotator UCFootIKComponent::NormalToRotator(FVector NormalVector, FName boneName)
{
	FTransform BoneTransform = Skeletal->GetSocketTransform(boneName, ERelativeTransformSpace::RTS_World);
    FQuat BoneQuat = BoneTransform.GetRotation();

    FVector LocalNormal = BoneQuat.UnrotateVector(NormalVector);

    float Roll = UKismetMathLibrary::DegAtan2(LocalNormal.Y, LocalNormal.Z);
    float Pitch = -UKismetMathLibrary::DegAtan2(LocalNormal.X, LocalNormal.Z);

    FRotator pResult = FRotator(Pitch, 0.0f, Roll);


	return pResult;
}

