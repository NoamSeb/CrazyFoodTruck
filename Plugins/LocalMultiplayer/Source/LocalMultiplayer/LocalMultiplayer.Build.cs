// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class LocalMultiplayer : ModuleRules
{
	public LocalMultiplayer(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
		PublicDependencyModuleNames.AddRange(new string[]
		{
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"DeveloperSettings"
		});
		
		PrivateDependencyModuleNames.AddRange(new string[]
		{

		});
	}
}
