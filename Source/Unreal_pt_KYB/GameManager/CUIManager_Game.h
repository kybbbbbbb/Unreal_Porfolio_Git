#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "../Interface/CInteractive_NPC.h"
#include "CUserWidget.h"
#include "Components/Image.h"
#include "../Item/CItemDataAsset.h"
#include "../Components/CWeaponComponent_reset.h"
#include "CUIManager_Game.generated.h"

/*------------------------------------------------------------------------
 
  @ Ŭ����: UCUIManager_Game
  @ ��  ��: �̱������� ����
  @ �ݷ�Ʈ: �̱��� GetInstance
  @ TODO: X
 
------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCUIManager_Game : public UObject
{
	GENERATED_BODY()

public:
	UCUIManager_Game();

	void Initialize(UWorld* World);

	void ShowPlayerUI();
	void HidePlayerUI();

	UFUNCTION()
	void ShowItemUI();

	void HideItemUI();

	void ShowSkillUI();
	void HideskillUI();

	void UpdateItemBox(const TArray<FPrimaryAssetId>& InAssetIDDatas, const TArray<UCItemDataAsset*>& ItemDataAssets);

	void ShowNPCDialogueUI(ACInteractive_NPC* InNPC);

	UFUNCTION()
	void HideNPCDialogueUI();

	//Quest ������ ����
	void QuestUIOn(FQuestTable* Indata);

	UFUNCTION()
	void MonsterDeadDelegate();

	static UCUIManager_Game* GetInstance(UWorld* Inworld);

private:
	UPROPERTY()
	UCUserWidget* DialogueUI = nullptr;

	UPROPERTY()
	UUserWidget* PlayerUI = nullptr;

	UPROPERTY()
	UUserWidget* QuestUI = nullptr;

	UPROPERTY()
	UUserWidget* ItemBoxUI = nullptr;

	UPROPERTY()
	UUserWidget* SkillUI = nullptr;

	TSubclassOf<UCUserWidget> DialogueUIclass;
	TSubclassOf<UUserWidget> PlayerUIclass;
	TSubclassOf<UUserWidget> QuestUIclass;
	TSubclassOf<UUserWidget> ItemBoxUIclass;
	TSubclassOf<UUserWidget> SkillUIclass;

	
	static UCUIManager_Game* Instance;

	UPROPERTY()
	ACInteractive_NPC* CurrentInNPC;

	UFUNCTION()
	void HPprogressbarChange(float RemainHP, float DamageAmount, float MaxHP);

	UFUNCTION()
	void MPprogressbarChange(float RemainMP, float UsedMPAmount, float MaxMP);

	UFUNCTION()
	void ChangedWeaponImage(int8 TypeNum);



	void SkillTextureUpdate();
private:
	int8 CurrentQuestRowindex = 0;

	UPROPERTY()
	UImage* CurrentWeaponImage;

	int8 CurrentQuestValue = 0;
	FQuestTable* CurrentQuest;

	UPROPERTY()
	UCWeaponComponent_reset* weapon;
	UPROPERTY()
	UTexture2D* NoExistSkilltexture;
	UPROPERTY()
	UWorld* InWorld;

	bool bOnitemInventory = false;
	bool bUnarmd = true;

public:
	static void ShutDown();
		
};