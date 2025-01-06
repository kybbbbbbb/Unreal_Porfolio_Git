#include "DebugWidget_YB.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Editor.h"
#include "EditorViewportClient.h"
#include "EngineUtils.h"
#include "Selection.h"
#include "Components/CapsuleComponent.h"
#include "Widgets/Text/STextBlock.h"
#include "SceneView.h"
#include "Slate/SceneViewport.h"
#include "GameFramework/PlayerStart.h"

void DebugWidget_YB::Construct(const FArguments& InArgs)
{
	//4가지의 섹션으로 구분
	ChildSlot
		[
			//렌더링 섹션----------------------------------------------
			SNew(SVerticalBox)
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Black)
						.Padding(10)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(STextBlock)
										.Text(FText::FromString("Rendering Section"))
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
										.ColorAndOpacity(FLinearColor::White)
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(this, &DebugWidget_YB::GetFPS)
										.ColorAndOpacity(FLinearColor::Yellow)
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(this, &DebugWidget_YB::GetFrameTime)
										.ColorAndOpacity(FLinearColor::Yellow)
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(STextBlock)
										.Text(this, &DebugWidget_YB::GetActorCount)
										.ColorAndOpacity(FLinearColor::Yellow)
								]
						]
				]
				//액션 섹션----------------------------------------------
				+ SVerticalBox::Slot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Black)
						.Padding(10)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(STextBlock)
										.Text(FText::FromString("Actions Section"))
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
										.ColorAndOpacity(FLinearColor::White)
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(SButton)
										.Text(FText::FromString(TEXT("Snap Actors")))
										.OnClicked_Lambda([this]() { SnapActors(); return FReply::Handled(); })
										.ButtonColorAndOpacity(FLinearColor::Blue)
										.ContentPadding(FMargin(10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(SButton)
										.Text(FText::FromString("PlayerStart To CurrentLocation"))
										.OnClicked_Lambda([this]() { MovePlayerStartToViewPort(); return FReply::Handled(); })
										.ButtonColorAndOpacity(FLinearColor::Blue)
										.ContentPadding(FMargin(10))
								]
								+ SVerticalBox::Slot()
								.AutoHeight()
								.Padding(5)
								[
									SNew(SButton)
										.Text(FText::FromString("Take Screenshot"))
										.OnClicked_Lambda([this]() { TakeScreenShot(); return FReply::Handled(); })
										.ButtonColorAndOpacity(FLinearColor::Blue)
										.ContentPadding(FMargin(10))
								]
						]
				]
				//인포 섹션----------------------------------------------
				+ SVerticalBox::Slot()
				.FillHeight(1.0f)
				.Padding(5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Black)
						.Padding(10)
						[
							SNew(SVerticalBox)
								+ SVerticalBox::Slot()
								.AutoHeight()
								[
									SNew(STextBlock)
										.Text(FText::FromString("Actor Info Section"))
										.Font(FCoreStyle::GetDefaultFontStyle("Regular", 14))
										.ColorAndOpacity(FLinearColor::White)
								]
								+ SVerticalBox::Slot()
								.FillHeight(1.0f)
								[
									SNew(SScrollBox)
										+ SScrollBox::Slot()
										[
											SAssignNew(InfoBox, SVerticalBox)
										]
								]
						]
				]
		];
}

void DebugWidget_YB::Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime)
{
	UpdateActorInfo();
}

FText DebugWidget_YB::GetFPS() const
{
	static double LastTime = 0.0;
	static int32 FrameCount = 0;
	static float FPS = 0.0f;

	double CurrentTime = FPlatformTime::Seconds();
	FrameCount++;

	if (CurrentTime - LastTime >= 0.1)
	{
		FPS = FrameCount / (CurrentTime - LastTime);
		LastTime = CurrentTime;
		FrameCount = 0;
	}

	return FText::Format(NSLOCTEXT("YourWidget", "FPS", "FPS: {0}"), FText::AsNumber(FPS));
}

FText DebugWidget_YB::GetFrameTime() const
{
	FWorldContext& context = GEditor->GetEditorWorldContext(); // 현재 월드 가져오기
	UWorld* World = context.World();
	if (World)
	{
		float FrameTime = World->GetDeltaSeconds(); // 실시간 프레임 렌더링 시간 가져오기
		return FText::Format(NSLOCTEXT("YourWidget", "FrameTime", "Frame Time: {0} ms"), FText::AsNumber(FrameTime * 1000.0f)); // 밀리초로 변환
	}
	return FText::FromString("Frame Time: N/A"); // 월드가 nullptr일 경우 처리
}

FText DebugWidget_YB::GetActorCount() const
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		int32 ActorCount = World->GetActorCount();
		return FText::Format(NSLOCTEXT("YourWidget", "ActorCount", "All Actors: {0}"), FText::AsNumber(ActorCount));
	}
	return FText::FromString("Actors: N/A");
}

