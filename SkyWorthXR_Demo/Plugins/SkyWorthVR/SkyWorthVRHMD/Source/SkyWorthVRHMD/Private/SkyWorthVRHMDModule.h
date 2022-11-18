//=============================================================================
// FILE: SkyWorthVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================


#pragma once

#include "ISkyWorthVRHMDModule.h"
#include "IHeadMountedDisplay.h"

//-----------------------------------------------------------------------------
// FSkyWorthVRHMDModule Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class FSkyWorthVRHMDModule : public ISkyWorthVRHMDModule
{
public:
    /** IHeadMountedDisplayModule implementation */
//    virtual TSharedPtr< class IHeadMountedDisplay, ESPMode::ThreadSafe > CreateHeadMountedDisplay() override;	
	FSkyWorthVRHMDModule();
	virtual ~FSkyWorthVRHMDModule() override;

	void RegisterSettings();
	void UnregisterSettings();

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;
	virtual TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > GetVulkanExtensions() override;
	virtual bool IsHMDConnected() override;
	virtual bool PreInit() override;

	static const FString SkyWorthVRHMDModuleString;

	/** Returns the key into the HMDPluginPriority section of the config file for this module */
	FString GetModuleKeyName() const
	{
		return SkyWorthVRHMDModuleString;
	}

	void GetModuleAliases(TArray<FString>& AliasesOut) const override
	{
		AliasesOut.Add(TEXT("SkyWorthVR"));
	}

	static inline FSkyWorthVRHMDModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FSkyWorthVRHMDModule>("SkyWorthVR");
	}

	// TSharedPtr< IHeadMountedDisplay, ESPMode::ThreadSafe > SkyWorthVRHMD;//maintain singleton instance, since for Android platforms, UEngine::PreExit() is not called, which means the destructor for FSkyWorthVRHMD is never called.  Nonetheless, CreateHeadMountedDisplay() can still be called an arbitrary number of times -- June 13, 2016
};
