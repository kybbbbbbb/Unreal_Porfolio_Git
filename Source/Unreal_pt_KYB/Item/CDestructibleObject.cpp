#include "Item/CDestructibleObject.h"
#include "CPortion.h"
#include "../GameManager/CSoundManager.h"

ACDestructibleObject::ACDestructibleObject()
{
	Health = 15.0f;
	bIsDestroyed = false;

	GeometryCollection = CreateDefaultSubobject<UGeometryCollectionComponent>("Geometryobject");
	GeometryCollection->SetSimulatePhysics(true);
}

void ACDestructibleObject::BeginPlay()
{
	Super::BeginPlay();
	TArray<float> damaged;
	damaged.Add(500000000.0f);
	damaged.Add(500000000.0f);
	damaged.Add(500000000.0f);
	GeometryCollection->SetDamageThreshold(damaged);

	
}

void ACDestructibleObject::BreakObject(FVector HitLocation, FVector ImpulseDir)
{
	UCSoundManager* SoundManager = UCSoundManager::GetInstance();


	if (bIsDestroyed) return; // �̹� �ı��� ��� ���� �ߴ�
	Health -= 5.0f;

	if (Health <= 0.0f)
	{
		GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Ignore);
		GeometryCollection->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
		GeometryCollection->OnActorEnableCollisionChanged();

		bIsDestroyed = true;

		if (GeometryCollection)
		{


			if (SoundManager != nullptr)
			{
				SoundManager->PlaySound3D("SI_WoodBoxBroken", GetActorLocation(), GetWorld());
			}
			TArray<float> damaged;
			damaged.Add(10.0f);
			GeometryCollection->SetDamageThreshold(damaged);
			
			FVector CenterLocation = GetActorLocation();

			float Radius = 1500.0f; // ���޽��� �ݰ�
			float Strength = 300.0f; // ���޽��� ����

			// ����� ���޽��� ����
			GeometryCollection->AddRadialImpulse(CenterLocation+FVector(0,0,50), Radius, Strength,
				ERadialImpulseFalloff::RIF_Linear, true);

			DropItem();
			return;
		}
	}

	if (SoundManager != nullptr)
	{
		SoundManager->PlaySound3D("SI_WoodBoxHit", GetActorLocation(), GetWorld());
	}
	ChangeColor();
}

//������ ����
void ACDestructibleObject::DropItem()
{
	if (DropPortionItem.Num() > 0)
	{
		
		int32 randomportionitem = FMath::RandRange(0, DropPortionItem.Num() - 1);

		ACPortion* spawnitem = GetWorld()->SpawnActor<ACPortion>(DropPortionItem[randomportionitem]->PortionData.portionBP, GetActorLocation(), GetActorRotation());
		spawnitem->SetitemID(DropPortionItem[randomportionitem]->GetPrimaryAssetId());

		UCSoundManager* SoundManager = UCSoundManager::GetInstance();
		if (SoundManager != nullptr)
		{
			SoundManager->PlaySound3D("SU_Quest", GetActorLocation(), GetWorld());
		}

		UStaticMeshComponent* MeshComponent = Cast<UStaticMeshComponent>(spawnitem->GetComponentByClass(UStaticMeshComponent::StaticClass()));
		if (MeshComponent)
		{
			MeshComponent->SetSimulatePhysics(true);
			MeshComponent->SetEnableGravity(true);

			// ���� �������� ���޽� ����
			FVector Impulse = FVector(0, 0, 5000.0f); 
			MeshComponent->AddImpulseAtLocation(Impulse, GetActorLocation());


		}
	}
}

void ACDestructibleObject::ChangeColor()
{
	if (!GeometryCollection) return;

	UMaterialInstanceDynamic* DynamicMaterial = GeometryCollection->CreateAndSetMaterialInstanceDynamic(0);
	if (DynamicMaterial)
	{
		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, FString::Printf(TEXT("%d"), 999));
		FLinearColor OriginalColor;
		DynamicMaterial->GetVectorParameterValue(FName("Albedo Tint"), OriginalColor);

		DynamicMaterial->SetVectorParameterValue(FName("Albedo Tint"), FLinearColor::Red);
		
		// ���� �ð� �� ���� �������� ����
		FTimerHandle TimerHandle;
		GetWorld()->GetTimerManager().SetTimer(TimerHandle, [DynamicMaterial, OriginalColor]()
			{
				DynamicMaterial->SetVectorParameterValue(FName("Albedo Tint"), OriginalColor);
			}, 0.1f, false);
	}
}




