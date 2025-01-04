#include "SCustomDebuggingWindow.h"
#include "CustomDebugging_YB.h"
#include "Widgets/Text/STextBlock.h"
#include "Widgets/Layout/SBox.h"
#include "Widgets/SOverlay.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"
#include "Engine/Engine.h"
#include "Styling/SlateColor.h" // 배경 색상 등을 위한 헤더
#include "Editor.h"
#include "Selection.h"


void SCustomDebuggingWindow::Construct(const FArguments& InArgs)
{
	ChildSlot
		[
			SNew(SOverlay)
				+ SOverlay::Slot()
				[
					SNew(SVerticalBox)

						// Frame Rate Section
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SBox)
								.WidthOverride(300)
								.HeightOverride(50)
								[
									SNew(SBorder)
										.BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f))) // Background color
										.Padding(5)
										[
											SNew(STextBlock)
												.Text(this, &SCustomDebuggingWindow::GetFrameRateText)
												.ColorAndOpacity(FSlateColor(FLinearColor::White)) // Text color
										]
								]
						]

						// Actor Count Section
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SBox)
								.WidthOverride(300)
								.HeightOverride(50)
								[
									SNew(SBorder)
										.BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f)))
										.Padding(5)
										[
											SNew(STextBlock)
												.Text(this, &SCustomDebuggingWindow::GetActorCountText)
												.ColorAndOpacity(FSlateColor(FLinearColor::White))
										]
								]
						]

						// Actor Snap Section
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(STextBlock)
								.Text(FText::FromString(TEXT("Actor Snap Function")))
						]
						+ SVerticalBox::Slot()
						.AutoHeight()
						.Padding(10)
						[
							SNew(SBox)
								.WidthOverride(300)
								.HeightOverride(50)
								[
										SNew(SBorder)
										.BorderBackgroundColor(FSlateColor(FLinearColor(0.1f, 0.1f, 0.1f)))
										.Padding(5)
										[
											SNew(SButton)
												.OnClicked_Lambda([this]() -> FReply
													{
														ActorSnap();
														return FReply::Handled();
													})
												.Text(FText::FromString(TEXT("Snap Actor to Origin")))
										]
								]
						]
				]
		];
}

FText SCustomDebuggingWindow::GetFrameRateText() const
{
	const float FrameRate = 1.0f / FApp::GetDeltaTime();
	FString FrameRateString = FString::Printf(TEXT("Frame Rate: %d FPS"), FMath::RoundToInt(FrameRate));
	return FText::FromString(FrameRateString);
}

FText SCustomDebuggingWindow::GetActorCountText() const
{
	UWorld* World = GEngine->GetWorldContexts()[0].World();
	if (World)
	{
		int32 ActorCount = World->GetActorCount();
		FString ActorCountString = FString::Printf(TEXT("Actors in World: %d"), ActorCount);
		return FText::FromString(ActorCountString);
	}
	return FText::FromString(TEXT("Actors in World: No World Found"));
}

void SCustomDebuggingWindow::ActorSnap() 
{
	// 선택된 액터를 가져옵니다.
	AActor* SelectedActor = nullptr;
	if (GEditor)
	{
		// 현재 선택된 액터를 가져옵니다.
		USelection * select = GEditor->GetSelectedActors();
		
		if (select->Num() > 0)
		{
			SelectedActor = Cast<AActor>(select->GetSelectedObject(0));
		}
	}

	if (SelectedActor)
	{
		// 액터를 (0, 0, 0) 위치로 스냅시킵니다.
		SelectedActor->SetActorLocation(FVector(0.f, 0.f, 0.f));

		// 사용자가 확인할 수 있도록 알림을 표시할 수 있습니다.
		UE_LOG(LogTemp, Warning, TEXT("Actor %s snapped to (0, 0, 0)"), *SelectedActor->GetName());
	}
	else
	{
		// 선택된 액터가 없을 경우
		UE_LOG(LogTemp, Warning, TEXT("No actor selected to snap."));
	}
}
