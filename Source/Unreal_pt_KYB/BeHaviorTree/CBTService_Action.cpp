// Fill out your copyright notice in the Description page of Project Settings.


#include "BeHaviorTree/CBTService_Action.h"

UCBTService_Action::UCBTService_Action()
{
	NodeName = "Action_Divider";

	Interval = 0.1f;
	RandomDeviation = 0.0f;

}

void UCBTService_Action::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{


}
