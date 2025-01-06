// Copyright Epic Games, Inc. All Rights Reserved.

#include "FinalYB_DebugPlugin.h"
#include "LevelEditor.h"
#include "DebugWidget_YB.h"

#define LOCTEXT_NAMESPACE "FFinalYB_DebugPluginModule"

void FFinalYB_DebugPluginModule::StartupModule()
{
	FGlobalTabmanager::Get()->RegisterNomadTabSpawner("FinalYB_Debug", FOnSpawnTab::CreateRaw(this, &FFinalYB_DebugPluginModule::OnSpawnPluginTab))
		.SetDisplayName(FText::FromString("FinalYB Debug Plugin"))
		.SetMenuType(ETabSpawnerMenuType::Enabled);
}

void FFinalYB_DebugPluginModule::ShutdownModule()
{
	FGlobalTabmanager::Get()->UnregisterNomadTabSpawner("FinalYB_Debug");
}

TSharedRef<SDockTab> FFinalYB_DebugPluginModule::OnSpawnPluginTab(const FSpawnTabArgs& SpawnTabArgs)
{
	return SNew(SDockTab)
		.TabRole(ETabRole::NomadTab)
		[
			SNew(DebugWidget_YB)
		];
}


#undef LOCTEXT_NAMESPACE
	
IMPLEMENT_MODULE(FFinalYB_DebugPluginModule, FinalYB_DebugPlugin)