#include "Components/CTargetComponent.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"
#include "CStateComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "../Characters/CMonsters_AI.h"
#include "Components/WidgetComponent.h"

UCTargetComponent::UCTargetComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
	OwnerCharacter = Cast<ACharacter>(GetOwner());
	TargetUI = CreateDefaultSubobject<UWidgetComponent>("Widget");
	TSubclassOf<UUserWidget> TargetUIclass;
	TargetUIclass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/BowWiget_1.BowWiget_1_C'").Class;
	TargetUI->SetWidgetClass(TargetUIclass);
	TargetUI->SetDrawSize(FVector2D(70, 70));
	TargetUI->SetWidgetSpace(EWidgetSpace::Screen);
}


void UCTargetComponent::BeginPlay()
{
	Super::BeginPlay();
	

	TargetUI->InitWidget();
	TargetUI->SetVisibility(false);
}


void UCTargetComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	if (Target == nullptr)
	{
		TargetUI->SetVisibility(false);
		return;
	}

	UCStateComponent* state = Cast<UCStateComponent>(Target->GetComponentByClass(UCStateComponent::StaticClass()));

	if (state->IsDeadMode() == true)
	{
		End();
		return;
	}

	FRotator controlRotation = OwnerCharacter->GetControlRotation();
	FRotator ownerToTarget = UKismetMathLibrary::FindLookAtRotation(OwnerCharacter->GetActorLocation(), Target->GetActorLocation());
	ownerToTarget.Pitch = controlRotation.Pitch;


	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();

	TargetUI->SetWorldLocation(Target->GetActorLocation());
	TargetUI->SetVisibility(true);

	if (controlRotation.Equals(ownerToTarget, FinishAngle))
	{
		controller->SetControlRotation(ownerToTarget);

		return;
	}


	FRotator targetRotation = FRotator(controlRotation.Pitch, ownerToTarget.Yaw, ownerToTarget.Roll);
	FRotator result = UKismetMathLibrary::RInterpTo(controlRotation, targetRotation, DeltaTime, 10.0f);

	controller->SetControlRotation(result);


}

void UCTargetComponent::Toggle()
{
	if (Target == nullptr)
	{
		Start();
		return;
	}
	End();

}


void UCTargetComponent::Start()
{
	FVector location = OwnerCharacter->GetActorLocation();

	TArray<AActor*> ignore;
	ignore.Add(OwnerCharacter);

	TArray<FHitResult> hitResult;
	UKismetSystemLibrary::SphereTraceMultiByProfile(GetWorld(), location, location, TraceDistance, "Targeting", false, ignore, DrawDebug, hitResult, true);

	if (hitResult.IsEmpty())
	{
		return;
	}

	ACharacter* target = GetNealyFrontAngle(hitResult);

	if (IsValid(target))
	{
		Change(target);
	}
	else
	{
		End();
	}
}


void UCTargetComponent::Change(ACharacter* InCandidate)
{
	if (InCandidate == nullptr)
	{
		End();
		return;
	}

	//if (IsValid(Particle))
	//{

	//	Particle->DestroyComponent();
	//	Particle = nullptr;
	//}

	if (InCandidate != nullptr)
	{
		//Particle = UGameplayStatics::SpawnEmitterAttached(particleAsset, InCandidate->GetMesh(), "Root", FVector(0, 0, -10), FRotator::ZeroRotator, EAttachLocation::KeepRelativeOffset);
		Target = InCandidate;

	}
	bExistTarget = true;

}

ACharacter* UCTargetComponent::GetNealyFrontAngle(const TArray<FHitResult>& InHitResult)
{
	float angle = 0.0f;
	ACharacter* candidate = nullptr;
	for (const FHitResult& result : InHitResult)
	{
		if (result.GetActor() == nullptr)
			continue;
		
		UCStateComponent* State = Cast<UCStateComponent>(result.GetActor()->GetComponentByClass(UCStateComponent::StaticClass()));
		if (State == nullptr)
			continue;
		if (State->IsDeadMode() == true)
			continue;
		ACMonsters_AI* monster = Cast<ACMonsters_AI>(result.GetActor());
		if (monster == nullptr)
			continue;
		if (monster->GetTeamID() == 1)
			continue;
		
		FVector targetLocation = result.GetActor()->GetActorLocation();
		FVector direction = targetLocation - OwnerCharacter->GetActorLocation();
		direction.Normalize();

		FRotator rotator = OwnerCharacter->GetControlRotation();
		FVector forward = FQuat(rotator).GetForwardVector();

		float dot = FVector::DotProduct(direction, forward);

		if (dot >= angle)
		{
			angle = dot;
			candidate = Cast<ACharacter>(result.GetActor());
		}
	}
	return candidate;
}


void UCTargetComponent::End()
{
	Target = nullptr;
	bExistTarget = false;
	//if (!!Particle)
	//	Particle->DestroyComponent();
}

