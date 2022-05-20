// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class NetworkTimeSync : ModuleRules
{
	public NetworkTimeSync(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.Add("Core");

		PrivateDependencyModuleNames.AddRange(
			new string[]
			{
				"CoreUObject",
				"Engine",
				"Slate",
				"SlateCore",
				"DeveloperSettings"
				// ... add private dependencies that you statically link with here ...	
			}
		);
	}
}
