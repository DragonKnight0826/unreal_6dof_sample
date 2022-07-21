//=============================================================================
// FILE: SnapdragonVRHMD_TextureSetProxy.cpp
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================
#include "SnapdragonVRHMD_TextureSetProxy.h"
#include "SnapdragonXR_CVars.h"

FSnapdragonVRTextureSet::FSnapdragonVRTextureSet()
{
	check(CVars::GNumSwapchainImages > 0);
	SwapChainImages.AddZeroed(CVars::GNumSwapchainImages);
}

FSnapdragonVRTextureSet::~FSnapdragonVRTextureSet()
{

}

void FSnapdragonVRTextureSet::SwitchToNextElement()
{
	SwapChainImageIndex = ((SwapChainImageIndex + 1) % CVars::GNumSwapchainImages);
	SetNativeResource(SwapChainImages[SwapChainImageIndex]);
}