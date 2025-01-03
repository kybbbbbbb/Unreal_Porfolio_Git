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
		// 사운드의 카테고리와 볼륨 찾기
		FSoundData* SoundData = SoundDataTable->FindRow<FSoundData>(SoundName, TEXT(""));
		if (SoundData)
		{
			// 카테고리에 해당하는 볼륨 가져오기
			float Volume = CategoryMap.FindRef(SoundData->Category);

			// 사운드 재생
			UGameplayStatics::PlaySound2D(InWorld, Sound, Volume);
		}
		else
		{
			// SoundData가 없으면 기본 볼륨(1.0f)으로 재생
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

			// 감쇠 설정
			USoundAttenuation* Attenuation = NewObject<USoundAttenuation>();
			Attenuation->Attenuation.DistanceAlgorithm = EAttenuationDistanceModel::Linear;  
			// 거리 설정
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
		float Volume = 1.0f; // 기본 볼륨

		if (SoundData)
		{
			Volume = CategoryMap.FindRef(SoundData->Category);
		}

		// BGM이 이미 재생 중인지 확인
		if (BGMComponent && BGMComponent->IsPlaying())
		{
			return; // 이미 재생 중이면 새로운 재생을 막음
		}

		// 새로운 UAudioComponent 생성 및 BGM 재생
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
		//키는 데이터테이블 이름
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

//Uobject 싱글톤 삭제 함수, 포인터 형 변수는 모두 여기서 널화
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
