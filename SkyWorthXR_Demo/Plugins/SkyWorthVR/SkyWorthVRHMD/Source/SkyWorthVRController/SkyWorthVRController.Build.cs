// Copyright (c) 2016 QUALCOMM Technologies Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SkyWorthVRController : ModuleRules
	{
        public SkyWorthVRController(ReadOnlyTargetRules  Target) : base(Target)
		{

			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			string SkyWorthSDKHeaderDirectory = ModuleDirectory;

			// Assume that the SVR stuff is located relative to ..\MyProjectRoot\Plugins\SkyWorthVR/XR
			// and that the SkyWorthVRLib code is co-located next to the this module's dir
			// ..\MyProjectRoot\Plugins\SkyWorthVR\SnapdragopnXRLib\Include

			//System.Console.WriteLine(">>>>>>>>>>>>>>>>>>>>>>>>>>SkyWorthSDKHeaderDirectory->" + SkyWorthSDKHeaderDirectory);


			//System.Console.WriteLine("SkyWorthSDKHeaderDirectory->" + SkyWorthSDKHeaderDirectory);
			// Loop through List with foreach.
			//foreach(string daPath in PrivateIncludePaths)
			//{
			//	System.Console.WriteLine("SkyWorthController::PrivateIncludePaths -> " + daPath);
			//}


            PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"Core",
					"CoreUObject",
					"Engine",
					"InputCore",
					"InputDevice",
					"HeadMountedDisplay",
					"UMG",
					"Slate",
					"SlateCore",
					"SkyworthXRLib"
				}
				);

            PrivateIncludePaths.AddRange(
				new string[] {
					// ... add other private include paths required here ...
					}
				);

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "SkyWorthVRController_APL.xml"));
            }
            // removed the dependency on the SVR lib code here because in the plugin description for the controllers (the uplugin file)
            // we state that the SkyWorthHMD plugin is required and IT has a dependency on the VR/XR lib
            // adding the controller plugin will automatically add the HMD plugin - and will complain if you try to remove it.
        }
	}
}
