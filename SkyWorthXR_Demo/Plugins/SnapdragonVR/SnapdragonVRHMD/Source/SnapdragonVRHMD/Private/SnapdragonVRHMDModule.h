//=============================================================================
// FILE: SnapdragonVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================


#pragma once

#include "ISnapdragonVRHMDModule.h"
#include "IHeadMountedDisplay.h"

//-----------------------------------------------------------------------------
// FSnapdragonVRHMDModule Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
class FSnapdragonVRHMDModule : public ISnapdragonVRHMDModule
{
public:
    /** IHeadMountedDisplayModule implementation */
//    virtual TSharedPtr< class IHeadMountedDisplay, ESPMode::ThreadSafe > CreateHeadMountedDisplay() override;	
	FSnapdragonVRHMDModule();
	virtual ~FSnapdragonVRHMDModule() override;

	virtual void StartupModule() override;
	virtual void ShutdownModule() override;
	virtual TSharedPtr< class IXRTrackingSystem, ESPMode::ThreadSafe > CreateTrackingSystem() override;
	virtual TSharedPtr< IHeadMountedDisplayVulkanExtensions, ESPMode::ThreadSafe > GetVulkanExtensions() override;
	virtual bool IsHMDConnected() override;
	virtual bool PreInit() override;

	static const FString SnapdragonVRHMDModuleString;

	/** Returns the key into the HMDPluginPriority section of the config file for this module */
	FString GetModuleKeyName() const
	{
		return SnapdragonVRHMDModuleString;
	}

	void GetModuleAliases(TArray<FString>& AliasesOut) const override
	{
		AliasesOut.Add(TEXT("SnapdragonVR"));
	}

	static inline FSnapdragonVRHMDModule& Get()
	{
		return FModuleManager::LoadModuleChecked<FSnapdragonVRHMDModule>("SnapdragonVR");
	}

	// TSharedPtr< IHeadMountedDisplay, ESPMode::ThreadSafe > SnapdragonVRHMD;//maintain singleton instance, since for Android platforms, UEngine::PreExit() is not called, which means the destructor for FSnapdragonVRHMD is never called.  Nonetheless, CreateHeadMountedDisplay() can still be called an arbitrary number of times -- June 13, 2016
};
