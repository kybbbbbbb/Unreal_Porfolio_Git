#include "GameManager/CUIManager_Game.h"
#include "Kismet/GameplayStatics.h"
#include "Components/Button.h"
#include "CUserWidget.h"
#include "Components/ProgressBar.h"
#include "../Components/CHealthComponent.h"
#include "Components/CheckBox.h"
#include "Components/VerticalBox.h"
#include "Components/HorizontalBox.h"
#include "Blueprint/WidgetTree.h"
#include "Components/HorizontalBoxSlot.h"
#include "Components/UniformGridPanel.h"
#include "Components/UniformGridSlot.h"
#include "Materials/MaterialInstanceDynamic.h"


UCUIManager_Game* UCUIManager_Game::Instance = nullptr;


UCUIManager_Game::UCUIManager_Game()
{
	DialogueUIclass = ConstructorHelpers::FClassFinder<UCUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/QuestWiget_1.QuestWiget_1_C'").Class;

	PlayerUIclass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/PlayerUI_1.PlayerUI_1_C'").Class;

	ItemBoxUIclass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/Item/ItemWidjet.ItemWidjet_C'").Class;

	QuestUIclass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/QuestUI.QuestUI_C'").Class;

	SkillUIclass = ConstructorHelpers::FClassFinder<UUserWidget>(L"/Script/UMGEditor.WidgetBlueprint'/Game/04_Object/SkillUI.SkillUI_C'").Class;

	NoExistSkilltexture = ConstructorHelpers::FObjectFinder<UTexture2D>(L"/Script/Engine.Texture2D'/Game/05_Material/NoButtonTexture.NoButtonTexture'").Object;
}

