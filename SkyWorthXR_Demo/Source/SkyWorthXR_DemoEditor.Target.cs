// Fill out your copyright notice in the Description page of Project Settings.

using UnrealBuildTool;
using System.Collections.Generic;

public class SkyWorthXR_DemoEditorTarget : TargetRules
{
	public SkyWorthXR_DemoEditorTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Editor;
		DefaultBuildSettings = BuildSettingsVersion.V2;

		if (Configuration == UnrealTargetConfiguration.Shipping)
		{
			bUseLoggingInShipping = true;
		}

		ExtraModuleNames.AddRange( new string[] { "SkyWorthXR_Demo" } );
	}
}
