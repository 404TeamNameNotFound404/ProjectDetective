// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System.Collections.Generic;

public class ProjectDetectiveTarget : TargetRules
{
	public ProjectDetectiveTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V5;
		IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_4;
		ExtraModuleNames.Add("ProjectDetective");
		
		if (Target.Platform == UnrealTargetPlatform.Linux)
		{
			// Linux-specific settings (Clang is used automatically by Unreal)
			bUsePCHFiles = true;  // Precompiled headers for Linux
			GlobalDefinitions.Add("USE_LINUX_SPECIFIC_CODE=1");
		}
		else if (Target.Platform == UnrealTargetPlatform.Win64)
		{
			// Windows-specific settings (MSVC is used automatically by Unreal)
			bUseUnityBuild = false;  // Example: disable Unity Build for Windows
			GlobalDefinitions.Add("USE_WINDOWS_SPECIFIC_CODE=1");
		}
	}
}
