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

	// 1초마다 경로 업데이트
	float CurrentTime = GetWorld()->GetTimeSeconds();
	if (CurrentTime - LastUpdateTime >= 1.0f)
	{
		LastUpdateTime = CurrentTime;
		PlayNavigation(targetLocation); // 경로 재탐색
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


	// 기본 쿼리 필터 클래스 사용
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
	
	//비동기 실행
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
			//기존 스플라인 있다면 삭제-------------------------------------
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
			


			//스플라인 포인트 추가와 커브셋팅--------------------------------
			//(추가)점과 점사이가 일정거리 이하면 병합(꺾임 문제 해결)
			const float MergeLength = 150.0;
			TArray<FVector> FilteredPoints;
			if (pathPoints.Num() > 0)
			{
				FVector CurrentPoint = pathPoints[0].Location; // 첫 번째 점은 무조건 추가
				FilteredPoints.Add(CurrentPoint);

				for (int32 i = 1; i < pathPoints.Num(); ++i)
				{
					FVector NextPoint = pathPoints[i].Location;
					float Distance = FVector::Dist(CurrentPoint, NextPoint);

					if (Distance < MergeLength)
					{
						// 두 점의 중간 점 계산
						FVector MidPoint = (CurrentPoint + NextPoint) / 2.0f;

						// 현재 병합된 점을 중간 점으로 대체
						FilteredPoints.Last() = MidPoint;
						CurrentPoint = MidPoint; // 중간 점을 기준으로 설정
					}
					else
					{
						FilteredPoints.Add(NextPoint);
						CurrentPoint = NextPoint;
					}
				}
			}


			// 병합된 점들로 스플라인 설정
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


			//탄젠트를 이용해 곡선 제한
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
				const float MaxTangentScale = 200.0f; // 탄전트 크기 제한
				if (Distance > MaxTangentScale)
				{
					Tangent = Tangent.GetSafeNormal() * MaxTangentScale;
				}

				if (i > 0)
				{
					SplineComponent->SetTangentAtSplinePoint(i - 1, Tangent, ESplineCoordinateSpace::World);
				}
			}



			// 스플라인 메쉬 생성-----------------------------------------
			for (int32 i = 0; i < SplineComponent->GetNumberOfSplinePoints() - 1; ++i)
			{
				FVector StartLocation, StartTangent;
				FVector EndLocation, EndTangent;

				// 시작 및 끝 지점의 위치와 탄젠트 가져오기
				SplineComponent->GetLocationAndTangentAtSplinePoint(i, StartLocation, StartTangent, ESplineCoordinateSpace::Local);
				SplineComponent->GetLocationAndTangentAtSplinePoint(i + 1, EndLocation, EndTangent, ESplineCoordinateSpace::Local);

				// 스플라인 메쉬 컴포넌트 생성
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




//동기로 길찾기 구현
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
//		// 경로를 디버깅으로 출력
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



