#include "Item/InventorySystem.h"
#include "AssetRegistry/AssetRegistryModule.h"
#include "../GameManager/CUIManager_Game.h"


UInventorySystem::UInventorySystem()
{
}

void UInventorySystem::AddItem(FPrimaryAssetId itemID)
{
	items.Add(itemID);
	
	UCUIManager_Game* instance = UCUIManager_Game::GetInstance(GetWorld());
	if (instance == nullptr)
		return;
	instance->UpdateItemBox(items, ItemDataAssets);
}

void UInventorySystem::Beginplay()
{
	//특정 폴더에 아이템을 몰아두고 데이터 가져옴
	FAssetRegistryModule* AssetRegistryModule = FModuleManager::LoadModulePtr<FAssetRegistryModule>("AssetRegistry");

	if (AssetRegistryModule)
	{
		IAssetRegistry& AssetRegistry = AssetRegistryModule->Get();

		TArray<FString> PathsToScan;
		PathsToScan.Add("/Game/04_Object/Item/itemAsset");
		AssetRegistry.ScanPathsSynchronous(PathsToScan);

		FName AssetPath = FName("/Game/04_Object/Item/itemAsset");
		TArray<FAssetData> AssetData;
		AssetRegistry.GetAssetsByPath(AssetPath, AssetData);


		for (const FAssetData& Data : AssetData)
		{
			UCItemDataAsset* ItemDataAsset = Cast<UCItemDataAsset>(Data.GetAsset());
			if (!ItemDataAsset)
			{
				ItemDataAsset = Cast<UCItemDataAsset>(Data.ToSoftObjectPath().TryLoad());
			}

			if (ItemDataAsset != nullptr)
			{
				ItemDataAssets.Add(ItemDataAsset);
			}

		}
	}
	items.Empty();
}
