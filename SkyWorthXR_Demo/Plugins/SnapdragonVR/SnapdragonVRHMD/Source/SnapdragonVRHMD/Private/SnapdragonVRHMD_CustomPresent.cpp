//=============================================================================
// FILE: SnapdragonVRHMD_CustomPresent.cpp
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================
#include "SnapdragonVRHMD_CustomPresent.h"
#include "Runtime/Core/Public/Misc/ScopeLock.h"
#include "SnapdragonVRHMD.h"

#define dbgprint(format, ...) //FPlatformMisc::LowLevelOutputDebugStringf(*FString::Printf(TEXT("%s line %d: %s"), TEXT(__FILE__), __LINE__, *FString::Printf(TEXT(format), ##__VA_ARGS__)))

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

#include <android_native_app_glue.h>

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"

extern struct android_app* GNativeAndroidApp;

void FSnapdragonVRHMDCustomPresent::OnBackBufferResize()
{
}

bool FSnapdragonVRHMDCustomPresent::NeedsNativePresent()
{
	return false;
}

bool FSnapdragonVRHMDCustomPresent::Present(int32& InOutSyncInterval)
{
	// CTORNE: Check was removed from Release4.17, should it be here?
	check(IsInRenderingThread());

	FinishRendering();

	return false;
}

FSnapdragonVRHMDCustomPresent::FSnapdragonVRHMDCustomPresent(FSnapdragonVRHMD* pHMD)
	: bInVRMode(false)
	, bContextInitialized(false)
	, bUsesArrayTexture(false)
{
}

FSnapdragonVRHMDCustomPresent::~FSnapdragonVRHMDCustomPresent()
{

}

void FSnapdragonVRHMDCustomPresent::BeginRendering(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	bContextInitialized = true;

	if (bInVRMode)
	{
		// TODO: On Spacedock (Shooting Gallery) there seems to be a problem with
		// Unreal trying to do something with the EGL surface. It doesn't seem to
		// be a race condition because even if we delay here for 100 frames there
		// is still a problem.

		// Need the current head pose
		#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
		float PredictedTime = SC::GSXR_nativeGetPredictedDisplayTime();
		mRenderPose = SC::GSXR_nativeGetPredictedPose(PredictedTime);
		#endif
		//sxrprint("(%s) (Frame %d) BeginRendering(mRenderPose) => Head pose (%0.2f ms) = [%0.4f, %0.4f, %0.4f, %0.4f]",
		//	IsInRenderingThread() ? TEXT("Render") : TEXT("Game"),
		//	GFrameNumber,
		//	PredictedTime,
		//	mRenderPose.pose.rotation.x, mRenderPose.pose.rotation.y, mRenderPose.pose.rotation.z, mRenderPose.pose.rotation.w);
		
		// this might not be the best place for this, but it will unbind the shader per eye
		// blindly doing it for both eyes will not work, so use the default left eye
		SC::GSXR_nativeBeginEye(kGSXRLeftEye, kGSXRTypeTexture);
		//SC::GSXR_nativeBeginEye(kGSXRRightEye);
	}
}

FSnapdragonVRHMDCustomPresent* FSnapdragonVRHMDCustomPresent::Create(FSnapdragonVRHMD* pHMD)
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_VULKAN
	if (FCString::Stricmp(GDynamicRHI->GetName(), TEXT("Vulkan")) == 0)
	{
		return CreateCustomPresent_Vulkan(pHMD);
	}
#endif
	return CreateCustomPresent_OpenGLES(pHMD);
}


void FSnapdragonVRHMDCustomPresent::FinishRendering()
{
	// CTORNE: Check was removed from Release4.17, should it be here?
	check(IsInRenderingThread());

	if (bInVRMode)
	{
		FPoseStateFrame PoseState;
		PoseState.Pose = mRenderPose;
		PoseState.FrameNumber = GFrameNumber;
		//UpdateMyPoses(PoseState);

		 UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMDCustomPresent::FinishRendering()  fetching frame:%u/%u  sxrFrame#:%llu  Position = x:%5.2f, y:%5.2f, z:%5.2f    Rotation=[%5.3f,%5.3f,%5.3f,%5.3f]"),
		 	GFrameNumber, GFrameNumberRenderThread, PoseState.FrameNumber,
		 	PoseState.Pose.pose.position.x, PoseState.Pose.pose.position.y, PoseState.Pose.pose.position.z, 
		 	PoseState.Pose.pose.rotation.x, PoseState.Pose.pose.rotation.y, PoseState.Pose.pose.rotation.z, PoseState.Pose.pose.rotation.w
		 );

		 UE_LOG(LogSVR, Error, TEXT("sxr (%s) (Frame %llu) FinishRendering() => Calling SubmitFrame"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber);
		 //sxrprint("(%s) (Frame %d) FinishRendering() => Calling SubmitFrame", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber);

		// will render the bounding area - so we do it for each eye (mismatched with SC::GSXR_nativeBeginEye calls)
		// must be done just before SC::GSXR_nativeSubmitDataFrame
		SC::GSXR_nativeEndEye(GSXRWhichEye::kGSXRLeftEye, kGSXRTypeTexture);
		SC::GSXR_nativeEndEye(GSXRWhichEye::kGSXRRightEye, kGSXRTypeTexture);

		SubmitFrame(PoseState);
	}
}

