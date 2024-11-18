#include "Weapons/SubActions/CSubAction_RightMouse_Bow.h"
#include "GameFramework/Character.h"
#include "Characters/CAnimInstance.h"
#include "GameFramework/SpringArmComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CStateComponent.h"
#include "Curves/CurveVector.h"
#include "../CAttachment_Bow.h"
#include "../CDoAction_Bow.h"
#include "DrawDebugHelpers.h"
#include "Kismet/GameplayStatics.h"
#include "GameManager/CCameraManagerComponent.h"


UCSubAction_RightMouse_Bow::UCSubAction_RightMouse_Bow()
{
	curve_View_Bending_Arm = ConstructorHelpers::FObjectFinder<UCurveVector>(L"/Script/Engine.CurveVector'/Game/Weapon/Curve_Aiming.Curve_Aiming'").Object;
	curve_socketOffset = ConstructorHelpers::FObjectFinder<UCurveVector>(L"/Script/Engine.CurveVector'/Game/Weapon/Curve_Aiming_socketOffset.Curve_Aiming_socketOffset'").Object;
	Mesh = ConstructorHelpers::FObjectFinder<UStaticMesh>(L"/Script/Engine.StaticMesh'/Game/05_Material/SplineCylinder1.SplineCylinder1'").Object;
	
}
void UCSubAction_RightMouse_Bow::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);
	SpringArm = Cast<USpringArmComponent>(InOwner->GetComponentByClass(USpringArmComponent::StaticClass()));
	Camera = Cast<UCameraComponent>(InOwner->GetComponentByClass(UCameraComponent::StaticClass()));


	FOnTimelineVector timeline1;
	FOnTimelineVector timeline2;
	timeline1.BindUFunction(this, "OnAiming_Camera");
	timeline2.BindUFunction(this, "OnAiming_socket");
	Timeline.AddInterpVector(curve_View_Bending_Arm,timeline1);
	Timeline.AddInterpVector(curve_socketOffset,timeline2);
	Timeline.SetPlayRate(200.0f);


	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	if (!!bow)
		Bending = bow->GetAniminstance_Bending();

	LastAimSphere = Cast<UStaticMeshComponent>(bow->GetComponentByClass(UStaticMeshComponent::StaticClass()));
	LastAimSphere->SetVisibility(false);
}


void UCSubAction_RightMouse_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);
	Timeline.TickTimeline(InDeltaTime);

	if (bInAction == false)
		return;


	UCDoAction_Bow* doaction_bow = Cast<UCDoAction_Bow>(DoAction);
	if (doaction_bow != nullptr)
	{
		ACArrow* arrow = doaction_bow->GetAttachedArrow();
		if (arrow != nullptr)
		{
			FPredictProjectilePathParams PredictParams;

			PredictParams.StartLocation = arrow->GetActorLocation();
			PredictParams.LaunchVelocity = arrow->GetActorUpVector()*5000.0;
			PredictParams.bTraceWithCollision = true;
			PredictParams.TraceChannel = ECollisionChannel::ECC_Pawn;
			PredictParams.ProjectileRadius = 3.0f;
			PredictParams.MaxSimTime = 1.0f;
			PredictParams.OverrideGravityZ = Owner->GetWorld()->GetGravityZ();
			PredictParams.SimFrequency = 50.0f;
			PredictParams.DrawDebugType = EDrawDebugTrace::None;
			
			/*if (AimSpline == nullptr)
				return;*/


			FPredictProjectilePathResult PredictResult;
			bool bsucceed = UGameplayStatics::PredictProjectilePath(arrow, PredictParams, PredictResult);
		
			TArray<FPredictProjectilePathPointData> Points = PredictResult.PathData;
			//AimSpline->ClearSplinePoints();

		
			
			if(doaction_bow->GetisReload() == true)
				UpdateSplineMesh(Points);


			LastAimSphere->SetWorldLocation(Points.Last().Location + FVector(0,0,10));
			LastAimSphere->SetVisibility(true);
		}

	}


}


void UCSubAction_RightMouse_Bow::OnAiming_Camera(FVector Output)
{
	Camera->FieldOfView = Output.X;
	if (!!Bending)
		*Bending = Output.Y;
	SpringArm->TargetArmLength = Output.Z;
}


void UCSubAction_RightMouse_Bow::OnAiming_socket(FVector Output)
{
	SpringArm->SocketOffset = Output;

}


