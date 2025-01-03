#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CInteractableObject.h"
#include "CInteractive_Object.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: ACInteractive_Object
  @ 설  명: 상호작용 가능한 오브젝트 제작 시 사용
  @ 콜루트:
  @ TODO: 

------------------------------------------------------------------------*/

UCLASS()
class UNREAL_PT_KYB_API ACInteractive_Object : public AActor, public ICInteractableObject
{
	GENERATED_BODY()
	
public:	
	ACInteractive_Object();

protected:
	UPROPERTY(EditAnywhere, Category = "Interactive")
	bool isdisposable = false;

protected:
	virtual void BeginPlay() override;

public:
	// ICInteractableObject을 통해 상속됨
	void Interact() override;

	UFUNCTION(BlueprintNativeEvent, Category = "Interactive")
	void OnInteract();
	virtual void OnInteract_Implementation();

protected:
	//isdisposable이 true인 경우만 사용해야한다.
	UPROPERTY(BlueprintReadOnly)
	bool bOnaction = false;

};
