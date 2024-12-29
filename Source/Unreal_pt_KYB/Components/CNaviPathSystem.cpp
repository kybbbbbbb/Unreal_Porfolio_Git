#include "Components/CNaviPathSystem.h"
#include "Components/SplineComponent.h"
#include "NavigationSystem.h"
#include "GameFramework/Character.h"
#include "NavigationPath.h"
#include "NavFilters/NavigationQueryFilter.h"
#include "Components/SplineMeshComponent.h"
#include "Kismet/KismetMathLibrary.h"

UCNaviPathSystem::UCNaviPathSystem()
{
	PrimaryComponentTick.bCanEverTick = true;
	owner = Cast<ACharacter>(GetOwner());
	SplineComponent = CreateDefaultSubobject<USplineComponent>("spline");
	

}


void UCNaviPathSystem::BeginPlay()
{
	Super::BeginPlay();
}


void UCNaviPathSystem::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
	if (bNavOn == false)
		return;
	if (targetLocation == FVector::ZeroVector)
		return;

	double distance = UKismetMathLibrary::Vector_Distance(owner->GetActorLocation(), targetLocation);
	if (distance < 200.0f)
	{
		targetLocation = FVector::ZeroVector;
		EndNav();
	}

	// 1�ʸ��� ��� ������Ʈ
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastUpdateTime >= 1.0f)
	{
		LastUpdateTime = CurrentTime;
		PlayNavigation(targetLocation); // ��� ��Ž��
	}
}

void UCNaviPathSystem::PlayNavigation(FVector Location)
{
	UNavigationSystemV1* Navsystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (owner == nullptr)
		return;
	if (Navsystem == nullptr)
		return;

	FVector startLocation = owner->GetActorLocation();


	// �⺻ ���� ���� Ŭ���� ���
	TSubclassOf<UNavigationQueryFilter> QueryFilterClass = UNavigationQueryFilter::StaticClass();
	FSharedConstNavQueryFilter QueryFilter = 
		UNavigationQueryFilter::GetQueryFilter(*Navsystem->GetDefaultNavDataInstance(), owner, QueryFilterClass);
	
	Query.StartLocation = owner->GetNavAgentLocation();
	Query.EndLocation = Location;
	Query.NavAgentProperties = owner->GetNavAgentPropertiesRef();
	Query.SetAllowPartialPaths(true);
	Query.NavData = Navsystem->GetDefaultNavDataInstance();
	Query.Owner = owner;
	Query.QueryFilter = QueryFilter;
	
	//�񵿱� ����
	uint32 ID = Navsystem->FindPathAsync(owner->GetNavAgentPropertiesRef(), Query, FNavPathQueryDelegate::CreateUObject(this, &UCNaviPathSystem::OnPathFoundAsync));
}

void UCNaviPathSystem::NaviOn()
{
	if (bNavOn == true)
	{
		bNavOn = false;
		EndNav();
	}
	else
	{
		if (targetLocation != FVector::ZeroVector)
		{
			bNavOn = true;
			PlayNavigation(targetLocation);
		}
	}

}

