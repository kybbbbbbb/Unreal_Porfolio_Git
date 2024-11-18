#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YBGameMode.generated.h"

UCLASS()
class UNREAL_PT_KYB_API AYBGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AYBGameMode();

public:
	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
	void BeginPlay() override;
	bool AreAllLevelInstancesLoaded();
	void ShowLoadingScreen();
	void Tick(float DeltaSeconds) override;

	void PauseGame();
	void ResumeGame();

	bool AreAllLevelsLoadedRecursively(const TArray<ULevelStreaming*>& Levels);
	bool AreCriticalResourcesReady();
	UUserWidget* LoadingScreenWidget;


	int8 GameModeset = 1;
};
