#include "CustomDebugging_YB.h"
#include "SCustomDebuggingWindow.h"
#include "Widgets/Docking/SDockTab.h"
#include "Widgets/Layout/SBox.h"
#include "Engine/World.h"
#include "GameFramework/Actor.h"

#define LOCTEXT_NAMESPACE "FCustomDebugging_YBModule"

void FCustomDebugging_YBModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner("CustomDebuggingWindow", FOnSpawnTab::CreateRaw(this, &FCustomDebugging_YBModule::OnSpawnPluginTab))
		.SetDisplayName(LOCTEXT("CustomDebuggingWindowTitle", "Custom Debugging"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
}

void FCustomDebugging_YBModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("CustomDebuggingWindow");
}

void FCustomDebugging_YBModule::RegisterMenus()
{
}

TSharedRef<class SDockTab> FCustomDebugging_YBModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(SCustomDebuggingWindow)
		];
	
}

#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FCustomDebugging_YBModule, CustomDebugging_YB)