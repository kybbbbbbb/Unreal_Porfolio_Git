#include "GameManager/CSoundManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/AudioComponent.h"

UCSoundManager* UCSoundManager::Instance = nullptr;

UCSoundManager::UCSoundManager()
{
	UDataTable* DT_SoundData = ConstructorHelpers::FObjectFinder<UDataTable>(L"/Script/Engine.DataTable'/Game/04_Object/Sound/SoundData.SoundData'").Object;
	if (DT_SoundData != nullptr)
	{
		SoundDataTable = DT_SoundData;
	}
}

UCSoundManager* UCSoundManager::GetInstance()
{
	if (Instance == nullptr)
	{
		Instance = NewObject<UCSoundManager>();
		Instance->AddToRoot();
		Instance->LoadSounds();
	}
	return Instance;
}

void UCSoundManager::PlaySound2D(const FName& SoundName, UWorld* InWorld)
{
	if (USoundCue* Sound = SoundMap.FindRef(SoundName))
	{
		// ������ ī�װ��� ���� ã��
		FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundName, TEXT(""));
		if (SoundData)
		{
			// ī�װ��� �ش��ϴ� ���� ��������
			float Volume = CategoryMap.FindRef(SoundData->Category);

			// ���� ���
			UGameplayStatics::PlaySound2D(InWorld, Sound, Volume);
		}
		else
		{
			// SoundData�� ������ �⺻ ����(1.0f)���� ���
			UGameplayStatics::PlaySound2D(InWorld, Sound, 1.0f);
		}
	}
}

void UCSoundManager::PlaySound3D(const FName& SoundName, const FVector& Location, UWorld* InWorld)
{
	if (USoundCue* Sound = SoundMap.FindRef(SoundName))
	{
		FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundName, TEXT(""));
		if (SoundData)
		{
			float FinalVolume = CategoryMap[SoundData->Category];

			// ���� ����
			USoundAttenuation* Attenuation = NewObject<USoundAttenuation>();
			Attenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Linear;  
			// �Ÿ� ����
			Attenuation->Attenuation.FalloffDistance = 3000.0f;     
			Attenuation->Attenuation.AttenuationShapeExtents = FVector(1000.0f); 

			UGameplayStatics::PlaySoundAtLocation(
				InWorld,
				Sound,
				Location,
				FRotator::ZeroRotator,
				FinalVolume,
				1.0f,
				0.0f,
				Attenuation
			);
		}
	}
}

void UCSoundManager::PlayFootstep(const FName& SurfaceType)
{
	if (USoundCue* Sound = SoundMap.FindRef(FName(*FString::Printf(TEXT("Footstep_%s"), *SurfaceType.ToString()))))
	{
		UGameplayStatics::PlaySound2D(GetWorld(), Sound, CategoryMap.FindRef("Footstep"));
	}
}

void UCSoundManager::PlayBGM(const FName& SoundName, UWorld* InWorld)
{
	if (USoundCue* Sound = SoundMap.FindRef(SoundName))
	{
		FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundName, TEXT(""));
		float Volume = 1.0f; // �⺻ ����

		if (SoundData)
		{
			Volume = CategoryMap.FindRef(SoundData->Category);
		}

		// BGM�� �̹� ��� ������ Ȯ��
		if (BGMComponent && BGMComponent->IsPlaying())
		{
			return; // �̹� ��� ���̸� ���ο� ����� ����
		}

		// ���ο� UAudioComponent ���� �� BGM ���
		BGMComponent = UGameplayStatics::SpawnSound2D(InWorld, Sound, Volume);
	}
}

float UCSoundManager::GetVolume(const FName& Category)
{
	return 0.0f;
}

void UCSoundManager::LoadSounds()
{
	if (!SoundDataTable)
	{
		return;
	}

	for (auto It : SoundDataTable->GetRowMap())
	{
		//Ű�� ���������̺� �̸�
		FName SoundName = It.Key;
		FSoundData* SoundData = (FSoundData*)It.Value;

		if (SoundData->SoundCue.IsNull() == false)
		{

			USoundCue* LoadedSound = SoundData->SoundCue.LoadSynchronous();

			if (LoadedSound)
			{
				SoundMap.Add(SoundName, LoadedSound);

				if (!CategoryMap.Contains(SoundData->Category))
				{
					CategoryMap.Add(SoundData->Category, SoundData->DefaultVolume);
				}
			}
		}
	}

}

//Uobject �̱��� ���� �Լ�, ������ �� ������ ��� ���⼭ ��ȭ
void UCSoundManager::ShutDown()
{
	if (Instance)
	{
		Instance->SoundMap.Empty();
		Instance->CategoryMap.Empty();
		Instance->SoundDataTable = nullptr;

		if (Instance->IsRooted())
		{
			Instance->RemoveFromRoot();
		}
		Instance = nullptr;
	}

}
