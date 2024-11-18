#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "UObject/NameTypes.h"
#include "CAttachment.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FiveParams(FAttachmentBeginOverlap, class ACharacter*, InAttacker, AActor*, InAttackCauser, class ACharacter*, InOther, const FHitResult&, SweepResult, UPrimitiveComponent*, OverlapComponent);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FAttachmentEndOverlap, class ACharacter*, InAttacker, class ACharacter*, InOther);


UCLASS()
class UNREAL_PT_KYB_API ACAttachment : public AActor
{
	GENERATED_BODY()
	
public:	
	ACAttachment();

protected:
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class USceneComponent* Root;
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	class ACharacter* OwnerCharacter;

	UFUNCTION(BlueprintCallable)
	void AttachTo(FName InSocketName);


	UFUNCTION(BlueprintCallable)
	void AttachToCillision(FName InCollisionName);

protected:
	virtual void BeginPlay() override;

	//두개의 이벤트 모두 웨폰 에셋에서 바인딩 되었다(참고)
	//아래는 두 함수는 단순히 블프에서 이벤트를 실행시키기 위한 함수이다.
public:
	UFUNCTION(BlueprintNativeEvent)
		void OnBeginEquip();
	virtual void OnBeginEquip_Implementation() {}

	UFUNCTION(BlueprintNativeEvent)
		void OnUnEquip();
	virtual void OnUnEquip_Implementation() {}
	
	//콜리전 온오프 함수
private:
	TArray<class UShapeComponent*> Collisions;
	class UShapeComponent* CurrentSweepTraceCollision;

protected:
	void Tick(float DeltaSeconds) override;
	bool bSweepOn = false;

public:
	void OnWeaponSweepTraceOverlap();

	void OnCollision(FName InName);
	void OffCollision(FName InName);

	void OnSweepTrace(FName InName);
	void OffSweepTrace(FName InName);


	
	UFUNCTION()
	void OnComponentBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);
	UFUNCTION()
	void OnComponentEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	FAttachmentBeginOverlap OnAttachmentBeginOverlap;
	FAttachmentEndOverlap OnAttachmentEndOverlap;

};
