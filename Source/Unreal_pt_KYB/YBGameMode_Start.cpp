#include "YBGameMode_Start.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Sound/SoundCue.h"
#include "Blueprint/UserWidget.h"
#include "Components/Button.h"
#include "Engine/StreamableManager.h"
#include "Engine/AssetManager.h"
#include "Engine/LevelStreamingDynamic.h"
#include "Components/AudioComponent.h"

AYBGameMode_Start::AYBGameMode_Start()
{
	DefaultPawnClass = nullptr; 
	PrimaryActorTick.bCanEverTick = true;


}

void AYBGameMode_Start::BeginPlay()
{
	Super::BeginPlay();
	ShowLoadingScreen();
}



bool AYBGameMode_Start::AreAllLevelInstancesLoaded()
{
	for (ULevelStreaming* StreamingLevel : GetWorld()->GetStreamingLevels())
	{
		if (StreamingLevel && (!StreamingLevel->IsLevelLoaded() || !StreamingLevel->IsLevelVisible()))
		{
			return false;
		}
	}
	return true;

}

void AYBGameMode_Start::ShowLoadingScreen()
{
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/LoadingUI.LoadingUI_C'"));
	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidgetClass);
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->AddToViewport();
		}
	}


}

void AYBGameMode_Start::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (AreAllLevelInstancesLoaded())
	{
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->RemoveFromViewport();
		}
		SetupCameraAndUI();

		SetActorTickEnabled(false);
	}
}








void AYBGameMode_Start::SetupCameraAndUI()
{
	// ī�޶� ����
	ACameraActor* StartCamera = Cast<ACameraActor>(UGameplayStatics::GetActorOfClass(GetWorld(), ACameraActor::StaticClass()));
	if (StartCamera)
	{
		APlayerController* PlayerController = UGameplayStatics::GetPlayerController(this, 0);
		if (PlayerController)
		{
			PlayerController->SetViewTarget(StartCamera);

			PlayerController->bShowMouseCursor = true;

		}
	}

	// ���� ���
	//USoundCue* StartSound = LoadObject<USoundCue>(nullptr, TEXT("/Script/Engine.SoundCue'/Game/04_Object/Sound/SoundCue/BGM/BGM_Full.BGM_Full'"));
	//if (StartSound)
	//{
	//	soundComponent = UGameplayStatics::SpawnSound2D(this, StartSound);
	//}

	TSubclassOf<UUserWidget> MenuScreenWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/MenuWiget.MenuWiget_C'"));
	if (MenuScreenWidgetClass)
	{
		MenuWidget = CreateWidget<UUserWidget>(GetWorld(), MenuScreenWidgetClass);
		if (MenuWidget)
		{
			MenuWidget->AddToViewport();

			// Start ��ư �̺�Ʈ ���ε�
			UButton* StartButton = Cast<UButton>(MenuWidget->GetWidgetFromName(TEXT("StartButton")));
			if (StartButton)
			{
				StartButton->OnClicked.AddDynamic(this, &AYBGameMode_Start::OnStartButtonClicked);
			}
		}
	}



}

void AYBGameMode_Start::OnStartButtonClicked()
{
	// ���� ����
	if (soundComponent)
	{
		soundComponent->Stop();
		soundComponent = nullptr; // �ʿ信 ���� ���� ����
	}
	if (MenuWidget)
	{
		MenuWidget->RemoveFromViewport();
	}
	OnAssetsLoaded();
}



void AYBGameMode_Start::OnAssetsLoaded()
{
	UGameplayStatics::OpenLevel(this, "open_world_LSP_v2");
	

}

