#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "CQuestData.generated.h"


USTRUCT()
struct FQuest
{
	GENERATED_BODY()

public:
	FText QuestName;

	FText QuestDescription;

	bool bIsAccepted = false; 

	int32 Count = 0;
	int32 GoalCount = 0;

};

USTRUCT()
struct FQuestTable : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	FText Title;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	FText Description;
	UPROPERTY(EditAnywhere, BlueprintReadWrite) 
	int32 GoalCount;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	FVector Location;
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	bool bIsAccepted = false;

	void AcceptQuest();
	FORCEINLINE bool GetIsAccepted() { return bIsAccepted; }
};



/*------------------------------------------------------------------------

  @ 이  름: UCQuestData
  @ 설  명: 퀘스트 데이터 저장용 UObject 클래스
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS(Blueprintable)
class UNREAL_PT_KYB_API UCQuestData : public UObject
{
	GENERATED_BODY()

public:
	UCQuestData();
	
public:


};