void FSnapdragonVRHMDCustomPresent::UpdateViewport(const FViewport& Viewport, FRHIViewport* InViewportRHI)
{
	check(IsInGameThread());
	check(InViewportRHI);

	InViewportRHI->SetCustomPresent(this);
}

void FSnapdragonVRHMDCustomPresent::DoBeginVR()
{
	check(IsInRenderingThread());
	check(!IsInGameThread());

	if (!bInVRMode)
	{
		#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

		bInVRMode = true;

		//sxr performance levels can be manipulated by render or game thread, so prevent race conditions
		FScopeLock ScopeLock(&PerfLevelCriticalSection);
		GSXRBeginParams beginParams;
		memset(&beginParams, 0, sizeof(GSXRBeginParams));

		if (FSnapdragonVRHMD::PerfLevelCpuLastSet != kGSXRNumPerfLevels && FSnapdragonVRHMD::PerfLevelGpuLastSet != kGSXRNumPerfLevels)
		{
			beginParams.cpuPerfLevel = FSnapdragonVRHMD::PerfLevelCpuLastSet;
			beginParams.gpuPerfLevel = FSnapdragonVRHMD::PerfLevelGpuLastSet;
		}
		else
		{
			const GSXRPerfLevel PerfLevelDefault = FSnapdragonVRHMD::GetCVarSnapdragonVrPerfLevelDefault();
			FSnapdragonVRHMD::PerfLevelsLastSetByCvarRead(&beginParams.cpuPerfLevel, &beginParams.gpuPerfLevel, PerfLevelDefault, PerfLevelDefault);
		}

		beginParams.nativeWindow = GNativeAndroidApp->window;
		beginParams.mainThreadId = gettid();

		FSnapdragonVRHMD::PerfLevelCpuWrite(beginParams.cpuPerfLevel);
		FSnapdragonVRHMD::PerfLevelGpuWrite(beginParams.gpuPerfLevel);
		FSnapdragonVRHMD::PerfLevelLog(TEXT("SC::GSXR_nativeStartSlam"), FSnapdragonVRHMD::PerfLevelCpuLastSet, FSnapdragonVRHMD::PerfLevelGpuLastSet);

		/*FAndroidMisc::UnlockAndroidWindow();*/
		beginParams.colorSpace = kGSXRColorSpaceLinear;

		if (SC::GSXR_nativeStartSlam(&beginParams) != GSXR_ERROR_NONE)
		{
			bInVRMode = false;
		}
		#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

	}
}

void FSnapdragonVRHMDCustomPresent::DoEndVR()
{

}

void FSnapdragonVRHMDCustomPresent::CreateLayout(float centerX, float centerY, float radiusX, float radiusY, GSXRLayoutCoords *pLayout)
{
	// This is always in screen space so we want Z = 0 and W = 1
	float lowerLeftPos[4] = { centerX - radiusX, centerY - radiusY, 0.0f, 1.0f };
	float lowerRightPos[4] = { centerX + radiusX, centerY - radiusY, 0.0f, 1.0f };
	float upperLeftPos[4] = { centerX - radiusX, centerY + radiusY, 0.0f, 1.0f };
	float upperRightPos[4] = { centerX + radiusX, centerY + radiusY, 0.0f, 1.0f };

	float lowerUVs[4] = { 0.0f, 0.0f, 1.0f, 0.0f };
	float upperUVs[4] = { 0.0f, 1.0f, 1.0f, 1.0f };

	memcpy(pLayout->LowerLeftPos, lowerLeftPos, sizeof(lowerLeftPos));
	memcpy(pLayout->LowerRightPos, lowerRightPos, sizeof(lowerRightPos));
	memcpy(pLayout->UpperLeftPos, upperLeftPos, sizeof(upperLeftPos));
	memcpy(pLayout->UpperRightPos, upperRightPos, sizeof(upperRightPos));

	memcpy(pLayout->LowerUVs, lowerUVs, sizeof(lowerUVs));
	memcpy(pLayout->UpperUVs, upperUVs, sizeof(upperUVs));

	float identTransform[16] = { 1.0f, 0.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		0.0f, 0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f };
	memcpy(pLayout->TransformMatrix, identTransform, sizeof(pLayout->TransformMatrix));
}

#endif //SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS