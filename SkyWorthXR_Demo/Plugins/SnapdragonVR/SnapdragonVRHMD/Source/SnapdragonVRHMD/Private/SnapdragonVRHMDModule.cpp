//=============================================================================
// FILE: SnapdragonVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#include "SnapdragonVRHMDModule.h"
#include "SnapdragonVRHMD.h"

const FString FSnapdragonVRHMDModule::SnapdragonVRHMDModuleString(TEXT("SnapdragonVR")); // an FString is a regular string array
const FName   ISnapdragonVRHMDModule::SnapdragonVRHMDModuleName(TEXT("SnapdragonVR")); // an FName is case insensitive

//-----------------------------------------------------------------------------

FSnapdragonVRHMDModule::FSnapdragonVRHMDModule()
{
}

FSnapdragonVRHMDModule::~FSnapdragonVRHMDModule()
{
}
TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > FSnapdragonVRHMDModule::CreateTrackingSystem()
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	TSharedRef< FSnapdragonVRHMD, ESPMode::ThreadSafe > HMD = FSceneViewExtensions::NewExtension<FSnapdragonVRHMD>();
	if (HMD->IsInitialized())
	{
		UE_LOG(LogSVR, Log, TEXT("SnapdragonVR->IsInitialized() == True"));

		return HMD;
	}

	UE_LOG(LogSVR, Log, TEXT("SnapdragonVR->IsInitialized() == Failed"));
	// 		SnapdragonVRHMD.Reset();//failed to initialize; free useless instance of SnapdragonVR
#endif

	return nullptr;
}


//-----------------------------------------------------------------------------
TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe >  FSnapdragonVRHMDModule::GetVulkanExtensions()
{
//	if (!VulkanExtensions.IsValid())
//	{
//		VulkanExtensions = MakeShareable(new FSnapdragonVRHMDModule::FVulkanExtensions);
//	}
//
//	return VulkanExtensions;
	return nullptr;
}

bool FSnapdragonVRHMDModule::IsHMDConnected()
{
	return true;
}

bool FSnapdragonVRHMDModule::PreInit()
{
	return true;
}
void FSnapdragonVRHMDModule::StartupModule()
{
	IHeadMountedDisplayModule::StartupModule();
#if WITH_EDITOR
	if (GIsEditor)
	{
		// register settings
		// ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings");
		// if (SettingsModule != nullptr)
		// {
		// 	SettingsModule->RegisterSettings("Project", "Plugins", "GSXRSetting",
		// 	LOCTEXT("GSXRSettingsName", "GSXR Settings"),
		// 				LOCTEXT("GSXRSettingsDescription", "Configure the GSXR SDK."),
		// 		GetMutableDefault<UGSXRSettings>()
		// 	);
		// }
	}
#endif //WITH_EDITOR
}

void FSnapdragonVRHMDModule::ShutdownModule()
{
	IHeadMountedDisplayModule::ShutdownModule();
}
IMPLEMENT_MODULE(FSnapdragonVRHMDModule, SnapdragonVRHMD)
