// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class Cordelias_SightEditorTarget : TargetRules
{
	public Cordelias_SightEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.Latest;
		IncludeOrderVersion = EngineIncludeOrderVersion.Latest;
		bWarningsAsErrors = true;
        bOverrideBuildEnvironment = true;

        ExtraModuleNames.AddRange( new string[] { "Cordelias_Sight" } );
	}
}
