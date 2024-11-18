#include "GameManager/CUserWidget.h"
#include "Components/Button.h"

void UCUserWidget::NativeConstruct()
{
	// YourButton이 제대로 바인딩되었는지 확인 후 클릭 이벤트 바인딩
	if (LeftYesButton)
	{
		LeftYesButton->OnClicked.AddDynamic(this, &UCUserWidget::OnButtonClick);
	}

}

void UCUserWidget::OnButtonClick()
{

}