void DebugWidget_YB::SnapActors()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		const FScopedTransaction Transaction(NSLOCTEXT("YourWidget", "SnapActorsToFloor", "Snap Actors to Floor"));

		TArray<AActor*> SelectedActors;
		GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

		for (AActor* Actor : SelectedActors)
		{
			if (Actor)
			{
				Actor->Modify();

				FVector ActorLocation = Actor->GetActorLocation();
				FVector Start = ActorLocation;
				FVector End = Start - FVector(0, 0, 10000);

				FHitResult HitResult;
				FCollisionQueryParams Params;
				Params.AddIgnoredActor(Actor);

				if (World->LineTraceSingleByChannel(HitResult, Start, End, ECC_Visibility, Params))
				{
					FVector NewLocation = HitResult.Location;

					// 모든 콜리전 컴포넌트를 검사
					float LowestPoint = TNumericLimits<float>::Max();
					for (UActorComponent* Component : Actor->GetComponents())
					{
						UPrimitiveComponent* PrimComp = Cast<UPrimitiveComponent>(Component);
						if (PrimComp && PrimComp->IsCollisionEnabled())
						{
							FBoxSphereBounds Bounds = PrimComp->Bounds;
							float ComponentLowestPoint = Bounds.Origin.Z - Bounds.BoxExtent.Z;
							if (ComponentLowestPoint < LowestPoint)
							{
								LowestPoint = ComponentLowestPoint;
							}
						}
					}

					// 가장 낮은 콜리전 컴포넌트를 기준으로 위치 조정
					float Adjustment = ActorLocation.Z - LowestPoint;
					NewLocation.Z += Adjustment;

					Actor->SetActorLocation(NewLocation);
				}
			}
		}
	}
}

void DebugWidget_YB::TakeScreenShot()
{
	FScreenshotRequest::RequestScreenshot(false);
}

void DebugWidget_YB::MovePlayerStartToViewPort()
{
	UWorld* World = GEditor->GetEditorWorldContext().World();
	if (World)
	{
		// 현재 뷰포트 가져오기
		FViewport* Viewport = GEditor->GetActiveViewport();
		if (Viewport)
		{
			// 뷰포트 클라이언트 가져오기
			FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(Viewport->GetClient());
			if (ViewportClient)
			{
				// 카메라 위치와 회전 가져오기
				FVector NewLocation = ViewportClient->GetViewLocation();

				// 플레이어 스타트 찾기
				for (TActorIterator<APlayerStart> It(World); It; ++It)
				{
					APlayerStart* PlayerStart = *It;
					if (PlayerStart)
					{
						// 플레이어 스타트를 새로운 위치로 이동
						PlayerStart->SetActorLocation(NewLocation);
						PlayerStart->MarkComponentsRenderStateDirty(); // 렌더 상태 업데이트
						break; // 첫 번째 플레이어 스타트만 이동
					}
				}
			}
		}
	}

}

void DebugWidget_YB::UpdateActorInfo()
{
	TArray<AActor*> SelectedActors;
	GEditor->GetSelectedActors()->GetSelectedObjects<AActor>(SelectedActors);

	InfoBox->ClearChildren();

	for (AActor* Actor : SelectedActors)
	{
		if (Actor)
		{
			// 액터의 이름, 클래스, 위치, 회전, 스케일 가져오기
			FString ActorName = Actor->GetName();
			FString ActorClassName = Actor->GetClass()->GetName();
			FVector ActorLocation = Actor->GetActorLocation();
			FRotator ActorRotation = Actor->GetActorRotation();
			FVector ActorScale = Actor->GetActorScale3D();

			// 액터의 태그 가져오기
			FString Tagss = TEXT("");
			for (const FName& Tag1 : Actor->Tags)
			{
				Tagss += Tag1.ToString() + TEXT(", ");
			}
			// 마지막 쉼표 제거
			if (Tagss.Len() > 0)
			{
				Tagss.RemoveAt(Tagss.Len() - 2, 2); // 마지막 쉼표와 공백 제거
			}

			// 액터 이름 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10,5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorName", "Name: {0}"), FText::FromString(ActorName)))
								.ColorAndOpacity(FLinearColor::Green)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 클래스 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10,5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorClass", "Class: {0}"), FText::FromString(ActorClassName)))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 위치 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorLocation", "Location: {0}"), FText::FromString(ActorLocation.ToString())))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 회전 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorRotation", "Rotation: {0}"), FText::FromString(ActorRotation.ToString())))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 스케일 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorScale", "Scale: {0}"), FText::FromString(ActorScale.ToString())))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 같은 클래스의 액터 수 추가
			int32 SameClassCount = 0;
			for (TActorIterator<AActor> It(GEditor->GetEditorWorldContext().World()); It; ++It)
			{
				if (It->GetClass() == Actor->GetClass())
				{
					SameClassCount++;
				}
			}

			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "SameClassCount", "Same Class Count: {0}"), FText::AsNumber(SameClassCount)))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 태그 정보 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(5,5)
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Gray)
						.Padding(10, 5)
						[
							SNew(STextBlock)
								.Text(FText::Format(NSLOCTEXT("YourWidget", "ActorTags", "Tags: {0}"), FText::FromString(Tagss)))
								.ColorAndOpacity(FLinearColor::White)
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // 글씨 크기 통일
						]
				];

			// 각 액터 사이에 여백 추가
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(10, 10, 10, 10) // 아래쪽 여백 추가
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Transparent) // 투명한 경계로 여백을 추가
						.Padding(0)
				];
		}
	}
}

