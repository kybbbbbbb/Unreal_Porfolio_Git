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
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AYBGameMode::PauseGame, 0.1f, false);  // 0.1초 후 실행
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
			// 현재 레벨이 로드되고 가시화되었는지 확인
			if (!StreamingLevel->IsLevelLoaded() || !StreamingLevel->IsLevelVisible())
			{
				return false;
			}

			// 하위 스트리밍 레벨을 추가하여 검사
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
	// 캐릭터들의 모든 움직임을 멈추기
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			Character->GetCharacterMovement()->StopMovementImmediately();  // 캐릭터의 움직임 멈추기
			Character->CustomTimeDilation = 0.0f;
		}
	}


	// 5초 후에 게임을 재개하는 타이머 설정
	FTimerHandle TimerHandle;
	GetWorld()->GetTimerManager().SetTimer(TimerHandle, this, &AYBGameMode::ResumeGame, 10.0f, false);  // 5초 후 실행

}

void AYBGameMode::ResumeGame()
{


	// 로딩 화면을 숨기기 (ShowLoadingScreen을 지우거나 숨김 처리)
	LoadingScreenWidget->RemoveFromViewport();

	// 캐릭터들의 움직임을 다시 허용
	TArray<AActor*> AllActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACharacter::StaticClass(), AllActors);
	for (AActor* Actor : AllActors)
	{
		ACharacter* Character = Cast<ACharacter>(Actor);
		if (Character)
		{
			Character->GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);  // 캐릭터 다시 걷기 모드로 설정
			Character->CustomTimeDilation = 1.0f;
		}
	}

}

bool AYBGameMode::AreCriticalResourcesReady()
{
	// IStreamingManager를 통해 스트리밍 상태 확인
	IStreamingManager& StreamingManager = IStreamingManager::Get();
	int32 NumWantingResources = StreamingManager.GetNumWantingResources();

	// 스트리밍이 필요한 리소스가 없는지 확인
	return NumWantingResources == 0;
}



void AYBGameMode::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);

	// 게임 종료 시 사운드 매니저 정리
	UCSoundManager::ShutDown();
	UCUIManager_Game::ShutDown();


}

