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
	// ī�޶� NPC�� ī�޶�� �ε巴�� ��ȯ (1�� ���� ��ȯ)
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

		//TODO: ���� ����Ʈ �ѹ��� ���� �� �ְ� UI�Ŵ������� ��������Ʈ�� ��ư ����
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

		// ���콺 Ŀ�� Ȱ��ȭ
		PlayerController->bShowMouseCursor = true;

		// Ŀ���� ȭ�� �߾����� �̵�
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

		// ���콺 Ŀ�� Ȱ��ȭ
		PlayerController->bShowMouseCursor = false;

	}
	if(viewTarget !=nullptr)
		PlayerController->SetViewTargetWithBlend(PlayerController->GetPawn(), 1, EViewTargetBlendFunction::VTBlend_Linear, 0.1f, false);
}



void ACInteractive_NPC::OnInteract_Implementation()
{
	//TODO: ���� NPC��ȭ�� ī�޶����� �ʿ�

}

