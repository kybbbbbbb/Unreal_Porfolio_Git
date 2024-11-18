#include "Interface/CInteractive_NPC.h"
#include "../GameManager/CUIManager_Game.h"

ACInteractive_NPC::ACInteractive_NPC()
{
	PrimaryActorTick.bCanEverTick = true;
	NPC_Standard_Dialogue = FText::FromString(TEXT("So Tire..."));
}

void ACInteractive_NPC::BeginPlay()
{
	Super::BeginPlay();
	NPCName = FText::FromString(TEXT("Warrior"));
}

void ACInteractive_NPC::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ACInteractive_NPC::Interact()
{


	OnInteract();
	// 카메라를 NPC의 카메라로 부드럽게 전환 (1초 동안 전환)
	PlayerStop();

	UCUIManager_Game* UIManager = UCUIManager_Game::GetInstance(GetWorld());

	if (UIManager == nullptr)
		return;


	UIManager->ShowNPCDialogueUI(this);

	bOnaction = true;



}

void ACInteractive_NPC::AcceptQuest_NPC()
{
	UCUIManager_Game* UIManager = UCUIManager_Game::GetInstance(GetWorld());

	if (UIManager == nullptr)
		return;

	UIManager->HideNPCDialogueUI();

		//TODO: 추후 퀘스트 넘버를 받을 수 있게 UI매니저에서 델리게이트로 버튼 연결
	FQuestTable* data = QuestTable->FindRow<FQuestTable>(FName("1"), TEXT("No Find"));
	data->AcceptQuest();
	
	UIManager->QuestUIOn(data);
	
}

void ACInteractive_NPC::PlayerStop()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();

	if (PlayerController)
	{
		PlayerController->SetIgnoreLookInput(true);
		PlayerController->SetIgnoreMoveInput(true);

		// 마우스 커서 활성화
		PlayerController->bShowMouseCursor = true;

		// 커서를 화면 중앙으로 이동
		PlayerController->SetMouseLocation(500, 500);
	}

	if (viewTarget != nullptr)
	{
		PlayerController->SetViewTargetWithBlend(viewTarget, 1, EViewTargetBlendFunction::VTBlend_EaseInOut, 2.0f, false);
	}
}

void ACInteractive_NPC::PlayerMove()
{
	APlayerController* PlayerController = GetWorld()->GetFirstPlayerController();
	if (PlayerController)
	{
		PlayerController->SetIgnoreLookInput(false);
		PlayerController->SetIgnoreMoveInput(false);

		// 마우스 커서 활성화
		PlayerController->bShowMouseCursor = false;

	}
	if(viewTarget !=nullptr)
		PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), 1, EViewTargetBlendFunction::VTBlend_Linear, 0.1f, false);
}



void ACInteractive_NPC::OnInteract_Implementation()
{
	//TODO: 추후 NPC대화시 카메라조작 필요

}