void UCUIManager_Game::Initialize(UWorld* World)
{
	 APawn* playerPawn = World->GetFirstPlayerController()->GetPawn();
	 if (playerPawn == nullptr)
		 return;
	
	if (World)
	{
		InWorld = World;
			DialogueUI = CreateWidget<UCUserWidget>(World, DialogueUIclass); 

			if (DialogueUI)
			{
				DialogueUI->AddToViewport();
				
				DialogueUI->GetRightNoButton()->OnClicked.AddDynamic(this, &UCUIManager_Game::HideNPCDialogueUI);
				DialogueUI->SetVisibility(ESlateVisibility::Collapsed);
			}

			PlayerUI = CreateWidget<UUserWidget>(World, PlayerUIclass); 

			if (PlayerUI)
			{
				UCHealthComponent* healthcomponent = Cast<UCHealthComponent>(playerPawn->GetComponentByClass(UCHealthComponent::StaticClass()));
				UCWeaponComponent_reset* Weaponcomponent = Cast<UCWeaponComponent_reset>(playerPawn->GetComponentByClass(UCWeaponComponent_reset::StaticClass()));
				weapon = Weaponcomponent;
				healthcomponent->HPDelegate.AddDynamic(this, &UCUIManager_Game::HPprogressbarChange);
				healthcomponent->MPDelegate.AddDynamic(this, &UCUIManager_Game::MPprogressbarChange);
				Weaponcomponent->WeaponTypeChanged.AddDynamic(this, &UCUIManager_Game::ChangedWeaponImage);
				
				UImage* Unarmed = Cast<UImage>(PlayerUI->GetWidgetFromName(TEXT("Unarmed")));
				CurrentWeaponImage = Unarmed;
				PlayerUI->AddToViewport();
				PlayerUI->SetVisibility(ESlateVisibility::Visible);


			}



			QuestUI = CreateWidget<UUserWidget>(World, QuestUIclass);
			if (QuestUIclass)
			{
				QuestUI->AddToViewport();

				QuestUI->SetVisibility(ESlateVisibility::Collapsed);

			}

			ItemBoxUI = CreateWidget<UUserWidget>(World, ItemBoxUIclass); 
			if (ItemBoxUI)
			{

				UButton* CloseUI = Cast<UButton>(ItemBoxUI->GetWidgetFromName(TEXT("CloseUI")));
				if (CloseUI != nullptr)
				{
					CloseUI->OnClicked.AddDynamic(this, &UCUIManager_Game::ShowItemUI);
				}
				
				ItemBoxUI->AddToViewport(100);

				ItemBoxUI->SetVisibility(ESlateVisibility::Collapsed);

			}

			SkillUI = CreateWidget<UUserWidget>(World, SkillUIclass); 

			if (SkillUI)
			{
				SkillUI->AddToViewport(1);

				SkillUI->SetVisibility(ESlateVisibility::Collapsed);

				UImage* CoolTime_Q = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("CoolTime_Q")));
				if (CoolTime_Q)
				{
					UObject* object = CoolTime_Q->GetBrush().GetResourceObject();
					SkillQ = Cast<UMaterialInstanceDynamic>(object);

					if (SkillQ == nullptr)
					{
						UMaterialInterface* Material = Cast<UMaterialInterface>(object);
						if (Material)
						{
							SkillQ = UMaterialInstanceDynamic::Create(Material, this);
							CoolTime_Q->SetBrushResourceObject(SkillQ);
						}
					}
				}




				UImage* CoolTime_E = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("CoolTime_E")));
				if (CoolTime_E)
				{
					UObject* object = CoolTime_E->GetBrush().GetResourceObject();
					SkillE = Cast<UMaterialInstanceDynamic>(object);

					if (SkillE == nullptr)
					{
						UMaterialInterface* Material = Cast<UMaterialInterface>(object);
						if (Material)
						{
							SkillE = UMaterialInstanceDynamic::Create(Material, this);
							CoolTime_E->SetBrushResourceObject(SkillE);
						}
					}
				}

				UImage* CoolTime_F = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("CoolTime_F")));
				if (CoolTime_F)
				{
					UObject* object = CoolTime_F->GetBrush().GetResourceObject();
					SkillF = Cast<UMaterialInstanceDynamic>(object);

					if (SkillF == nullptr)
					{
						UMaterialInterface* Material = Cast<UMaterialInterface>(object);
						if (Material)
						{
							SkillF = UMaterialInstanceDynamic::Create(Material, this);
							CoolTime_F->SetBrushResourceObject(SkillF);
						}
					}
				}


				CoolTimeText_Q = Cast<UTextBlock>(SkillUI->GetWidgetFromName(TEXT("CoolTimeText_Q")));
				if (CoolTimeText_Q)
				{
					CoolTimeText_Q->SetText(FText::FromString(""));
				}				
				CoolTimeText_E = Cast<UTextBlock>(SkillUI->GetWidgetFromName(TEXT("CoolTimeText_E")));
				if (CoolTimeText_E)
				{
					CoolTimeText_E->SetText(FText::FromString(""));
				}
				CoolTimeText_F = Cast<UTextBlock>(SkillUI->GetWidgetFromName(TEXT("CoolTimeText_F")));
				if (CoolTimeText_F)
				{
					CoolTimeText_F->SetText(FText::FromString(""));
				}
			}
	}


}

void UCUIManager_Game::Tick(float InDeltaTime)
{
	if (SkillQ_Handler != nullptr)
	{
		float remainTime = 0.0f;

		remainTime = InWorld->GetTimerManager().GetTimerRemaining(*SkillQ_Handler);
		
		float value = remainTime / SkillQ_MaxCoolTime;
		value = FMath::Clamp(value, 0.0f, 1.0f);
		SkillQ->SetScalarParameterValue("Percent", value);

		if (FMath::IsNearlyEqual(remainTime, -1.0f))
		{
			CoolTimeText_Q->SetText(FText());
		}
		else
		{
			FString string = FString::Printf(TEXT("%.1f"), remainTime);
			CoolTimeText_Q->SetText(FText::FromString(string));
		}
	}

	if (SkillE_Handler != nullptr)
	{
		float remainTime = 0.0f;

		remainTime = InWorld->GetTimerManager().GetTimerRemaining(*SkillE_Handler);
		
		float value = remainTime / SkillE_MaxCoolTime;
		value = FMath::Clamp(value, 0.0f, 1.0f);
		SkillE->SetScalarParameterValue("Percent", value);

		if (FMath::IsNearlyEqual(remainTime, -1.0f))
		{
			CoolTimeText_E->SetText(FText());
		}
		else
		{
			FString string = FString::Printf(TEXT("%.1f"), remainTime);
			CoolTimeText_E->SetText(FText::FromString(string));
		}
	}

	if (SkillF_Handler != nullptr)
	{
		float remainTime = 0.0f;

		remainTime = InWorld->GetTimerManager().GetTimerRemaining(*SkillF_Handler);
		
		float value = remainTime / SkillF_MaxCoolTime;
		value = FMath::Clamp(value, 0.0f, 1.0f);
		SkillF->SetScalarParameterValue("Percent", value);

		if (FMath::IsNearlyEqual(remainTime, -1.0f))
		{
			CoolTimeText_F->SetText(FText());
		}
		else
		{
			FString string = FString::Printf(TEXT("%.1f"), remainTime);
			CoolTimeText_F->SetText(FText::FromString(string));
		}
	}
}


