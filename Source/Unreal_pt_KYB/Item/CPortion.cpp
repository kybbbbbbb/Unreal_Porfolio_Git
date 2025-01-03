#include "Item/CPortion.h"
#include "../Characters/CPlayer.h"
#include "InventorySystem.h"

ACPortion::ACPortion()
{

}

void ACPortion::BeginPlay()
{
	Super::BeginPlay();
}

void ACPortion::Interact()
{
	ACPlayer* player = Cast<ACPlayer>(GetWorld()->GetFirstPlayerController()->GetCharacter());
	if (player == nullptr)
		return;
	UInventorySystem* inventory = player->GetInventory();
	if (inventory != nullptr)
	{
		inventory->AddItem(itemID);
	}

	Destroy();
}

void ACPortion::SetitemID(FPrimaryAssetId InitemID)
{
	itemID = InitemID;
}

