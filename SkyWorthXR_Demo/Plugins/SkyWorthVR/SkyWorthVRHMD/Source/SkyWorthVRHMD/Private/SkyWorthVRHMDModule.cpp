//=============================================================================
// FILE: SkyWorthVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#include "SkyWorthVRHMDModule.h"
#include "SkyWorthVRHMD.h"
#include "SXR_Settings.h"

#if WITH_EDITOR
#include "PropertyEditorModule.h"
#include "ISettingsModule.h"
#include "ISettingsSection.h"
#endif

IMPLEMENT_MODULE(FSkyWorthVRHMDModule, SkyWorthVRHMD)

const FString FSkyWorthVRHMDModule::SkyWorthVRHMDModuleString(TEXT("SkyWorthVR")); // an FString is a regular string array
const FName   ISkyWorthVRHMDModule::SkyWorthVRHMDModuleName(TEXT("SkyWorthVR")); // an FName is case insensitive

#define LOCTEXT_NAMESPACE "FSXRModule"

//-----------------------------------------------------------------------------

void FSkyWorthVRHMDModule::RegisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->RegisterSettings("Project", "Plugins", "SkyWorthSetting",
			LOCTEXT("SkyWorthXRSettingsName", "SkyWorthXR Settings"),
			LOCTEXT("SkyWorthXRSettingsDescription", "Configure the SkyWorthXR plug-in."),
			GetMutableDefault<USXR_Settings>());
		USXR_Settings* const Settings = GetMutableDefault<USXR_Settings>();
	}
#endif
}

void FSkyWorthVRHMDModule::UnregisterSettings()
{
#if WITH_EDITOR
	if (ISettingsModule* SettingsModule = FModuleManager::GetModulePtr<ISettingsModule>("Settings"))
	{
		SettingsModule->UnregisterSettings("Project", "Plugins", "SkyWorthSetting");
	}
#endif
}

FSkyWorthVRHMDModule::FSkyWorthVRHMDModule()
{
}

FSkyWorthVRHMDModule::~FSkyWorthVRHMDModule()
{
}

TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > FSkyWorthVRHMDModule::CreateTrackingSystem()
{

#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	TSharedRef< FSkyWorthVRHMD, ESPMode::ThreadSafe > HMD = FSceneViewExtensions::NewExtension<FSkyWorthVRHMD>();
	if (HMD->IsInitialized())
	{
		UE_LOG(LogSVR, Log, TEXT("SkyWorthVR->IsInitialized() == True"));

		return HMD;
	}
	// 		SkyWorthVRHMD.Reset();//failed to initialize; free useless instance of SkyWorthVR
	UE_LOG(LogSVR, Error, TEXT("SkyWorthVR->IsInitialized() == Failed"));

#endif
	
	return nullptr;

}


//-----------------------------------------------------------------------------
TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe >  FSkyWorthVRHMDModule::GetVulkanExtensions()
{
//	if (!VulkanExtensions.IsValid())
//	{
//		VulkanExtensions = MakeShareable(new FSkyWorthVRHMDModule::FVulkanExtensions);
//	}
//
//	return VulkanExtensions;
	return nullptr;
}

bool FSkyWorthVRHMDModule::IsHMDConnected()
{
	return true;
}

bool FSkyWorthVRHMDModule::PreInit()
{
	return true;
}
void FSkyWorthVRHMDModule::StartupModule()
{
	IHeadMountedDisplayModule::StartupModule();
#if WITH_EDITOR
	FCoreDelegates::OnFEngineLoopInitComplete.AddRaw(this, &FSkyWorthVRHMDModule::RegisterSettings);
#endif //WITH_EDITOR
}

void FSkyWorthVRHMDModule::ShutdownModule()
{
	IHeadMountedDisplayModule::ShutdownModule();
	UnregisterSettings();
}

#undef LOCTEXT_NAMESPACE