void UCUIManager_Game::ShowPlayerUI()
{
	PlayerUI->SetVisibility(ESlateVisibility::Visible);
}

void UCUIManager_Game::HidePlayerUI()
{
	PlayerUI->SetVisibility(ESlateVisibility::Collapsed);
}


void UCUIManager_Game::ShowItemUI()
{
	APlayerController* PlayerController = InWorld->GetFirstPlayerController();
	if (!PlayerController) return;

	if (bOnitemInventory == false)
	{
		// 인벤토리 활성화
		bOnitemInventory = true;
		ItemBoxUI->SetVisibility(ESlateVisibility::Visible);

		// 플레이어 입력 비활성화 및 마우스 커서 표시
		PlayerController->SetInputMode(FInputModeUIOnly());  // UI 전용 모드로 설정
		PlayerController->bShowMouseCursor = true;           // 마우스 커서 표시
		PlayerController->SetIgnoreMoveInput(true);          // 이동 입력 무시
		PlayerController->SetIgnoreLookInput(true);          // 시야 전환 입력 무시
	}
	else
	{
		// 인벤토리 비활성화
		bOnitemInventory = false;
		ItemBoxUI->SetVisibility(ESlateVisibility::Collapsed);

		// 플레이어 입력 활성화 및 마우스 커서 숨기기
		PlayerController->SetInputMode(FInputModeGameOnly());  // 게임 모드로 설정
		PlayerController->bShowMouseCursor = false;            // 마우스 커서 숨김
		PlayerController->SetIgnoreMoveInput(false);           // 이동 입력 활성화
		PlayerController->SetIgnoreLookInput(false);           // 시야 전환 입력 활성화
	}
}

