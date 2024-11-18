#include "YBGameMode.h"
#include "GameManager/CSoundManager.h"
#include "GameManager/CUIManager_Game.h"
#include "Kismet/GameplayStatics.h"
#include "Camera/CameraActor.h"
#include "Components/Button.h"
#include "ContentStreaming.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"


AYBGameMode::AYBGameMode()
{
	ConstructorHelpers::FClassFinder<APawn> player(L"/Script/Engine.Blueprint'/Game/Characters/01_Player/BP_CPlayer2.BP_CPlayer2_C'");
	DefaultPawnClass = player.Class;
	PrimaryActorTick.bCanEverTick = true;
}

void AYBGameMode::BeginPlay()
{
	Super::BeginPlay();
	ShowLoadingScreen();
	UGameplayStatics::LoadStreamLevel(this, "YB_Map", true, true, FLatentActionInfo());
	SetActorTickEnabled(true);

	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AYBGameMode::PauseGame, 0.1f, false);  // 0.1�� �� ����
}










bool AYBGameMode::AreAllLevelInstancesLoaded()
{
	return AreAllLevelsLoadedRecursively(GetWorld()->GetStreamingLevels());
}

bool AYBGameMode::AreAllLevelsLoadedRecursively(const TArray<ULevelStreaming*>& Levels)
{
	TArray<ULevelStreaming*> LevelsToCheck = GetWorld()->GetStreamingLevels();
	while (LevelsToCheck.Num() > 0)
	{
		ULevelStreaming* StreamingLevel = LevelsToCheck.Pop();
		if (StreamingLevel)
		{
			// ���� ������ �ε�ǰ� ����ȭ�Ǿ����� Ȯ��
			if (!StreamingLevel->IsLevelLoaded() || !StreamingLevel->IsLevelVisible())
			{
				return false;
			}

			// ���� ��Ʈ���� ������ �߰��Ͽ� �˻�
			UWorld* SubLevelWorld = StreamingLevel->GetLoadedLevel()->GetWorld();
			if (SubLevelWorld)
			{
				LevelsToCheck.Append(SubLevelWorld->GetStreamingLevels());
			}
		}
	}
	return true;
}



void AYBGameMode::ShowLoadingScreen()
{
	TSubclassOf<UUserWidget> LoadingScreenWidgetClass = LoadClass<UUserWidget>(nullptr, TEXT("/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/LoadingUI.LoadingUI_C'"));
	if (LoadingScreenWidgetClass)
	{
		LoadingScreenWidget = CreateWidget<UUserWidget>(GetWorld(), LoadingScreenWidgetClass);
		if (LoadingScreenWidget)
		{
			LoadingScreenWidget->AddToViewport(1000);
		}
	}
}

void AYBGameMode::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	
}

void AYBGameMode::PauseGame()
{
	// ĳ���͵��� ��� �������� ���߱�
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			Character->GetCharacterMovement()->StopMovementImmediately();  // ĳ������ ������ ���߱�
			Character->CustomTimeDilation = 0.0f;
		}
	}


	// 5�� �Ŀ� ������ �簳�ϴ� Ÿ�̸� ����
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AYBGameMode::ResumeGame, 10.0f, false);  // 5�� �� ����

}

void AYBGameMode::ResumeGame()
{


	// �ε� ȭ���� ����� (ShowLoadingScreen�� ����ų� ���� ó��)
	LoadingScreenWidget->RemoveFromViewport();

	// ĳ���͵��� �������� �ٽ� ���
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);  // ĳ���� �ٽ� �ȱ� ���� ����
			Character->CustomTimeDilation = 1.0f;
		}
	}

}

bool AYBGameMode::AreCriticalResourcesReady()
{
	// IStreamingManager�� ���� ��Ʈ���� ���� Ȯ��
	IStreamingManager& StreamingManager = IStreamingManager::Get();
	int32 NumWantingResources = StreamingManager.GetNumWantingResources();

	// ��Ʈ������ �ʿ��� ���ҽ��� ������ Ȯ��
	return NumWantingResources == 0;
}



void AYBGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// ���� ���� �� ���� �Ŵ��� ����
	UCSoundManager::ShutDown();
	UCUIManager_Game::ShutDown();


}

