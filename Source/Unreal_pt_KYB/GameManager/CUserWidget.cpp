#include "GameManager/CUserWidget.h"
#include "Components/Button.h"

void UCUserWidget::NativeConstruct()
{
	// YourButton�� ����� ���ε��Ǿ����� Ȯ�� �� Ŭ�� �̺�Ʈ ���ε�
	if (LeftYesButton)
	{
		LeftYesButton->OnClicked.AddDynamic(this, &UCUserWidget::OnButtonClick);
	}

}

void UCUserWidget::OnButtonClick()
{

}
