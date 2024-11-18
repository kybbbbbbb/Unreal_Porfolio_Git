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
	FName Category;  // "SFX", "BGM", "Footstep" µî
};

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

	void PlaySound2D(const FName& SoundName, UWorld* InWorld);
	void PlaySound3D(const FName& SoundName, const FVector& Location, UWorld* InWorld);
	void PlayFootstep(const FName& SurfaceType);
	void PlayBGM(const FName& SoundName, UWorld* InWorld);

	void SetVolume(const FName& Category, float Volume);
	float GetVolume(const FName& Category);

public:
	void LoadSounds(); 

public:
	static void ShutDown();

};
