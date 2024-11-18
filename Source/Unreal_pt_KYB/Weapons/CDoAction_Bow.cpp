#include "Weapons/CDoAction_Bow.h"
#include "Components/CStateComponent.h"
#include "GameFramework/Character.h"
#include "CAttachment.h"
#include "CAttachment_Bow.h"
#include "Components/PoseableMeshComponent.h"
#include "AddOns/CArrow.h"
#include "Kismet/GameplayStatics.h"
#include "CEquipment.h"
#include "Weapons/CSubAction_RightMouse.h"

UCDoAction_Bow::UCDoAction_Bow()
{

}

void UCDoAction_Bow::BeginPlay(ACharacter* Owner, class ACAttachment* InAttachment, class UCEquipment* InEquipment, const TArray<FDoActionData>& InData, TArray<FHitData>& InHitDatas, TArray<class UCSubAction_RightMouse*>& InSubActionData)
{
	Super::BeginPlay(Owner, InAttachment, InEquipment, InData, InHitDatas, InSubActionData);

	SkeletalMesh = Cast<USkeletalMeshComponent>(InAttachment->GetComponentByClass(USkeletalMeshComponent::StaticClass()));
	PosableMesh = Cast<UPoseableMeshComponent>(InAttachment->GetComponentByClass(UPoseableMeshComponent::StaticClass()));


	OriginLocation = PosableMesh->GetBoneLocationByName("String", EBoneSpaces::ComponentSpace);
	bEquipped = InEquipment->GetEquipped();


	ACAttachment_Bow* bow = Cast<ACAttachment_Bow>(InAttachment);
	Bending = bow->GetAniminstance_Bending();
}

void UCDoAction_Bow::DoAction()
{
	if(State->IsIdleMode() == false) return;
	if(State->IsEquipMode() == true) return;
	if (State->IsSubActionMode() == false)
	{
		Super::DoAction();
		DoActionDatas[0].DoAction(OwnerCharacter);
		return;
	}
	
	if (isReload == false)
		return;
	isReload = false;
	Super::DoAction();


	ESubActionEnum skillnumber = GetCurrentSubActionSkillNumber();
	if (skillnumber != ESubActionEnum::NoExistSkill)
	{
		DoActionDatas[(int8)skillnumber].DoAction(OwnerCharacter);
	}


	for (UCSubAction_RightMouse* subAction : *SubActionData)
	{
		if (subAction->GetInSubAction_RightMouse() == true)
		{
			subAction->StopSubAction();
		}
	}

}

void UCDoAction_Bow::Begin_DoAction()
{
	Super::Begin_DoAction();

	*Bending = 0.0f;
	bAttachedString = false;


	ACArrow* arrow = GetAttachedArrow();
	arrow->DetachFromActor(FDetachmentTransformRules(EDetachmentRule::KeepWorld, true));


	arrow->OnHit.AddDynamic(this, &UCDoAction_Bow::OnArrowHit);
	arrow->OnEndPlay.AddDynamic(this, &UCDoAction_Bow::OnArrowEndPlay);
	
	FVector forward = FQuat(OwnerCharacter->GetControlRotation()).GetForwardVector();

	arrow->Shoot(forward);

	AttackedActor.Empty();


}



void UCDoAction_Bow::End_DoAction()
{
	Super::End_DoAction();

	isReload = false;
	CreateArrow();
	AttackedActor.Empty();
}

void UCDoAction_Bow::OnBeginEquip()
{
	Super::OnBeginEquip();

	CreateArrow();
}

void UCDoAction_Bow::OnUnEquip()
{
	Super::OnUnEquip();
	PosableMesh->SetBoneLocationByName("String", OriginLocation, EBoneSpaces::ComponentSpace);
	for (int32 i = Arrows.Num() - 1; i >= 0; i--)
	{
		ACArrow* arrow = Arrows[i];

		if (Arrows[i]->IsShoot() == true)
		{
			Arrows.Remove(arrow);
			arrow->Destroy();
		}
	}

	bAttachedString = false;
}

void UCDoAction_Bow::OnArrowHit(AActor* InCauser, ACharacter* InOtherCharacter, const FHitResult& Hit)
{
	if(HitDatas.Num() < 1) return;

	
	HitDatas[10].SendDamage(OwnerCharacter, InCauser, InOtherCharacter, Hit , nullptr);
}

void UCDoAction_Bow::OnArrowEndPlay(ACArrow* InDestroyer)
{
	Arrows.Remove(InDestroyer);
}

void UCDoAction_Bow::Tick(float InDeltaTime)
{
	Super::Tick(InDeltaTime);

	PosableMesh->CopyPoseFromSkeletalComponent(SkeletalMesh);
	
	if (*bEquipped == false)
		return;
	
	//추후 노티파이로 붙는 시점 함수로 전달
	if (bAttachedString == true)
	{
		
		FVector handLocation = OwnerCharacter->GetMesh()->GetSocketLocation("Hand_Bow_Right");
		PosableMesh->SetBoneLocationByName("String", handLocation, EBoneSpaces::WorldSpace);
		
	}

	

}

void UCDoAction_Bow::OnAttachmentBeginOverlap(ACharacter* InAttacker, AActor* InAttackCauser, ACharacter* InOther, const FHitResult& SweepResult, UPrimitiveComponent* OverlapComponent)
{
	Super::OnAttachmentBeginOverlap(InAttacker, InAttackCauser, InOther, SweepResult, OverlapComponent);

	if (InOther == nullptr || InOther == OwnerCharacter)
		return;
	if (HitDatas.Num() - 1 < index)
		return;
	if (AttackedActor.Contains(InOther))
		return;


	AttackedActor.Add(InOther);

	HitDatas[index].SendDamage(InAttacker, InAttackCauser, InOther, SweepResult, OverlapComponent);

}

void UCDoAction_Bow::CreateArrow()
{
	//게임이 종료중인지 체크하는 bIsTearingDown
	if (World->bIsTearingDown == true)
		return;
	if (ArrowClass == nullptr)
		return;
	if (!!GetAttachedArrow())
		return;

	FTransform transform;
	ACArrow* arrow = World->SpawnActorDeferred<ACArrow>(ArrowClass, transform, nullptr, nullptr, ESpawnActorCollisionHandlingMethod::AlwaysSpawn);
	
	if (arrow == nullptr)
		return;

	arrow->AddIgnoreActor(OwnerCharacter);

	FAttachmentTransformRules rule = FAttachmentTransformRules(EAttachmentRule::KeepRelative, true);
	arrow->AttachToComponent(OwnerCharacter->GetMesh(), rule, "Hand_Bow_Right_Arrow");
	

	Arrows.Add(arrow);
	//스폰 액터 디퍼드를 최종 스폰하려면 아래를 사용해야한다.
	//액터 생명주기 참고
	UGameplayStatics::FinishSpawningActor(arrow, transform);
	

}

ACArrow* UCDoAction_Bow::GetAttachedArrow()
{
	for (ACArrow* arrow : Arrows)
	{
		if (arrow->IsShoot() == false)
			return arrow;
	}

	
	return nullptr;
}