void UCNaviPathSystem::OnPathFoundAsync(uint32 QueryID, ENavigationQueryResult::Type Result, FNavPathSharedPtr Path)
{
	if (Path.IsValid())
	{
		if (Path != nullptr)
		{
			//���� ���ö��� �ִٸ� ����-------------------------------------
			TArray<FNavPathPoint> pathPoints = Path->GetPathPoints();

			SplineComponent->ClearSplinePoints();
			for (USplineMeshComponent* mesh : SplineMesh)
			{
				if (mesh)
				{
					mesh->DestroyComponent();
				}
			}
			SplineMesh.Empty();
			


			//���ö��� ����Ʈ �߰��� Ŀ�����--------------------------------
			//(�߰�)���� �����̰� �����Ÿ� ���ϸ� ����(���� ���� �ذ�)
			const float MergeLength = 150.0;
			TArray<FVector> FilteredPoints;
			if (pathPoints.Num() > 0)
			{
				FVector CurrentPoint = pathPoints[0].Location; // ù ��° ���� ������ �߰�
				FilteredPoints.Add(CurrentPoint);

				for (int32 i = 1; i < pathPoints.Num(); ++i)
				{
					FVector NextPoint = pathPoints[i].Location;
					float Distance = FVector::Dist(CurrentPoint, NextPoint);

					if (Distance < MergeLength)
					{
						// �� ���� �߰� �� ���
						FVector MidPoint = (CurrentPoint + NextPoint) / 2.0f;

						// ���� ���յ� ���� �߰� ������ ��ü
						FilteredPoints.Last() = MidPoint;
						CurrentPoint = MidPoint; // �߰� ���� �������� ����
					}
					else
					{
						FilteredPoints.Add(NextPoint);
						CurrentPoint = NextPoint;
					}
				}
			}


			// ���յ� ����� ���ö��� ����
			for (int32 i = 0; i < FilteredPoints.Num(); ++i)
			{
				SplineComponent->AddSplinePoint(FilteredPoints[i], ESplineCoordinateSpace::World);

				if (i == 0 || i == FilteredPoints.Num() - 1)
				{
					SplineComponent->SetSplinePointType(i, ESplinePointType::Linear);
				}
				else
				{
					SplineComponent->SetSplinePointType(i, ESplinePointType::CurveClamped);
				}
			}


			//ź��Ʈ�� �̿��� � ����
			for (int32 i = 0; i < FilteredPoints.Num(); ++i)
			{
				FVector CurrentPoint = FilteredPoints[i];
				FVector PreviousPoint = (i > 0) ? FilteredPoints[i - 1] : CurrentPoint;
				FVector NextPoint = (i < FilteredPoints.Num() - 1) ? FilteredPoints[i + 1] : CurrentPoint;

				FVector Tangent;
				float Distance;

				
				if (i > 0 && i < FilteredPoints.Num() - 1)
				{
					Tangent = (NextPoint - PreviousPoint) / 2.0f;
				}
				else if (i == 0) 
				{
					Tangent = NextPoint - CurrentPoint;
				}
				else
				{
					Tangent = CurrentPoint - PreviousPoint;
				}

				Distance = Tangent.Size();
				const float MaxTangentScale = 200.0f; // ź��Ʈ ũ�� ����
				if (Distance > MaxTangentScale)
				{
					Tangent = Tangent.GetSafeNormal() * MaxTangentScale;
				}

				if (i > 0)
				{
					SplineComponent->SetTangentAtSplinePoint(i - 1, Tangent, ESplineCoordinateSpace::World);
				}
			}



			// ���ö��� �޽� ����-----------------------------------------
			for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
			{
				FVector StartLocation, StartTangent;
				FVector EndLocation, EndTangent;

				// ���� �� �� ������ ��ġ�� ź��Ʈ ��������
				SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
				SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

				// ���ö��� �޽� ������Ʈ ����
				USplineMeshComponent* newSplineMesh = NewObject<USplineMeshComponent>(this);
				if (newSplineMesh)
				{
					newSplineMesh->SetStaticMesh(SplineMeshAsset);
					newSplineMesh->SetForwardAxis(ESplineMeshAxis::Z);
					newSplineMesh->SetStartAndEnd(StartLocation, StartTangent, EndLocation, EndTangent);
					newSplineMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
					newSplineMesh->RegisterComponentWithWorld(GetWorld());
					
					SplineMesh.Add(newSplineMesh);
				}
			}
		}
	}
}




//����� ��ã�� ����
//void UCNaviPathSystem::OnFindPathToLocationSynchronously(FVector targetLocation)
//{
//	UNavigationSystemV1* Navsystem = UNavigationSystemV1::GetCurrent(GetWorld());
//	if (Navsystem == nullptr)
//		return;
//
//	FVector startLocation = owner->GetActorLocation();
//
//
//	UNavigationPath* Path = Navsystem->FindPathToLocationSynchronously(owner->GetWorld(), startLocation, targetLocation);
//	if (Path != nullptr)
//	{
//		TArray<FVector> pathPoints = Path->PathPoints;
//
//		SplineComponent->ClearSplinePoints();
//		for (const FVector& Point : pathPoints)
//		{
//			SplineComponent->AddSplinePoint(Point, ESplineCoordinateSpace::World);
//		}
//		SplineComponent->UpdateSpline();
//
//		// ��θ� ��������� ���
//		for (int32 i = 0; i < pathPoints.Num() - 1; i++)
//		{
//			DrawDebugLine(
//				GetWorld(),
//				pathPoints[i],
//				pathPoints[i + 1],
//				FColor::Green,
//				false,
//				5.0f,
//				0,
//				5.0f
//			);
//		}
//		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Blue, TEXT("Navi In3"));
//	}
//	else
//	{
//		GEngine->AddOnScreenDebugMessage(-1, 5, FColor::Red, TEXT("Navi path finding failed or no valid path!"));
//	}
//
//}

void UCNaviPathSystem::EndNav()
{
	SplineComponent->ClearSplinePoints();
	for (USplineMeshComponent* mesh : SplineMesh)
	{
		if (mesh)
		{
			mesh->DestroyComponent();
		}
	}
	SplineMesh.Empty();
}



