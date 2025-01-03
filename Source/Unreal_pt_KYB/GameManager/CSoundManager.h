#pragma once

#include "CoreMinimal.h"
#include "UObject/NoExportTypes.h"
#include "Sound/SoundCue.h"
#include "CSoundManager.generated.h"

USTRUCT(BlueprintType)
struct FSoundData : public FTableRowBase
{
	GENERATED_BODY()

public:
	UPROPERTY(EditAnywhere)
	TSoftObjectPtr<USoundCue> SoundCue;

	UPROPERTY(EditAnywhere)
	float DefaultVolume = 1.0f;

	UPROPERTY(EditAnywhere)
	FName Category;  // "SFX", "BGM", "Footstep" 등
};

/*------------------------------------------------------------------------

  @ 이  름: UCSoundManager
  @ 설  명: 싱글톤 사운드 매니저, 
  @ 콜루트:
  @ TODO:

------------------------------------------------------------------------*/
UCLASS()
class UNREAL_PT_KYB_API UCSoundManager : public UObject
{
	GENERATED_BODY()
	
public:
	UCSoundManager();
private:
	static UCSoundManager* Instance;

	UPROPERTY()
	TMap<FName, USoundCue*> SoundMap;

	UPROPERTY()
	UDataTable* SoundDataTable;

	UPROPERTY()
	TMap<FName, float> CategoryMap;

	UPROPERTY()
	UAudioComponent* BGMComponent = nullptr;

public:
	static UCSoundManager* GetInstance();
	static void ShutDown();

	void PlaySound2D(const FName& SoundName, UWorld* InWorld);
	void PlaySound3D(const FName& SoundName, const FVector& Location, UWorld* InWorld);
	void PlayFootstep(const FName& SurfaceType);
	void PlayBGM(const FName& SoundName, UWorld* InWorld);

	float GetVolume(const FName& Category);

public:
	void LoadSounds(); 
};