void UCUIManager_Game::HideItemUI()
{

	ItemBoxUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UCUIManager_Game::ShowSkillUI()
{
	SkillUI->SetVisibility(ESlateVisibility::Visible);
	
}

void UCUIManager_Game::HideskillUI()
{
	SkillUI->SetVisibility(ESlateVisibility::Collapsed);
}

void UCUIManager_Game::UpdateItemBox(const TArray<FPrimaryAssetId>& InAssetIDDatas, const TArray<UCItemDataAsset*>& ItemDataAssets)
{
	UUniformGridPanel* ItemGrid = Cast<UUniformGridPanel>(ItemBoxUI->GetWidgetFromName(TEXT("ItemGrid")));
	if (ItemGrid)
	{
		int32 SlotIndex = 0;

		// 획득한 각 아이템 ID에 대해 일치하는 ID를 찾기
		for (const FPrimaryAssetId& ItemID : InAssetIDDatas)
		{
			for (UCItemDataAsset* ItemDataAsset : ItemDataAssets)
			{
				if (ItemDataAsset->GetPrimaryAssetId() == ItemID)
				{
					// ItemGrid 내의 이미지 슬롯들을 순회하며 빈 슬롯을 찾아 이미지를 배치
					if (UImage* ItemImageSlot = Cast<UImage>(ItemGrid->GetChildAt(SlotIndex)))
					{
						ItemImageSlot->SetBrushFromTexture(ItemDataAsset->Itemicon);
						ItemImageSlot->SetToolTipText(FText::FromString(ItemDataAsset->ItemName));
					}
					SlotIndex++;
					if (SlotIndex >= ItemGrid->GetChildrenCount()) break; 
				}
			}
		}
	}

}




void UCUIManager_Game::ShowNPCDialogueUI(ACInteractive_NPC* InNPC)
{
	if(PlayerUI != nullptr)
		HidePlayerUI();
	if (SkillUI != nullptr)
		HideskillUI();

	CurrentInNPC = InNPC;
	UDataTable* QuestList = InNPC->GetQuest();

	if (QuestList == nullptr)
	{
		UTextBlock* QuestDialogue = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("Dialogue_Block")));
		UTextBlock* QuestNPCName = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("NPCName_Block")));
		UButton* LeftYesButton = DialogueUI->GetLeftYesButton();

		QuestDialogue->SetText(InNPC->GetStandardDialogue());
		QuestNPCName->SetText(InNPC->GetNPCName());
		LeftYesButton->SetVisibility(ESlateVisibility::Collapsed);

		DialogueUI->SetVisibility(ESlateVisibility::Visible);
		return;
	}



	TArray<FQuestTable*> AllQuestData;
	QuestList->GetAllRows<FQuestTable>("Get all data", AllQuestData);

	if (AllQuestData.Num() > 0 && DialogueUI != nullptr)
	{
		FQuestTable* CurrentQuestData = nullptr;

		for (int32 i = 0; i < AllQuestData.Num(); i++)
		{
			if (AllQuestData[i]->GetIsAccepted() == false)
			{
				CurrentQuestData = AllQuestData[i];

				CurrentQuestRowindex = i + 1;
				break;
			}
		}

		if (CurrentQuestData != nullptr)
		{
			UTextBlock* QuestDialogue = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("Dialogue_Block")));
			UTextBlock* QuestNPCName = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("NPCName_Block")));
			UButton* LeftYesButton = DialogueUI->GetLeftYesButton();

			QuestDialogue->SetText(CurrentQuestData->Description);
			QuestNPCName->SetText(InNPC->GetNPCName());

			LeftYesButton->SetVisibility(ESlateVisibility::Visible);
			LeftYesButton->OnClicked.AddDynamic(InNPC, &ACInteractive_NPC::AcceptQuest_NPC);
			DialogueUI->SetVisibility(ESlateVisibility::Visible);
			return;
		}

		if (CurrentQuestData == nullptr)
		{
			UTextBlock* QuestDialogue = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("Dialogue_Block")));
			UTextBlock* QuestNPCName = Cast<UTextBlock>(DialogueUI->GetWidgetFromName(TEXT("NPCName_Block")));
			UButton* LeftYesButton = DialogueUI->GetLeftYesButton();

			QuestDialogue->SetText(InNPC->GetStandardDialogue());
			QuestNPCName->SetText(InNPC->GetNPCName());
		

			LeftYesButton->SetVisibility(ESlateVisibility::Collapsed);
			DialogueUI->SetVisibility(ESlateVisibility::Visible);
			return;
		}
	}
}

void UCUIManager_Game::HideNPCDialogueUI()
{
	UButton* Okbutton = Cast<UButton>(DialogueUI->GetWidgetFromName(TEXT("LeftYesButton")));
	Okbutton->OnClicked.Clear();

	DialogueUI->SetVisibility(ESlateVisibility::Hidden);

	CurrentInNPC->PlayerMove();
	ShowPlayerUI();
	if (bUnarmd == false)
		ShowSkillUI();

}

void UCUIManager_Game::QuestUIOn(FQuestTable* Indata)
{
	UTextBlock* GoalCountText = Cast<UTextBlock>(QuestUI->GetWidgetFromName(TEXT("QuestCount")));
	CurrentQuestValue = 0;

	FText GoalCountMessage = FText::Format(
		FText::FromString(TEXT("({0} / {1})")),
		FText::AsNumber(CurrentQuestValue),
		FText::AsNumber(Indata->GoalCount)
	);
	CurrentQuest = Indata;

	GoalCountText->SetText(GoalCountMessage);
	QuestUI->SetVisibility(ESlateVisibility::Visible);
}

