// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;

public class VR_Template : ModuleRules
{
    public VR_Template(ReadOnlyTargetRules Target) : base(Target)
    {
        bAllowConfidentialPlatformDefines = true;

        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[] { 
            "Core",
            "CoreUObject",
            "Engine",
            "InputCore",
            "EnhancedInput", 
            "NavigationSystem",
            "CableComponent"
        });

        PrivateDependencyModuleNames.AddRange(new string[] { "HeadMountedDisplay" });

        //if (Target.Platform == UnrealTargetPlatform.PS5) 
        //{
        //    PrivateDependencyModuleNames.Add("FlexibleScaledRasterization");

        //}

        // Uncomment if you are using Slate UI
        // PrivateDependencyModuleNames.AddRange(new string[] { "Slate", "SlateCore" });

        // Uncomment if you are using online features
        // PrivateDependencyModuleNames.Add("OnlineSubsystem");

        // To include OnlineSubsystemSteam, add it to the plugins section in your uproject file with the Enabled attribute set to true
    }
}
