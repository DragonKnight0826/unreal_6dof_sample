//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "Core/Public/Modules/ModuleManager.h"
#include "IInputDeviceModule.h"

#define SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS (PLATFORM_ANDROID && (PLATFORM_ANDROID_ARM || PLATFORM_ANDROID_ARM64))

/**
* The public interface to this module.  In most cases, this interface is only public to sibling modules
* within this plugin.
*/
class ISnapdragonVRControllerPlugin : public IInputDeviceModule
{

public:

	/**
	* Singleton-like access to this module's interface.  This is just for convenience!
	* Beware of calling this during the shutdown phase, though.  Your module might have been unloaded already.
	*
	* @return Returns singleton instance, loading the module on demand if needed
	*/
	static inline ISnapdragonVRControllerPlugin& Get()
	{
		return FModuleManager::LoadModuleChecked< ISnapdragonVRControllerPlugin >("SnapdragonVRController");
	}

	/**
	* Checks to see if this module is loaded and ready.  It is only valid to call Get() if IsAvailable() returns true.
	*
	* @return True if the module is loaded and ready to use
	*/
	static inline bool IsAvailable()
	{
		return FModuleManager::Get().IsModuleLoaded("SnapdragonVRController");
	}
};
