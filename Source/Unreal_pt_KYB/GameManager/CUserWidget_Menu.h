#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "CUserWidget_Menu.generated.h"

//사용X클래스
//TODO: 추후 삭제
UCLASS()
class UNREAL_PT_KYB_API UCUserWidget_Menu : public UUserWidget
{
	GENERATED_BODY()
	

//private:
//	UPROPERTY(meta = (BindWidget))
//	class UButton* StartGameButton;
//	UPROPERTY(meta = (BindWidget))
//	class UButton* ExitButton;
//	UPROPERTY(meta = (BindWidget))
//	class UButton* LoadGameButton;
//
//
//public:
//	FORCEINLINE UButton* GetStartGameButton() { return StartGameButton; }
//	FORCEINLINE UButton* GetExitButton() { return ExitButton; }
//	FORCEINLINE UButton* GetLoadGameButton() { return LoadGameButton; }
//protected:
//	virtual void NativeConstruct() override;
//
//private:
//	UFUNCTION()
//	void OnButtonClick();
};
