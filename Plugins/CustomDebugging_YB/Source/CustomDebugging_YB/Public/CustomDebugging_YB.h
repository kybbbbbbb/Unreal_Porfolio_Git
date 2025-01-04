#pragma once

#include "CoreMinimal.h"
#include "Modules/ModuleManager.h"

class FCustomDebugging_YBModule : public IModuleInterface
{
public:

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;

private:
	void RegisterMenus();
	TSharedRef<class SDockTab> OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs);
};