void UCUIManager_Game::MonsterDeadDelegate()
{
	if (CurrentQuest == nullptr)
		return;
	CurrentQuestValue++;
	if (QuestUI == nullptr)
	{
		return;
	}
	UTextBlock* GoalCountText = Cast<UTextBlock>(QuestUI->GetWidgetFromName(TEXT("QuestCount")));
	if (CurrentQuest->GoalCount == CurrentQuestValue)
	{
		UCheckBox* QuestCheck = Cast<UCheckBox>(QuestUI->GetWidgetFromName(TEXT("QuestCheck")));
		UTextBlock* QuestScript = Cast<UTextBlock>(QuestUI->GetWidgetFromName(TEXT("QuestScript")));

		FSlateColor color(FLinearColor::Green);
		GoalCountText->SetColorAndOpacity(color);
		QuestScript->SetColorAndOpacity(color);
		QuestCheck->SetCheckedState(ECheckBoxState::Checked);

	}

	FText GoalCountMessage = FText::Format(
		FText::FromString(TEXT("({0} / {1})")),
		FText::AsNumber(CurrentQuestValue),
		FText::AsNumber(CurrentQuest->GoalCount)
	);

	GoalCountText->SetText(GoalCountMessage);
}

void UCUIManager_Game::SetSkillCoolTime(float InMaxCool, FTimerHandle* InHandle, FKey InKey)
{
	if (InKey == EKeys::Q)
	{
		SkillQ_Handler = InHandle;
		SkillQ_MaxCoolTime = InMaxCool;
	}
	else if (InKey == EKeys::E)
	{
		SkillE_Handler = InHandle;
		SkillE_MaxCoolTime = InMaxCool;
	}
	else if (InKey == EKeys::F)
	{
		SkillF_Handler = InHandle;
		SkillF_MaxCoolTime = InMaxCool;
	}



}


UCUIManager_Game* UCUIManager_Game::GetInstance(UWorld* Inworld)
{
	if (Inworld == nullptr)  
	{
		return nullptr;
	}

	if (Instance == nullptr)
	{
		Instance = NewObject<UCUIManager_Game>();  

		if (Instance)  
		{
			Instance->AddToRoot();
			Instance->Initialize(Inworld);  
		}
	}

	return Instance;
}




void UCUIManager_Game::HPprogressbarChange(float RemainHP, float DamageAmount, float MaxHP)
{
	UProgressBar* HPprogressbar = Cast<UProgressBar>(PlayerUI->GetWidgetFromName(TEXT("HP")));

	if (HPprogressbar != nullptr)
	{
		float percentage = (RemainHP / MaxHP);
		HPprogressbar->SetPercent(percentage);
	}
}

void UCUIManager_Game::MPprogressbarChange(float RemainMP, float UsedMPAmount, float MaxMP)
{
	UProgressBar* HPprogressbar = Cast<UProgressBar>(PlayerUI->GetWidgetFromName(TEXT("MP")));
	if (HPprogressbar != nullptr)
	{
		float percentage = (RemainMP / MaxMP);
		HPprogressbar->SetPercent(percentage);
	}

}

