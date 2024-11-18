#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget.generated.h"

UCLASS()
class UNREAL_PT_KYB_API UCUserWidget : public UUserWidget
{
	GENERATED_BODY()
	

private:
	UPROPERTY(meta = (BindWidget)) 
		class UButton* LeftYesButton;
	UPROPERTY(meta = (BindWidget)) 
		class UButton* RightNoButton; 


public:
	FORCEINLINE UButton* GetLeftYesButton() { return LeftYesButton; }
	FORCEINLINE UButton* GetRightNoButton() { return RightNoButton; }
protected:
	virtual void NativeConstruct() override; 

private:
	UFUNCTION()
	void OnButtonClick(); 

};
