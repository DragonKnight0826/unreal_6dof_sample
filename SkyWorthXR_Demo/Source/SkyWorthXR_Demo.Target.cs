// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SkyWorthXR_DemoTarget : TargetRules
{
	public SkyWorthXR_DemoTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		// enable logs and debugging for Shipping builds
		if (Configuration == UnrealTargetConfiguration.Shipping)
		{
            BuildEnvironment = TargetBuildEnvironment.Unique;
            bUseLoggingInShipping = true;
        }

		ExtraModuleNames.AddRange( new string[] { "SkyWorthXR_Demo" } );
	}
}
