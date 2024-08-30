// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class MTVS3_3rd : ModuleRules
{
	public MTVS3_3rd(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] { "Core", "CoreUObject", "CoreOnline", "Engine", "InputCore", "EnhancedInput", "OnlineSubsystem", "OnlineSubsystemUtils", "OnlineSubsystemSteam" });
	}
}
