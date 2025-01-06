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
	//렌더링 섹션
	FText GetFPS() const;
	FText GetFrameTime() const;


	//액션 섹션
	FText GetActorCount() const;
	void SnapActors();
	void TakeScreenShot();
	void MovePlayerStartToViewPort();

	TSharedPtr<SVerticalBox> InfoBox;

	void UpdateActorInfo();
};
