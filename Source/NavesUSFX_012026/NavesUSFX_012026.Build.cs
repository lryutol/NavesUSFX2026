// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NavesUSFX_012026 : ModuleRules
{
	public NavesUSFX_012026(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore" });
	}
}
