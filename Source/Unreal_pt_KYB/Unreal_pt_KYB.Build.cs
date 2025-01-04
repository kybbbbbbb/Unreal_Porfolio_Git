// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class Unreal_pt_KYB : ModuleRules
{
	public Unreal_pt_KYB(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
	
		PublicIncludePaths.Add(ModuleDirectory);
		
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayCameras", "GameplayTasks", "NavigationSystem", "Niagara", "UMG", "AssetRegistry", "GeometryCollectionEngine" });


		PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore", "UnrealEd", "LevelEditor"  });

	}
}
