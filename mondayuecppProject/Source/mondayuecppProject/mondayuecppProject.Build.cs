// Copyright Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;

public class mondayuecppProject : ModuleRules
{
	public mondayuecppProject(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] {
			"Core",
			"CoreUObject",
			"Engine",
			"InputCore",
			"EnhancedInput",
			"AIModule",
			"StateTreeModule",
			"GameplayStateTreeModule",
			"UMG",
			"Slate"
		});

		PrivateDependencyModuleNames.AddRange(new string[] { });

		PublicIncludePaths.AddRange(new string[] {
			"mondayuecppProject",
			"mondayuecppProject/Variant_Horror",
			"mondayuecppProject/Variant_Horror/UI",
			"mondayuecppProject/Variant_Shooter",
			"mondayuecppProject/Variant_Shooter/AI",
			"mondayuecppProject/Variant_Shooter/UI",
			"mondayuecppProject/Variant_Shooter/Weapons"
		});

		// Uncomment if you are using Slate UI
		// PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

		// Uncomment if you are using online features
		// PrivateDependencyModuleNames.Add("OnlineSubsystem");

		// To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
	}
}