void UCUIManager_Game::ChangedWeaponImage(int8 TypeNum)
{
	switch (TypeNum)
	{
	case 0:
	{
		UImage* Sword = Cast<UImage>(PlayerUI->GetWidgetFromName(TEXT("Sword")));
		Sword->SetRenderOpacity(1);
		CurrentWeaponImage->SetRenderOpacity(0);
		CurrentWeaponImage = Sword;
		bUnarmd = false;
		SkillTextureUpdate();
		SkillUI->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	case 1:
	{
		UImage* Fist = Cast<UImage>(PlayerUI->GetWidgetFromName(TEXT("Fist")));
		Fist->SetRenderOpacity(1);
		CurrentWeaponImage->SetRenderOpacity(0);
		CurrentWeaponImage = Fist;
		bUnarmd = false;
		SkillTextureUpdate();
		SkillUI->SetVisibility(ESlateVisibility::Visible);
		break;
	}
	case 2:
	{
		UImage* Bow = Cast<UImage>(PlayerUI->GetWidgetFromName(TEXT("Bow")));
		Bow->SetRenderOpacity(1);
		CurrentWeaponImage->SetRenderOpacity(0);
		CurrentWeaponImage = Bow;
		bUnarmd = false;
		SkillTextureUpdate();
		SkillUI->SetVisibility(ESlateVisibility::Visible);
		break;
	}

	case 3:
	{
		UImage* Unarmed = Cast<UImage>(PlayerUI->GetWidgetFromName(TEXT("Unarmed")));
		Unarmed->SetRenderOpacity(1);
		CurrentWeaponImage->SetRenderOpacity(0);
		CurrentWeaponImage = Unarmed;
		bUnarmd = true;
		SkillUI->SetVisibility(ESlateVisibility::Collapsed);
		break;
	}

	}

}



void UCUIManager_Game::SkillTextureUpdate()
{
	TArray<UCSubAction_RightMouse*>* CurrentSubActiondata = weapon->GetSubAction();
	bool QExist = false;
	bool EExist = false;
	bool FExist = false;
	bool MouseRExist = false;


	if (CurrentSubActiondata == nullptr)
		return;

	for (int32 i = 0; i < CurrentSubActiondata->Num(); i++)
	{
		FKey KeyName = (*CurrentSubActiondata)[i]->GetInSubAction_KeyNumber();
	
		if (KeyName == EKeys::RightMouseButton)
		{
			UImage* MRImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Mouse_R_Image")));
			UTexture2D* currentImage = (*CurrentSubActiondata)[i]->GetSkillImage();
			
			if (MRImage != nullptr && currentImage != nullptr)
			{
				MRImage->SetBrushFromTexture(currentImage);
				MouseRExist = true;
			}

		}
		else if (KeyName == EKeys::Q)
		{
			UImage* QImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Skill_Q_Image")));
			UTexture2D* currentImage = (*CurrentSubActiondata)[i]->GetSkillImage();

			if (QImage != nullptr && currentImage != nullptr)
			{
				QImage->SetBrushFromTexture(currentImage);
				QExist = true;
			}
			SkillQ_MaxCoolTime = (*CurrentSubActiondata)[i]->GetMaxCoolTime();
			SkillQ_Handler = (*CurrentSubActiondata)[i]->GetHandler();
		}
		else if (KeyName == EKeys::E)
		{
			UImage* EImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Skill_E_Image")));
			UTexture2D* currentImage = (*CurrentSubActiondata)[i]->GetSkillImage();

			if (EImage != nullptr && currentImage != nullptr)
			{
				EImage->SetBrushFromTexture(currentImage);
				EExist = true;
			}
			SkillE_MaxCoolTime = (*CurrentSubActiondata)[i]->GetMaxCoolTime();
			SkillE_Handler = (*CurrentSubActiondata)[i]->GetHandler();
		}
		else if (KeyName == EKeys::F)
		{
			SkillF_MaxCoolTime = (*CurrentSubActiondata)[i]->GetMaxCoolTime();
			SkillF_Handler = (*CurrentSubActiondata)[i]->GetHandler();
			FExist = true;
		}

	}

	if (QExist == false)
	{
		UImage* QImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Skill_Q_Image")));
		QImage->SetBrushFromTexture(NoExistSkilltexture);
	}
	if (EExist == false)
	{
		UImage* EImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Skill_E_Image")));
		EImage->SetBrushFromTexture(NoExistSkilltexture);
	}
	if (FExist == false)
	{
		UImage* FImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Skill_F_Image")));
		FImage->SetBrushFromTexture(NoExistSkilltexture);
	}
	if (MouseRExist == false)
	{
		UImage* MouseRImage = Cast<UImage>(SkillUI->GetWidgetFromName(TEXT("Mouse_R_Image")));
		MouseRImage->SetBrushFromTexture(NoExistSkilltexture);
	}
}

void UCUIManager_Game::ShutDown()
{
	if (Instance)
	{
		if (Instance->DialogueUI)
		{
			Instance->DialogueUI->RemoveFromParent();
			Instance->DialogueUI = nullptr;
		}

		if (Instance->PlayerUI)
		{
			Instance->PlayerUI->RemoveFromParent();
			Instance->PlayerUI = nullptr;
		}

		if (Instance->QuestUI)
		{
			Instance->QuestUI->RemoveFromParent();
			Instance->QuestUI = nullptr;
		}

		if (Instance->ItemBoxUI)
		{
			Instance->ItemBoxUI->RemoveFromParent();
			Instance->ItemBoxUI = nullptr;
		}

		if (Instance->SkillUI)
		{
			Instance->SkillUI->RemoveFromParent();
			Instance->SkillUI = nullptr;
		}

		Instance->CurrentInNPC = nullptr;
		Instance->CurrentWeaponImage = nullptr;
		Instance->CurrentQuest = nullptr;

		if (Instance->IsRooted())
		{
			Instance->RemoveFromRoot();
		}
		Instance = nullptr;
	}
}



