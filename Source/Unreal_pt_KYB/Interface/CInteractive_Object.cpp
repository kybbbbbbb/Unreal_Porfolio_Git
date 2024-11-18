#include "Interface/CInteractive_Object.h"

ACInteractive_Object::ACInteractive_Object()
{
 
}

void ACInteractive_Object::BeginPlay()
{
	Super::BeginPlay();
	
}

void ACInteractive_Object::Interact()
{
	if (isdisposable == true && bOnaction == true)
		return;

	if (bOnaction == false)
	{
		bOnaction = true;
		OnInteract();
		return;
	}
	if (bOnaction == true)
	{
		bOnaction = false;
		OnInteract();
	}

}

void ACInteractive_Object::OnInteract_Implementation()
{
	
}



