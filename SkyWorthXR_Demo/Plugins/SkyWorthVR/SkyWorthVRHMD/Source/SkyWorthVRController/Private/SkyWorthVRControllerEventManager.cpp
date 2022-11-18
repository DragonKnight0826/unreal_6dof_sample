//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "SkyWorthVRControllerEventManager.h"

static USkyWorthVRControllerEventManager* Singleton = nullptr;

//-----------------------------------------------------------------------------
USkyWorthVRControllerEventManager::USkyWorthVRControllerEventManager(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//-----------------------------------------------------------------------------
USkyWorthVRControllerEventManager* USkyWorthVRControllerEventManager::GetInstance()
{
	if (!Singleton)
	{
		Singleton = NewObject<USkyWorthVRControllerEventManager>();
		Singleton->AddToRoot();
	}
	return Singleton;
}