#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractiveComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class UNREAL_PT_KYB_API UCInteractiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UCInteractiveComponent();

private:
	class ACharacter* OwnerCharacter;
	class APlayerController* PlayerController;


protected:
	virtual void BeginPlay() override;

public:	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	void CheckInteractive();

private:
	UPROPERTY(EditAnywhere)
	float InteractionDistance = 600.0f;

private:
	TSet<UPrimitiveComponent*> OnOutlineResult;
		
};
