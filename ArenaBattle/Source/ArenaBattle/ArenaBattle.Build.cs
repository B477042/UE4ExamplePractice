// Copyright 1998-2018 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class ArenaBattle : ModuleRules
{
	public ArenaBattle(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "Engine", "InputCore","UMG",
            "HeadMountedDisplay" ,"AIModule","NavigationSystem","GameplayTasks"});
        PrivateDependencyModuleNames.AddRange(new string[] { "ArenaBattleSetting" });
	}
}
