// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class ShibAvatarBuilder : ModuleRules
{
	public ShibAvatarBuilder(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicIncludePaths.AddRange(
			new string[] {
				PluginDirectory,
				Path.Combine(PluginDirectory, "Source/ShibAvatarBuilder/Public"),
				Path.Combine(PluginDirectory, "Source/ShibAvatarBuilder/Public/UI"),
				Path.Combine(PluginDirectory, "Source/ShibAvatarBuilder/Public/Core"),
				Path.Combine(PluginDirectory, "Source/ShibAvatarBuilder/Public/Animation"),
				Path.Combine(PluginDirectory, "Source/ShibAvatarBuilder/Public/Save"),
			}
			);
				
		
		PrivateIncludePaths.AddRange(
			new string[] {
				// ... add other private include paths required here ...
			}
			);
			
		
		PublicDependencyModuleNames.AddRange(
			new string[]
			{
                "Core",
                "UMG",
                "Json",
                "JsonUtilities",
				// ... add other public dependencies that you statically link with here ...
			}
			);
			
		
		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
                "SlateCore",
                "EnhancedInput",
                "InputCore",
				// ... add private dependencies that you statically link with here ...	
			}
			);
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
			);
	}
}