void UCSubAction_RightMouse_Bow::Pressed()
{
	bInAction = true;
	if (SpringArm == nullptr)
		return;
	if (Camera == nullptr)
		return;
	if (State->IsActionMode())
		return;
	Super::Pressed();
	State->OnSubActionMode();
	UCDoAction_Bow* doaction_bow = Cast<UCDoAction_Bow>(DoAction);
	if (doaction_bow != nullptr)
	{
		doaction_bow->OffReload_Bow();
	}


	UCAnimInstance* animinstance = Cast<UCAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	animinstance->ChangedAiming(true);




	if(AimStartAnim != nullptr)
		Owner->PlayAnimMontage(AimStartAnim);

	OriginData.bEnableCameraLag = SpringArm->bEnableCameraLag;
	OriginData.CameraLocation = Camera->GetRelativeLocation();

	SpringArm->bEnableCameraLag = AimData.bEnableCameraLag;
	Camera->SetRelativeLocation(AimData.CameraLocation);

	UCCameraManagerComponent* CameraComp = Owner->FindComponentByClass<UCCameraManagerComponent>();
	CameraComp->SetbCameraTick(true);

	Timeline.PlayFromStart();
}

void UCSubAction_RightMouse_Bow::Released()
{
	if (SpringArm == nullptr)
		return;
	if (Camera == nullptr)
		return;

	if (State->IsSubActionMode() == false)
		return;

	UCDoAction_Bow* doaction_bow = Cast<UCDoAction_Bow>(DoAction);
	if (doaction_bow != nullptr)
	{
		ACArrow* arrow = doaction_bow->GetAttachedArrow();
		if (arrow != nullptr)
			arrow->OffVisibility();
		doaction_bow->OffAttachString();
		doaction_bow->OffReload_Bow();
	}

	UCAnimInstance* animinstance = Cast<UCAnimInstance>(Owner->GetMesh()->GetAnimInstance());
	animinstance->ChangedAiming(false);


	SpringArm->bEnableCameraLag = OriginData.bEnableCameraLag;
	Camera->SetRelativeLocation(OriginData.CameraLocation);


	Timeline.ReverseFromEnd();
	bInAction = false;
	State->OffSubActionMode();

	while (splines.Num() > 0)
	{
		USplineMeshComponent* SplineMesh = splines.Last();
		SplineMesh->DestroyComponent();
		splines.RemoveAt(splines.Num() - 1);
	}
	LastAimSphere->SetVisibility(false);

	UCCameraManagerComponent* CameraComp = Owner->FindComponentByClass<UCCameraManagerComponent>();
	CameraComp->SetbCameraTick(false);
	CameraComp->DefaultCameraChange(ECameraEnum::DefaultStandard);
	Super::Released();

}

void UCSubAction_RightMouse_Bow::StopSubAction()
{
	Super::StopSubAction();

	while (splines.Num() > 0)
	{
		USplineMeshComponent* SplineMesh = splines.Last();
		SplineMesh->DestroyComponent();
		splines.RemoveAt(splines.Num() - 1);
	}
	LastAimSphere->SetVisibility(false);
}

void UCSubAction_RightMouse_Bow::UpdateSplineMesh(const TArray<FPredictProjectilePathPointData>& Points)
{
	if (splines.Num() < Points.Num() - 1)
	{
		for (int32 i = splines.Num(); i < Points.Num() - 1; ++i)
		{
			USplineMeshComponent* SplineMesh = NewObject<USplineMeshComponent>(Owner);
			splines.Add(SplineMesh);
			SplineMesh->SetStaticMesh(Mesh);  // 메쉬 설정
			SplineMesh->SetMobility(EComponentMobility::Movable);
			SplineMesh->RegisterComponentWithWorld(Owner->GetWorld());
		}
	}

	for (int32 i = 0; i < Points.Num() - 1; ++i)
	{
		USplineMeshComponent* SplineMesh = splines[i];

		FVector StartLocation = Points[i].Location + FVector(0,0,-2.0f);
		FVector EndLocation = Points[i + 1].Location + FVector(0, 0, -2.0f);

		
		FVector StartTangent = (i > 0) ? (EndLocation - Points[i - 1].Location).GetSafeNormal() * 100.0f : (EndLocation - StartLocation).GetSafeNormal() * 100.0f;
		FVector EndTangent = (i < Points.Num() - 2) ? (Points[i + 2].Location - StartLocation).GetSafeNormal() * 100.0f : (EndLocation - StartLocation).GetSafeNormal() * 100.0f;

		// 스플라인 메쉬의 시작과 끝을 설정 (위치, 탄젠트)
		SplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent, true);
	}

	// 남은 스플라인 메쉬 컴포넌트 제거 (만약 필요 이상의 컴포넌트가 있을 경우)
	while (splines.Num() > Points.Num() - 1)
	{
		USplineMeshComponent* SplineMesh = splines.Last();
		SplineMesh->DestroyComponent();
		splines.RemoveAt(splines.Num() - 1);
	}

}
