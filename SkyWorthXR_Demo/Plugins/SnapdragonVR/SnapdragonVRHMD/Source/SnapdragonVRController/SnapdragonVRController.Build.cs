// Copyright (c) 2016 QUALCOMM Technologies Inc. All Rights Reserved.

using System.IO;

namespace UnrealBuildTool.Rules
{
	public class SnapdragonVRController : ModuleRules
	{
        public SnapdragonVRController(ReadOnlyTargetRules  Target) : base(Target)
		{

			PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

			string SnapdragonSDKHeaderDirectory = ModuleDirectory;

			// Assume that the SVR stuff is located relative to ..\MyProjectRoot\Plugins\SnapdragonVR/XR
			// and that the SnapdragonVRLib code is co-located next to the this module's dir
			// ..\MyProjectRoot\Plugins\SnapdragonVR\SnapdragopnXRLib\Include
			SnapdragonSDKHeaderDirectory = System.IO.Path.Combine(SnapdragonSDKHeaderDirectory, @"..\..\..\SnapdragonXRLib");
			SnapdragonSDKHeaderDirectory = System.IO.Path.GetFullPath(SnapdragonSDKHeaderDirectory);

			PrivateIncludePaths.AddRange(
				new string[]{
				SnapdragonSDKHeaderDirectory
				// ... add other private include paths required here ...
				}
			);

			//System.Console.WriteLine(">>>>>>>>>>>>>>>>>>>>>>>>>>SnapdragonSDKHeaderDirectory->" + SnapdragonSDKHeaderDirectory);


			//System.Console.WriteLine("SnapdragonSDKHeaderDirectory->" + SnapdragonSDKHeaderDirectory);
			// Loop through List with foreach.
			//foreach(string daPath in PrivateIncludePaths)
			//{
			//	System.Console.WriteLine("SnapdragonController::PrivateIncludePaths -> " + daPath);
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

			// removed the dependency on the SVR lib code here because in the plugin description for the controllers (the uplugin file)
			// we state that the SnapdragonHMD plugin is required and IT has a dependency on the VR/XR lib
			// adding the controller plugin will automatically add the HMD plugin - and will complain if you try to remove it.
		}
	}
}
