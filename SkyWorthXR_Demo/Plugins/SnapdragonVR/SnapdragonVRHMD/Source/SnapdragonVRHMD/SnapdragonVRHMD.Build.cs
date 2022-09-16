//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

using System;
using System.IO;

namespace UnrealBuildTool.Rules
{
    public class SnapdragonVRHMD : ModuleRules
    {
        public SnapdragonVRHMD(ReadOnlyTargetRules Target) : base(Target)
        {
            PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;


            // Assume that the SVR stuff is located relative to ..\MyProjectRoot\Plugins\SnapdragonVR/XR
            // and that the SnapdragonVRLib code is co-located next to the this module's dir
            // ..\MyProjectRoot\Plugins\SnapdragonVR\SnapdragonXRLib\Include
            // This path is a bit redundant but is the same path that's used in the Controller plug in
            //string SnapdragonSDKHeaderDirectory = System.IO.Path.Combine(ModuleDirectory, @"..\..\..\SnapdragonXRLib");
            //SnapdragonSDKHeaderDirectory = System.IO.Path.GetFullPath(SnapdragonSDKHeaderDirectory);

            // locate the lib dir relative to where the lib header is located
            //string SnapdragonSXRApiLibDirectory = System.IO.Path.Combine(SnapdragonSDKHeaderDirectory, @"..\SnapdragonXRLib\jni");
            //SnapdragonSXRApiLibDirectory = System.IO.Path.GetFullPath(SnapdragonSXRApiLibDirectory);

            // where is the APL file located (next to this CS file...)
            //string APLPath = System.IO.Path.Combine(SnapdragonSDKHeaderDirectory, @"..\..\SnapdragonVRHMD\Source\SnapdragonVRHMD");
            //APLPath = System.IO.Path.GetFullPath(APLPath);

            // Required for some private headers needed for the rendering support.
            string EngineRuntimePath = Path.GetFullPath(Target.RelativeEnginePath + @"\Source\Runtime");

            //System.Console.WriteLine("  >>>>>>>>>>>>  SnapdragonSDKHeaderDirectory -> " + SnapdragonSDKHeaderDirectory);
            //System.Console.WriteLine("  >>>>>>>>>>>>  SnapdragonSXRApiLibDirectory -> " + SnapdragonSXRApiLibDirectory);
            //System.Console.WriteLine("  >>>>>>>>>>>>  APLPath -> " + APLPath);


    //        PrivateIncludePaths.AddRange(
    //            new string[]{
    //            SnapdragonSDKHeaderDirectory
				//// ... add other private include paths required here ...
				//}
    //        );

            
            //System.Console.WriteLine(">>>>>>>>>>>>>>>>>>>>>>>>>>SnapdragonSDKHeaderDirectory->" + SnapdragonSDKHeaderDirectory);
            // Loop through List with foreach.

            PrivateIncludePaths.AddRange(
                new string[]{
                    "SnapdragonVRHMD/Private",
                    EngineRuntimePath + @"\Renderer\Private",
                    EngineRuntimePath + @"\Core\Private",
                    EngineRuntimePath + @"\OpenGLDrv\Private",
                    EngineRuntimePath + @"\VulkanRHI\Private",
                    //SnapdragonSDKHeaderDirectory
				// ... add other private include paths required here ...
				}
            );

            string pathtoOpenGL = System.IO.Path.GetFullPath(Target.UEThirdPartySourceDirectory + "OpenGL");
            PrivateIncludePaths.Add(pathtoOpenGL);

            // 4.20/21/22 move Vulkan headers around a bit
            // best to get latest one. The oculus cs file figures out the version from the header source
            // for now assume one of two places for the header. Note that all versions are the same (for same API level)

            //string pathtoVulkan = "";
            //// pre 420 header location
            //string pathtoVulkan419 = System.IO.Path.GetFullPath(Target.UEThirdPartySourceDirectory + @"Vulkan\Windows\Include\vulkan");
            //// This is for 4.20
            //string pathtoVulkan420 = System.IO.Path.GetFullPath(Target.UEThirdPartySourceDirectory + @"Vulkan\include\vulkan");


            //if (File.Exists(pathtoVulkan419 + "/vulkan.h"))
            //	{
            //		pathtoVulkan = pathtoVulkan419;
            //	}
            //else if (File.Exists(pathtoVulkan420 + "/vulkan.h"))
            //	{
            //		pathtoVulkan = pathtoVulkan420;
            //	}
            //else
            //	{
            //	System.Console.WriteLine("  >>>>>>>>>>>>  SnapdragonHMD::PrivateIncludePaths -> cant find Vulkan.h");
            //	}


            // future TODO - might look for NDK or vulkan SDK
            // NOTE:  for some reason using relative path fails. but full path works
            //PrivateIncludePaths.Add(System.IO.Path.GetFullPath(pathtoVulkan));

            // 			string VulkanSDKPath = Environment.GetEnvironmentVariable("VULKAN_SDK");
            // 
            // 			bool bHaveVulkan = false;
            // 			if (Target.Platform == UnrealTargetPlatform.Android)
            // 			{
            // 				// Note: header is the same for all architectures so just use arch-arm
            // 				string NDKPath = Environment.GetEnvironmentVariable("NDKROOT");
            // 				string NDKVulkanIncludePath = NDKPath + "/platforms/android-24/arch-arm/usr/include/vulkan";
            // 
            // 				// Use NDK Vulkan header if discovered, or VulkanSDK if available
            // 				if (File.Exists(NDKVulkanIncludePath + "/vulkan.h"))
            // 				{
            // 					bHaveVulkan = true;
            // 					PrivateIncludePaths.Add(NDKVulkanIncludePath);
            // 				}
            // 				else
            // 				if (!String.IsNullOrEmpty(VulkanSDKPath))
            // 				{
            // 					// If the user has an installed SDK, use that instead
            // 					bHaveVulkan = true;
            // 					PrivateIncludePaths.Add(VulkanSDKPath + "/Include/vulkan");
            // 				}
            // 				else
            // 				{
            // 					// Fall back to the Windows Vulkan SDK (the headers are the same)
            // 					bHaveVulkan = true;
            // 					PrivateIncludePaths.Add(Target.UEThirdPartySourceDirectory + "Vulkan/Windows/Include/vulkan");
            // 				}
            // 			}
            // 			

            foreach (string daPath in PrivateIncludePaths)
            {
                System.Console.WriteLine("  >>>>>>>>>>>>  SnapdragonHMD::PrivateIncludePaths -> " + daPath);
            }
            PublicIncludePathModuleNames.AddRange(
            new[] {
                "Launch",
                "ProceduralMeshComponent",
            });

            PrivateDependencyModuleNames.AddRange(
                new string[]
                {
                    "Core",
                    "CoreUObject",
                    "Engine",
                    "RHI",
					"RenderCore",
                    "Renderer",
                    "HeadMountedDisplay",
                    "Slate",
                    "SlateCore",
                    "OpenGLDrv",
                    "OpenGL",
                    "VulkanRHI",
					"SkyworthXRLib"
                }
                );

            AddEngineThirdPartyPrivateStaticDependencies(Target, "OpenGL");
            PrivateDependencyModuleNames.Add("OpenGLDrv");

            //AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
            //PrivateDependencyModuleNames.Add("VulkanRHI");
            if (Target.Type == TargetRules.TargetType.Editor)
            {
                PrivateDependencyModuleNames.AddRange(
                    new string[]
                    {
                    "UnrealEd",
                    "Slate",
                    "SlateCore",
                    "EditorStyle",
                    "EditorWidgets",
                    "DesktopWidgets",
                    "PropertyEditor",
                    "SharedSettingsWidgets",
                    "SequencerWidgets"
                    }
                );
            }

            // deleted "svrApi" module and removed references to it - we just link in the Android library directly
            if (Target.Platform == UnrealTargetPlatform.Win64)
            {
                PrivateIncludePaths.Add(EngineRuntimePath + @"\VulkanRHI\Private\Windows");
                AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
            }
            else
            {
                PrivateIncludePaths.Add(EngineRuntimePath + @"\VulkanRHI\Private\" + Target.Platform);
                AddEngineThirdPartyPrivateStaticDependencies(Target, "Vulkan");
            }

            if (Target.Platform == UnrealTargetPlatform.Android)
            {
                string PluginPath = Utils.MakePathRelativeTo(ModuleDirectory, Target.RelativeEnginePath);
                AdditionalPropertiesForReceipt.Add("AndroidPlugin", Path.Combine(PluginPath, "SnapdragonVRHMD_APL.xml"));

                

                //PublicAdditionalLibraries.Add(SnapdragonSXRApiLibDirectory + "/armeabi-v7a/libgsxr_fe.so");
                //PublicAdditionalLibraries.Add(SnapdragonSXRApiLibDirectory + "/arm64-v8a/libgsxr_fe.so");

                // toolchain will filter properly
                //PublicLibraryPaths.Add(SnapdragonSXRApiLibDirectory + "/armeabi-v7a");
                //PublicLibraryPaths.Add(SnapdragonSXRApiLibDirectory + "/arm64-v8a");
                //PublicAdditionalLibraries.Add("libsxrapi.so");

            }

        }
    }
}
