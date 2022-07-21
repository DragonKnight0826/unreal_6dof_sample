// Copyright 1998-2019 Epic Games, Inc. All Rights Reserved.

using UnrealBuildTool;
using System;
using System.IO;

public class SkyworthXRLib : ModuleRules
{
    public SkyworthXRLib(ReadOnlyTargetRules Target) : base(Target)
    {
        Type = ModuleType.External;

        string SourceDirectory = ModuleDirectory + "/SkyworthXRLib/";
         PublicIncludePaths.Add(SourceDirectory + "Include");

        if (Target.Platform == UnrealTargetPlatform.Android)
        {
            PublicAdditionalLibraries.Add(SourceDirectory + "Lib/armeabi-v7a/" + "libgsxr_fe.so");
            PublicAdditionalLibraries.Add(SourceDirectory + "Lib/arm64-v8a/" + "libgsxr_fe.so");

            AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(ModuleDirectory, "SkyworthXRLib_APL.xml"));
        }

    }
}