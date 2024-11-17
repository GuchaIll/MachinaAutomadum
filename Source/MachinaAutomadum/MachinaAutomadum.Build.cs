// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MachinaAutomadum : ModuleRules
{
	public MachinaAutomadum(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore", "EnhancedInput", "MediaAssets" });
	
	    PrivateDependencyModuleNames.AddRange( collection: new string[] { "AIModule"});

		PublicDependencyModuleNames.AddRange(new string[] { "GameplayAbilities", "GameplayTags", "GameplayTasks" });
	}
}
