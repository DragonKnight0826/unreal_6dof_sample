//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "../Classes/SkyWorthVRHMDEventManager.h"

static USkyWorthVRHMDEventManager* Singleton = nullptr;

USkyWorthVRHMDEventManager::USkyWorthVRHMDEventManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

USkyWorthVRHMDEventManager* USkyWorthVRHMDEventManager::GetInstance()
{
	if (!Singleton)
	{
		Singleton = NewObject<USkyWorthVRHMDEventManager>();
		Singleton->AddToRoot();
	}
	return Singleton;
}