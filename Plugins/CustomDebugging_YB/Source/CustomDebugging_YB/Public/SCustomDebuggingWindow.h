#pragma once

#include "CoreMinimal.h"
#include "Widgets/SCompoundWidget.h"

class SCustomDebuggingWindow : public SCompoundWidget
{

public:
	SLATE_BEGIN_ARGS(SCustomDebuggingWindow) {}
	SLATE_END_ARGS()

	void Construct(const FArguments& InArgs);

private:
	FText GetFrameRateText() const;
	FText GetActorCountText() const;
	void ActorSnap();
};
