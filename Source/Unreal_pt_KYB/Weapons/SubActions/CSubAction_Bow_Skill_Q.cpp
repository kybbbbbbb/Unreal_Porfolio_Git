#include "Weapons/SubActions/CSubAction_Bow_Skill_Q.h"
#include "Components/DecalComponent.h"
#include "GameFramework/PlayerController.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "../CAttachment.h"
#include "../CDoAction_Bow.h"

UCSubAction_Bow_Skill_Q::UCSubAction_Bow_Skill_Q()
{

}


void UCSubAction_Bow_Skill_Q::BeginPlay(ACharacter* InOwner, ACAttachment* InAttachment, UCDoAction* InDoAction)
{
	Super::BeginPlay(InOwner, InAttachment, InDoAction);

	PlayerController = Owner->GetController<APlayerController>();

	DecalComponent = Cast<UDecalComponent>(InAttachment->FindComponentByClass(UDecalComponent::StaticClass()));
	if (DecalComponent != nullptr)
		DecalComponent->SetVisibility(false);
}

void UCSubAction_Bow_Skill_Q::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	if (bInAction == false)
		return;


	FVector location = FVector::ZeroVector;
	FRotator rotation = FRotator::ZeroRotator;

	if (DecalComponent == nullptr)
		return;

	if (GetCursorLocationAndRotation(location, rotation) == false)
	{
		DecalComponent->SetVisibility(false);
		return;
	}

	DecalComponent->SetWorldLocationAndRotation(location, rotation);

	if(bDecalOn == true)
		DecalComponent->SetVisibility(true);

}

void UCSubAction_Bow_Skill_Q::Pressed()
{
	Super::Pressed();
	State->OnSubActionMode();

	//toggle
	UCDoAction_Bow* DoActionBow = Cast<UCDoAction_Bow>(DoAction);
	
	if (bInAction == true)
	{
		bInAction = false;
		State->OffSubActionMode();
		bDecalOn = false;
		DecalComponent->SetVisibility(false);
		
		if(DoActionBow != nullptr)
			DoActionBow->OffReload_Bow();
		
	}
	else
	{
		bInAction = true;
		bDecalOn = true;
		if (DoActionBow != nullptr)
			DoActionBow->OnReload_Bow();
	}
}


void UCSubAction_Bow_Skill_Q::Released()
{
	
	Super::Released();

}

	//@중요: 액션 시작 후 바로 제거되어야 하는 서브액션 요소들은 여기에서 실행
void UCSubAction_Bow_Skill_Q::StopSubAction()
{
	Super::StopSubAction();
	DecalComponent->SetVisibility(false);
	bInAction = false;
	State->OffSubActionMode();
}


	//@중요: 현재 화면 고정으로 대체, 추후 마우스 로케이션으로 값 구할시 활용
bool UCSubAction_Bow_Skill_Q::GetCursorLocationAndRotation(FVector& OutLocation, FRotator& OutRotation)
{
	if (PlayerController == nullptr)
	{
		return false;
	}


	FVector2D ScreenCenter = FVector2D(GEngine->GameViewport->Viewport->GetSizeXY() / 2);


	FHitResult hitResult;
	PlayerController->GetHitResultAtScreenPosition(ScreenCenter, ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	if (hitResult.bBlockingHit == false)
		return false;

	OutLocation = hitResult.Location; 
	OutRotation = hitResult.ImpactNormal.Rotation(); 

	return true; 



	//if (PlayerController == nullptr)
	//{
	//	return false;
	//}

	//FHitResult hitResult;
	//PlayerController->GetHitResultUnderCursorByChannel(ETraceTypeQuery::TraceTypeQuery1, false, hitResult);

	//if (hitResult.bBlockingHit == false)
	//	return false;

	//OutLocation = hitResult.Location;
	//OutRotation = hitResult.ImpactNormal.Rotation();
	//return true;
}
