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
	//4������ �������� ����
	ChildSlot
		[
			//������ ����----------------------------------------------
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
				//�׼� ����----------------------------------------------
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
				//���� ����----------------------------------------------
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
	FWorldContext& context = GEditor->GetEditorWorldContext(); // ���� ���� ��������
	UWorld* World = context.World();
	if (World)
	{
		float FrameTime = World->GetDeltaSeconds(); // �ǽð� ������ ������ �ð� ��������
		return FText::Format(NSLOCTEXT("YourWidget", "FrameTime", "Frame Time: {0} ms"), FText::AsNumber(FrameTime * 1000.0f)); // �и��ʷ� ��ȯ
	}
	return FText::FromString("Frame Time: N/A"); // ���尡 nullptr�� ��� ó��
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

					// ��� �ݸ��� ������Ʈ�� �˻�
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

					// ���� ���� �ݸ��� ������Ʈ�� �������� ��ġ ����
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
		// ���� ����Ʈ ��������
		FViewport* Viewport = GEditor->GetActiveViewport();
		if (Viewport)
		{
			// ����Ʈ Ŭ���̾�Ʈ ��������
			FEditorViewportClient* ViewportClient = static_cast<FEditorViewportClient*>(Viewport->GetClient());
			if (ViewportClient)
			{
				// ī�޶� ��ġ�� ȸ�� ��������
				FVector NewLocation = ViewportClient->GetViewLocation();

				// �÷��̾� ��ŸƮ ã��
				for (TActorIterator<APlayerStart> It(World); It; ++It)
				{
					APlayerStart* PlayerStart = *It;
					if (PlayerStart)
					{
						// �÷��̾� ��ŸƮ�� ���ο� ��ġ�� �̵�
						PlayerStart->SetActorLocation(NewLocation);
						PlayerStart->MarkComponentsRenderStateDirty(); // ���� ���� ������Ʈ
						break; // ù ��° �÷��̾� ��ŸƮ�� �̵�
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
			// ������ �̸�, Ŭ����, ��ġ, ȸ��, ������ ��������
			FString ActorName = Actor->GetName();
			FString ActorClassName = Actor->GetClass()->GetName();
			FVector ActorLocation = Actor->GetActorLocation();
			FRotator ActorRotation = Actor->GetActorRotation();
			FVector ActorScale = Actor->GetActorScale3D();

			// ������ �±� ��������
			FString Tagss = TEXT("");
			for (const FName& Tag1 : Actor->Tags)
			{
				Tagss += Tag1.ToString() + TEXT(", ");
			}
			// ������ ��ǥ ����
			if (Tagss.Len() > 0)
			{
				Tagss.RemoveAt(Tagss.Len() - 2, 2); // ������ ��ǥ�� ���� ����
			}

			// ���� �̸� ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// Ŭ���� ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// ��ġ ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// ȸ�� ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// ������ ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// ���� Ŭ������ ���� �� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// �±� ���� �߰�
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
								.Font(FCoreStyle::GetDefaultFontStyle("Regular", 10)) // �۾� ũ�� ����
						]
				];

			// �� ���� ���̿� ���� �߰�
			InfoBox->AddSlot()
				.AutoHeight()
				.Padding(10, 10, 10, 10) // �Ʒ��� ���� �߰�
				[
					SNew(SBorder)
						.BorderBackgroundColor(FLinearColor::Transparent) // ������ ���� ������ �߰�
						.Padding(0)
				];
		}
	}
}

