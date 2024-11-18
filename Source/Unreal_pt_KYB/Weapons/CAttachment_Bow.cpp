#include "Weapons/CAttachment_Bow.h"
#include "Components/PoseableMeshComponent.h"
#include "CAnimInstance_Bow.h"
#include "GameFramework/Character.h"
ACAttachment_Bow::ACAttachment_Bow()
{
	SkeletalMeshComponent = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMesh");
	SkeletalMeshComponent->SetupAttachment(Root);
	PoseableMesh = CreateDefaultSubobject<UPoseableMeshComponent>("PoseableMesh");
	PoseableMesh->SetupAttachment(Root);

	USkeletalMesh* mesh;
	mesh = ConstructorHelpers::FObjectFinder<USkeletalMesh>(L"/Script/Engine.SkeletalMesh'/Game/Weapon/ShortBow_01/SK_ShortBow_01.SK_ShortBow_01'").Object;

	SkeletalMeshComponent->SetSkeletalMesh(mesh);
	SkeletalMeshComponent->SetCollisionProfileName("NoCollision");

	TSubclassOf<UCAnimInstance_Bow> AnimInstance;
	AnimInstance = ConstructorHelpers::FClassFinder<UCAnimInstance_Bow>(L"/Script/Engine.AnimBlueprint'/Game/Characters/01_Player/Animation/Bow/CAnimInstance_Bow.CAnimInstance_Bow_C'").Class;
	SkeletalMeshComponent->SetAnimInstanceClass(AnimInstance);


}
void ACAttachment_Bow::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


float* ACAttachment_Bow::GetAniminstance_Bending()
{
	UCAnimInstance_Bow* bow = Cast<UCAnimInstance_Bow>(SkeletalMeshComponent->GetAnimInstance());
	if (bow != nullptr)
		return bow->GetBending();
	return nullptr;
}


void ACAttachment_Bow::OnBeginEquip_Implementation()
{
	Super::OnBeginEquip_Implementation();

	AttachTo("Hand_Bow");

	APlayerController* controller = Cast<APlayerController>(OwnerCharacter->GetController<APlayerController>());
	
	if (controller == nullptr)
	{
		return;
	}

	OriginViewPitchRange.X = controller->PlayerCameraManager->ViewPitchMin;
	OriginViewPitchRange.Y = controller->PlayerCameraManager->ViewPitchMax;

	controller->PlayerCameraManager->ViewPitchMin = ViewPitchRange.X;
	controller->PlayerCameraManager->ViewPitchMax = ViewPitchRange.Y;


}

void ACAttachment_Bow::OnUnEquip_Implementation()
{
	Super::OnUnEquip_Implementation();
	
	AttachTo("Holster_Bow");
	APlayerController* controller = OwnerCharacter->GetController<APlayerController>();

	if (controller == nullptr)
		return;

	controller->PlayerCameraManager->ViewPitchMin = OriginViewPitchRange.X;
	controller->PlayerCameraManager->ViewPitchMax = OriginViewPitchRange.Y;
}

void ACAttachment_Bow::BeginPlay()
{
	Super::BeginPlay();

	AttachTo("Holster_Bow");

	SkeletalMeshComponent->SetVisibility(false);

	PoseableMesh->SetSkeletalMesh(SkeletalMeshComponent->SkeletalMesh);
	PoseableMesh->CopyPoseFromSkeletalComponent(SkeletalMeshComponent);
}
