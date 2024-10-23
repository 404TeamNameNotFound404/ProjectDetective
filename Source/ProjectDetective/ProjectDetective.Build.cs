// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ProjectDetective : ModuleRules
{
	public ProjectDetective(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			PublicDefinitions.Add("USE_LINUX_SPECIFIC_CODE=1");
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			PublicDefinitions.Add("USE_WINDOWS_SPECIFIC_CODE=1");
		}
		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "GameplayTags" });
		
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;
		
	}
}
