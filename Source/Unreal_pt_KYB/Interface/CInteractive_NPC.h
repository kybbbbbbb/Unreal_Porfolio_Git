#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractableObject.h"
#include "../GameManager/CQuestData.h"
#include "CInteractive_NPC.generated.h"

UCLASS()
class UNREAL_PT_KYB_API ACInteractive_NPC : public AActor , public ICInteractableObject
{
	GENERATED_BODY()
	
public:	
	ACInteractive_NPC();

protected:
	virtual void BeginPlay() override;

private:
	UPROPERTY(EditAnywhere)
	FText NPCName;
	UPROPERTY(EditAnywhere)
	FText NPC_Standard_Dialogue;
	UPROPERTY(EditAnywhere)
	UDataTable* QuestTable;

public:	
	virtual void Tick(float DeltaTime) override;
	void Interact() override;

	UFUNCTION()
	void AcceptQuest_NPC();

	FORCEINLINE UDataTable* GetQuest() { return QuestTable; }
	FORCEINLINE const FText GetNPCName() { return NPCName; }
	FORCEINLINE const FText GetStandardDialogue() { return NPC_Standard_Dialogue; }


	UFUNCTION(BlueprintNativeEvent, Category = "Interactive")
	void OnInteract();
	virtual void OnInteract_Implementation();

public:
	void PlayerStop();
	void PlayerMove();

protected:
	UPROPERTY(BlueprintReadOnly)
	bool bOnaction = false;

	UPROPERTY(EditAnywhere)
	class AActor* viewTarget;



};
