// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FinalYB_DebugPlugin : ModuleRules
{
	public FinalYB_DebugPlugin(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;
		

			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
				"Core",
                "CoreUObject", 
				"Engine", 
				"InputCore", 
				"Slate", 
				"SlateCore", 
				"EditorStyle", 
				"UnrealEd", 
				"LevelEditor"
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
			}
			);

	}
}
