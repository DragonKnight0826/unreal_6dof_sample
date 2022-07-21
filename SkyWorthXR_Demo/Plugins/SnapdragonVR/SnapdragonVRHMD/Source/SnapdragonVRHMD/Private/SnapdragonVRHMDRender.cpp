//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "Runtime/Core/Public/Misc/ScopeLock.h"
#include "SnapdragonVRHMD.h"
#include "RHIStaticStates.h"

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

#if PLATFORM_ANDROID
#include "Android/AndroidJNI.h"
#endif // PLATFORM_ANDROID


#include <android_native_app_glue.h>


#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

#define TextureQueueLength 3

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

extern struct android_app* GNativeAndroidApp;   // rbf validate we need this

//-----------------------------------------------------------------------------
// FSnapdragonVRHMDCustomPresent, IStereoRendering Implementation
//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList, class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture, FVector2D WindowSize) const 
{
	// was empty originally
	if (SpectatorScreenController)
	{
		SpectatorScreenController->RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture, WindowSize);
	}
	// UE_LOG(LogSVR,Log,TEXT("RenderTexture_RenderThread Textureid:%d "),(*((uint32_t *)SrcTexture->GetNativeResource())));
}


//-----------------------------------------------------------------------------
// FSnapdragonVRHMDCustomPresent Implementation
//-----------------------------------------------------------------------------
// FSnapdragonVRHMDCustomPresent::FSnapdragonVRHMDCustomPresent(FSnapdragonVRHMD* pHMD)
//     : FRHICustomPresent(0),
//     bInVRMode(false),
//     bContextInitialized(false),
// 	PoseHistory(kMaxPoseHistoryLength)
// {
// }

// bool FSnapdragonVRHMD::AllocateRenderTargetTexture(
// 	uint32 Index, 
// 	uint32 SizeX, 
// 	uint32 SizeY, 
// 	uint8 Format, 
// 	uint32 NumMips, 
// 	ETextureCreateFlags InTexFlags, 
// 	ETextureCreateFlags InTargetableTextureFlags, 
// 	FTexture2DRHIRef& OutTargetableTexture, 
// 	FTexture2DRHIRef& OutShaderResourceTexture, 
// 	uint32 NumSamples)
// {
//     return pSnapdragonVRBridge->AllocateRenderTargetTexture(
//         SizeX, 
//         SizeY, 
//         Format, 
//         NumMips, 
// 		InTexFlags,
// 		InTargetableTextureFlags,
//         OutTargetableTexture, 
//         OutShaderResourceTexture,
// 		NumSamples);

// }


// void FSnapdragonVRHMDCustomPresent::FinishRendering()
// {
//     check(IsInRenderingThread());
// 
//     if(bInVRMode)
//     {
//         //const bool emptyQueue = mPoseQueueSvrBasis.IsEmpty();
// 		const bool PoseHistoryEmpty = PoseHistory.IsEmpty();
//         if (PoseHistoryEmpty)
//         {
//             UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMDCustomPresent::FinishRendering() found no pose!"));
//         }
//         else
//         {
//             FPoseStateFrame poseStateDebug;
//             PoseHistory.Dequeue(poseStateDebug);
// 
//             GSXRFrameParams frameParams;
//             memset(&frameParams, 0, sizeof(frameParams));
//             frameParams.frameIndex = poseStateDebug.FrameNumber;
//             frameParams.renderLayers[0].imageHandle = mTextureSet->GetRenderTargetResource();
// 			frameParams.renderLayers[0].imageType = kGSXRTypeTexture;
// 			frameParams.renderLayers[0].eyeMask = kEyeMaskBoth;
//             //frameParams.eyeBufferType = kEyeBufferStereoSingle; /// RBF validate this with Chris
//             frameParams.headPoseState = poseStateDebug.Pose;
//             frameParams.minVsyncs = 1;
// 
//             //UE_LOG(LogSVR, Error, TEXT("    FSnapdragonVRHMDCustomPresent::FinishRendering() => SC::GSXR_nativeSubmitDataFrame()"));
//             SC::GSXR_nativeSubmitDataFrame(&frameParams);
//             mTextureSet->SwitchToNextElement();
//         }
//     }
// }
// 
// void FSnapdragonVRHMDCustomPresent::UpdateViewport(const FViewport& Viewport, FRHIViewport* InViewportRHI)
// {
//     check(IsInGameThread());
//     check(InViewportRHI);
// 
//     InViewportRHI->SetCustomPresent(this);
// }


#endif //SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS