#include "Notify/CAnimNotify_StopTime.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"

FString UCAnimNotify_StopTime::GetNotifyName_Implementation() const
{

	return "StopTime";
}

void UCAnimNotify_StopTime::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	UWorld* world = MeshComp->GetOwner()->GetWorld();
		//패링시 타임스탑 추가 (20241025)----------------------------------------
	if (world == nullptr)
		return;

	TArray<ACharacter*> characters;
	for (AActor* actor : world->GetCurrentLevel()->Actors)
	{
		ACharacter* character = Cast<ACharacter>(actor);

		if (character)
		{
			character->CustomTimeDilation = 0.05f;
			characters.Add(character);
		}
	}

	UGameplayStatics::SetGlobalTimeDilation(world, 0.2f);

	FTimerDelegate timerDelegate;
	timerDelegate.BindLambda([this, characters, world]()
		{
			for (ACharacter* character : characters)
			{
				if (!!character)
				{
					character->CustomTimeDilation = 1.0f;

				}
			}
			UGameplayStatics::SetGlobalTimeDilation(world, 1.0f);


		});

	FTimerHandle timerHandle;
	world->GetTimerManager().SetTimer(timerHandle, timerDelegate, 0.5f * 0.2f, false);
	//----------------------------------------------------------------------------

}
