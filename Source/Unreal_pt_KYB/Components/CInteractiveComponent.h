#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "CInteractiveComponent.generated.h"

/*------------------------------------------------------------------------

  @ 이  름: UCInteractiveComponent
  @ 설  명: 상호작용 가능하게 해주는 컴포넌트, 레이캐스트 사용
  @ 콜루트: Player-> Press T -> Interactive
  @ TODO:

------------------------------------------------------------------------*/
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
	void IsRiding(bool Inbool);
	void CheckInteractive();

private:
	UPROPERTY(EditAnywhere)
	float InteractionDistance = 600.0f;
	bool isRiding = false;
private:
	TSet<UPrimitiveComponent*> OnOutlineResult;
		
};
