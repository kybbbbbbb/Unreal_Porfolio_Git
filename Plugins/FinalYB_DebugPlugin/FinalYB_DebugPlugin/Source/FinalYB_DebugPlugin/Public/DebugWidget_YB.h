#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class DebugWidget_YB : public SCompoundWidget
{
public:
	SLATE_BEGIN_ARGS(DebugWidget_YB) {}
	SLATE_END_ARGS()


	void Construct(const FArguments& InArgs);
	virtual void Tick(const FGeometry& AllottedGeometry, const double InCurrentTime, const float InDeltaTime) override;

private:
	//������ ����
	FText GetFPS() const;
	FText GetFrameTime() const;


	//�׼� ����
	FText GetActorCount() const;
	void SnapActors();
	void TakeScreenShot();
	void MovePlayerStartToViewPort();

	TSharedPtr<SVerticalBox> InfoBox;

	void UpdateActorInfo();
};
