#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "YBGameMode_Start.generated.h"

UCLASS()
class UNREAL_PT_KYB_API AYBGameMode_Start : public AGameModeBase
{
	GENERATED_BODY()
	
private:
	AYBGameMode_Start();


public:
	void BeginPlay() override;
	void SetupCameraAndUI();
	bool AreAllLevelInstancesLoaded();
	void ShowLoadingScreen();
	void Tick(float DeltaSeconds) override;
	void OnAssetsLoaded();

	UFUNCTION()
	void OnStartButtonClicked();


	UUserWidget* LoadingScreenWidget;
	UUserWidget* MenuWidget;
	UUserWidget* PressAnyKeyWidget;
	UAudioComponent* soundComponent;


};
