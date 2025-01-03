// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class FPSDemo : ModuleRules
{
	public FPSDemo(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule","UMG" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput" });
	}
}
