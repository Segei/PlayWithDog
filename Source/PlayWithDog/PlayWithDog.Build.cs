// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class PlayWithDog : ModuleRules
{
	public PlayWithDog(ReadOnlyTargetRules Target) : base(Target)
	{
		PrivateDependencyModuleNames.AddRange(new string[] { "AIModule" });
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[]
			{ "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "NavigationSystem"});
		PrivateIncludePaths.AddRange(new string[] { Name });
	}
}