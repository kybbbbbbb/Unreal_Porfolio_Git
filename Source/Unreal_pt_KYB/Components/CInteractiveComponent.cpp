#include "Components/CInteractiveComponent.h"
#include "GameFramework/Character.h"
#include "../Interface/CInteractableObject.h"

UCInteractiveComponent::UCInteractiveComponent()
{
	PrimaryComponentTick.bCanEverTick = true;
}


void UCInteractiveComponent::BeginPlay()
{
	Super::BeginPlay();

	OwnerCharacter = Cast<ACharacter>(GetOwner());
	PlayerController = OwnerCharacter->GetController<APlayerController>();
}


void UCInteractiveComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (!PlayerController) return;

	FVector2D ScreenLocation(GEngine->GameViewport->Viewport->GetSizeXY() / 2);

	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);

	FVector Start = WorldLocation;
	FVector End = Start + (WorldDirection * InteractionDistance);


	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());


	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);


	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, Params);

	AActor* hit = HitResult.GetActor();

	//히트된 객체가 없을때
	if(hit == nullptr)
	{
		//아웃라인
		if (OnOutlineResult.Num() < 1)
			return;

		for (UPrimitiveComponent* Outlinecomponents : OnOutlineResult)
		{
			Outlinecomponents->SetRenderCustomDepth(false);
		}
		OnOutlineResult.Empty();
		return;
	}

	//인터렉티브 오브젝트가 아닐 때의 로직
	ICInteractableObject* object = Cast<ICInteractableObject>(hit);
	if (object == nullptr)
	{
		if (OnOutlineResult.Num() < 1)
			return;

		for (UPrimitiveComponent* Outlinecomponents : OnOutlineResult)
		{
			Outlinecomponents->SetRenderCustomDepth(false);
		}
		OnOutlineResult.Empty();
		return;
	}


	TArray<UPrimitiveComponent*> results;
	hit->GetComponents(UPrimitiveComponent::StaticClass(), results);
	

	if (results.Num() > 0)
	{
		for (UPrimitiveComponent* result : results)
		{
			result->SetRenderCustomDepth(true);
			OnOutlineResult.Add(result);
		}
	}
	else
	{
		for (UPrimitiveComponent* Outlinecomponents : OnOutlineResult)
		{
			Outlinecomponents->SetRenderCustomDepth(false);
		}
		OnOutlineResult.Empty();
	}
}

void UCInteractiveComponent::IsRiding(bool Inbool)
{
	isRiding = Inbool;
}

void UCInteractiveComponent::CheckInteractive()
{
	if (!PlayerController) return;
	if (isRiding == true)
	{
		return;
	}
	FVector2D ScreenLocation(GEngine->GameViewport->Viewport->GetSizeXY() / 2);


	FVector WorldLocation, WorldDirection;
	PlayerController->DeprojectScreenPositionToWorld(ScreenLocation.X, ScreenLocation.Y, WorldLocation, WorldDirection);

	// 레이 디버그용
	FVector Start = WorldLocation;
	FVector End = Start + (WorldDirection * InteractionDistance);
	//DrawDebugLine(GetWorld(), Start, End, FColor::Green, false, 1, 0, 1);


	// 히트 결과를 얻습니다.
	FHitResult HitResult;
	FCollisionQueryParams Params;
	Params.AddIgnoredActor(GetOwner());

	FCollisionObjectQueryParams ObjectQueryParams;
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldStatic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_WorldDynamic);
	ObjectQueryParams.AddObjectTypesToQuery(ECC_Pawn);

	bool bHit = GetWorld()->LineTraceSingleByObjectType(HitResult, Start, End, ObjectQueryParams, Params);
	
	AActor* hit = HitResult.GetActor();
	if (hit == nullptr)
		return;

	ICInteractableObject* interaciveObject = Cast<ICInteractableObject>(hit);

	if (interaciveObject != nullptr)
	{
		interaciveObject->Interact();
	}
}

