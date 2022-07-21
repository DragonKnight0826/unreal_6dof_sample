//=============================================================================
// FILE: SnapdragonVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#include "SnapdragonVRHMD.h"
#include "Engine.h"
#include "HardwareInfo.h"
#include "../Public/SnapdragonVRHMDFunctionLibrary.h"

#include "../Classes/SnapdragonVRHMDEventManager.h"

#include "SnapdragonXR_CVars.h"

#include "XRThreadUtils.h"
#include "Renderer/Private/RendererPrivate.h"
#include "RenderCore/Public/RenderingThread.h"
#include "ScenePrivate.h"

#include "Engine/Public/Slate/SceneViewport.h"
#include "PostProcess/PostProcessHMD.h"

#include "GameFramework/PlayerController.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/Canvas.h"
#include "Engine/World.h"
#include "Misc/CoreDelegates.h"

#include "Core/Public/HAL/RunnableThread.h"


DECLARE_CYCLE_STAT(TEXT("Update Player Camera"), STAT_UpdatePlayerCamera, STATGROUP_Tickables);

#if PLATFORM_ANDROID

extern "C" GSXRResult GSXRInitializeOptArgs(const GSXRInitParams* pInitParams, void* pTmAPI);

/*
It appears that we must ensure that sxrEndVr() (which is always called on the CPU thread) never overlaps with
SC::GSXR_nativeSubmitDataFrame() (which is always called on the render thread) by mutexing bInVRMode in those cases.  Otherwise, we can
get mysterious deadlocks during suspend/resume (seemingly only if the phone is not plugged into USB!  Plugged in USB
phones never reproduced this deadlock).

It is equally important that we do *not* try to use this thread synchronization construct to guard against other times
bInVRMode is used (for example, at startup), or else a different mysterious deadlock on app launch occurs.  @todo NTF:
understand what's really going on here
*/
FCriticalSection FSnapdragonVRHMDCustomPresent::InVRModeCriticalSection;
FCriticalSection FSnapdragonVRHMDCustomPresent::PerfLevelCriticalSection;

#include <android_native_app_glue.h>

#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif // PLATFORM_ANDROID

#include "RHIStaticStates.h"
#include "Engine/Public/Slate/SceneViewport.h"

#include "DrawDebugHelpers.h"

#include "SCGSXRApi.h"

DEFINE_LOG_CATEGORY(LogSVR);

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

static const TCHAR* const GSXRPerfLevelSystemCStr = TEXT("sys");
static const TCHAR* const GSXRPerfLevelMinimumCStr = TEXT("min");
static const TCHAR* const GSXRPerfLevelMediumCStr = TEXT("med");
static const TCHAR* const GSXRPerfLevelMaximumCStr = TEXT("max");

//keep these two lines in sync (eg pass the string that corresponds to the enum)
static const TCHAR* const GSXRPerfLevelDefaultCStr = GSXRPerfLevelSystemCStr;
/*static*/ enum GSXRPerfLevel FSnapdragonVRHMD::GetCVarSnapdragonVrPerfLevelDefault() { return kGSXRPerfMedium; }

//these variables default values can be overridden by adding a line like this to Engine\Config\ConsoleVariables.ini: sxr.perfCpu=max
static TAutoConsoleVariable<FString> CVarPerfLevelCpu(TEXT("sxr.perfCpu"), GSXRPerfLevelDefaultCStr, TEXT("Set SnapdragonVr CPU performance consumption to one of the following: sys, min, med, max.  Note that if this value is set by Blueprint, the cvar will continue to report the last value it was set to, and will not reflect the value set by Blueprint"));
static TAutoConsoleVariable<FString> CVarPerfLevelGpu(TEXT("sxr.perfGpu"), GSXRPerfLevelDefaultCStr, TEXT("Set SnapdragonVr GPU performance consumption to one of the following: sys, min, med, max.  Note that if this value is set by Blueprint, the cvar will continue to report the last value it was set to, and will not reflect the value set by Blueprint"));

//BEG_Q3D_AUDIO_HACK
#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarQ3D_DrawSound(TEXT("q3d.drawSound"), 0, TEXT("Draws red tetrahedrons centered at each spatialized sound's position (hurts performance significantly)"));
static TAutoConsoleVariable<int32> CVarQ3D_DrawSoundName(TEXT("q3d.drawSoundName"), 0, TEXT("Draws red textual names of spatialized sounds near their positions (hurts performance dramatically)"));
static TAutoConsoleVariable<int32> CVarQ3D_LogSoundName(TEXT("q3d.logSoundName"), 0, TEXT("Logs sound names and positions of spatialized sounds each frame"));
static TAutoConsoleVariable<int32> CVarQ3D_LogSoundNumber(TEXT("q3d.logSoundNumber"), 0, TEXT("Logs the number of spatialized sounds playing on a frame"));
#endif//#if !UE_BUILD_SHIPPING
//END_Q3D_AUDIO_HACK


//-----------------------------------------------------------------------------
static bool FStringToSvrPerfLevel(enum GSXRPerfLevel* OutPerfLevel, const FString& InValueReceived)
{
    if (InValueReceived.Compare(FString(GSXRPerfLevelSystemCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kGSXRPerfSystem;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMinimumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kGSXRPerfMinimum;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMediumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kGSXRPerfMedium;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMaximumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kGSXRPerfMaximum;
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
static bool SvrPerfLevelToFString(FString* const OutPerfLevelFString, const enum GSXRPerfLevel InPerfLevel)
{
    switch (InPerfLevel)
    {
    case kGSXRPerfSystem:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelSystemCStr);
        return true;
    }
    case kGSXRPerfMinimum:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelMinimumCStr);
        return true;
    }
    case kGSXRPerfMedium:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelMediumCStr);
        return true;
    }
    case kGSXRPerfMaximum:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelMaximumCStr);
        return true;
    }
    default:
    {
        return false;
    }
    }
}

//-----------------------------------------------------------------------------
static bool IsPerfLevelValid(const enum GSXRPerfLevel InPerfLevel)
{
    return InPerfLevel >= kGSXRPerfSystem && InPerfLevel <= kGSXRPerfMaximum;
}

// CTORNE ->
/*static*/ enum GSXRPerfLevel FSnapdragonVRHMD::PerfLevelCpuLastSet = kGSXRNumPerfLevels, FSnapdragonVRHMD::PerfLevelGpuLastSet = kGSXRNumPerfLevels;
// <- CTORNE

//-----------------------------------------------------------------------------
/*static*/ void PerfLevelLastSet(enum GSXRPerfLevel* const OutPerfLevel, const enum GSXRPerfLevel InPerfLevel)
{
	check(IsPerfLevelValid(InPerfLevel));
	*OutPerfLevel = InPerfLevel;
}

//-----------------------------------------------------------------------------
/*static*/ void FSnapdragonVRHMD::PerfLevelCpuWrite(const enum GSXRPerfLevel InPerfLevel)
{
	PerfLevelLastSet(&PerfLevelCpuLastSet, InPerfLevel);
}

//-----------------------------------------------------------------------------
/*static*/ void FSnapdragonVRHMD::PerfLevelGpuWrite(const enum GSXRPerfLevel InPerfLevel)
{
	PerfLevelLastSet(&PerfLevelGpuLastSet, InPerfLevel);
}

//-----------------------------------------------------------------------------
/*static*/ void FSnapdragonVRHMD::PerfLevelLog(const TCHAR* const InPrefix,enum GSXRPerfLevel InPerfLevelCpu, enum GSXRPerfLevel InPerfLevelGpu)
{
#if !UE_BUILD_SHIPPING
    FString PerfLevelCpuLastSetFString, PerfLevelGpuLastSetFString;
    SvrPerfLevelToFString(&PerfLevelCpuLastSetFString, InPerfLevelCpu);
    SvrPerfLevelToFString(&PerfLevelGpuLastSetFString, InPerfLevelGpu);
    UE_LOG(LogSVR, Log, TEXT("%s:CPU = %s; GPU = %s"), InPrefix, *PerfLevelCpuLastSetFString, *PerfLevelGpuLastSetFString);
#endif//#if !UE_BUILD_SHIPPING
}

//-----------------------------------------------------------------------------
static bool PerfLevelLastSetByCvarRead(
	enum GSXRPerfLevel* OutPerfLevel,
	const TAutoConsoleVariable<FString>& InCVar,
	const enum GSXRPerfLevel InPerfLevelDefault)
{
	const bool bReadSucceeded = FStringToSvrPerfLevel(OutPerfLevel, InCVar.GetValueOnAnyThread());
	if (!bReadSucceeded)
	{
		*OutPerfLevel = InPerfLevelDefault;
	}

	return bReadSucceeded;
}

//-----------------------------------------------------------------------------
/*static*/ bool FSnapdragonVRHMD::PerfLevelsLastSetByCvarRead(
	enum GSXRPerfLevel* OutPerfLevelCpuCurrent,
	enum GSXRPerfLevel* OutPerfLevelGpuCurrent,
	const enum GSXRPerfLevel InPerfLevelCpuDefault,
	const enum GSXRPerfLevel InPerfLevelGpuDefault)
{
	bool bValid = PerfLevelLastSetByCvarRead(OutPerfLevelCpuCurrent, CVarPerfLevelCpu, InPerfLevelCpuDefault);
	bValid &= PerfLevelLastSetByCvarRead(OutPerfLevelGpuCurrent, CVarPerfLevelGpu, InPerfLevelGpuDefault);
	return bValid;
}

//-----------------------------------------------------------------------------
static void PerfLevelOnChangeByCvar(enum GSXRPerfLevel* const OutPerfLevelToSet, const IConsoleVariable* const InConsoleVar, const TCHAR* const InLogPrefix)
{
	//sxr performance levels can be manipulated by render or game thread, so prevent race conditions
	FScopeLock ScopeLock(&FSnapdragonVRHMDCustomPresent::PerfLevelCriticalSection);

	enum GSXRPerfLevel PerfLevel;
	const bool bReadSucceeded = FStringToSvrPerfLevel(&PerfLevel, InConsoleVar->GetString());
	if (bReadSucceeded)
	{
		PerfLevelLastSet(OutPerfLevelToSet, PerfLevel);
		FSnapdragonVRHMD::PerfLevelLog(InLogPrefix, FSnapdragonVRHMD::PerfLevelCpuLastSet, FSnapdragonVRHMD::PerfLevelGpuLastSet);
		SC::GSXR_nativeSetPerformanceLevels(FSnapdragonVRHMD::PerfLevelCpuLastSet, FSnapdragonVRHMD::PerfLevelGpuLastSet);
	}
}

//-----------------------------------------------------------------------------
static void PerfLevelCpuOnChangeByCvar(IConsoleVariable* InVar)
{
	PerfLevelOnChangeByCvar(&FSnapdragonVRHMD::PerfLevelCpuLastSet, InVar, TEXT("sxr.perfCpu cvar"));
}

//-----------------------------------------------------------------------------
static void PerfLevelGpuOnChangeByCvar(IConsoleVariable* InVar)
{
	PerfLevelOnChangeByCvar(&FSnapdragonVRHMD::PerfLevelGpuLastSet, InVar, TEXT("sxr.perfGpu cvar"));
}
#endif//#if SNAPDRAGONVR_SUPPORTED_PLATFORMS

const FName FSnapdragonVRHMD::SnapdragonVRHMDSystemName(TEXT("SnapdragonVRHMD"));



//-----------------------------------------------------------------------------
// FSnapdragonVRHMD, IHeadMountedDisplay Implementation
//-----------------------------------------------------------------------------



#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

//-----------------------------------------------------------------------------
// Grab a pointer to the Snapdragon XR system
FSnapdragonVRHMD* FSnapdragonVRHMD::GetSnapdragonHMD()
{
	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetSystemName() == FSnapdragonVRHMD::SnapdragonVRHMDSystemName)
	{
		return static_cast<FSnapdragonVRHMD*>(GEngine->XRSystem.Get());
	}

	return nullptr;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::IsHMDEnabled() const
{
    return true;
}
//-----------------------------------------------------------------------------
FString FSnapdragonVRHMD::GetVersionString() const
{
	FString s = FString::Printf(TEXT("SnapdragonVR - %s, built %s, %s"),
		*FEngineVersion::Current().ToString(), UTF8_TO_TCHAR(__DATE__), UTF8_TO_TCHAR(__TIME__));
	return s;
}
//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::HasValidTrackingPosition()
{
	// this actually tries to get one
	return DoesSupportPositionalTracking();
}

//-----------------------------------------------------------------------------
// void FSnapdragonVRHMD::SetFinalViewRect(const enum EStereoscopicPass StereoPass, const FIntRect& FinalViewRect)
// {
// //	CheckInRenderThread();

// 	//UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::SetFinalViewRect()"));

// //	const int32 ViewIndex = ViewIndexFromStereoPass(StereoPass);

// //	ExecuteOnRHIThread_DoNotWait([this, ViewIndex, FinalViewRect]()
// //	{
// //		CheckInRHIThread();

// 		//if (Frame_RHIThread.IsValid())
// 		//{
// 		//	Frame_RHIThread->FinalViewRect[ViewIndex] = FinalViewRect;
// 		//}
// //	});
// }

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type /*= EXRTrackedDeviceType::Any*/)
{
	if (Type == EXRTrackedDeviceType::Any || Type == EXRTrackedDeviceType::HeadMountedDisplay)
	{
		OutDevices.Add(IXRTrackingSystem::HMDDeviceId);
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::EnumerateTrackedDevices() - added device"));
		return true;
	}
	UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::EnumerateTrackedDevices() - no device added"));
	return false;
 }

//-----------------------------------------------------------------------------
void  FSnapdragonVRHMD::UpdatePoses()
// rbf validate
{
	// Don't update poses if on the render thread
	if (IsInRenderingThread())
	{
		return;
	}



	// UE_LOG(LogSVR, Error, TEXT("sxr (%s) (Frame %d) UpdatePoses() => Calling GetHeadPoseState"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber);
	// sxrprint("(%s) (Frame %d) UpdatePoses() => Calling GetHeadPoseState", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber);

	GetHeadPoseState(CachedHeadPoseState);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::UpdateMyPoses(GSXRHeadPoseState& Pose)
{
	GetHeadPoseState(Pose);
}

//-----------------------------------------------------------------------------
// rbf validate
bool FSnapdragonVRHMD::GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;

	if (DeviceId != HMDDeviceId)
	{
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetCurrentPose() - Device error %d != %d"), DeviceId, HMDDeviceId);
		return false;
	}

	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetCurrentPose() from %s thread for frame:%d/%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, GFrameNumberRenderThread );

	//PoseToOrientationAndPosition(CachedHeadPoseState.pose, CurrentOrientation, CurrentPosition, GetWorldToMetersScale());
	//CurHmdOrientation = LastHmdOrientation = BaseOrientation * CurrentOrientation;
	//CurHmdPosition = LastHmdPosition = CurrentPosition;
	// if(!bStartRendering)
	// {
	// 	return false;
	// }
	// UpdateSensorData();	
	if (IsInRenderingThread())
	{
		if (bIsEndGameFrame)
		{
			bIsEndGameFrame = false;
			UpdateSensorData();	
		}
		OutOrientation = CurrentFrame_RenderThread.Orientation;
		OutPosition = CurrentFrame_RenderThread.Position;
	}else if (IsInGameThread())
	{
		OutOrientation = CurrentFrame_GameThread.Orientation;
		OutPosition = CurrentFrame_GameThread.Position;
	}

	return true;
}

void FSnapdragonVRHMD::UpdateSensorData()
{
	float PredictedTime = SC::GSXR_nativeGetPredictedDisplayTime();

	
	// sxrprint("(%s) (Frame %d) GetHeadPoseState() => Getting head pose for %0.2f ms", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	
	GSXRHeadPoseState HeadPoseState = SC::GSXR_nativeGetPredictedPose(PredictedTime);

	FVector CurrentPosition;
	FQuat CurrentOrientation;
	PoseToOrientationAndPosition(HeadPoseState.pose, CurrentOrientation, CurrentPosition, GetWorldToMetersScale());
	CurrentFrame_RenderThread.Orientation= LastHmdOrientation = CurrentOrientation;
	CurrentFrame_RenderThread.Position = LastHmdPosition = CurrentPosition;
	CurrentFrame_RenderThread.headPoseState = HeadPoseState;
	CurrentFrame_RenderThread.PredictedDisplayTime = PredictedTime;

	UE_LOG(LogSVR, Log, TEXT("sxr (%s) (Frame %llu) GetHeadPoseState() => Getting head pose for %0.2f ms (%f,%f,%f),(%f,%f,%f,%f)"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime,
	HeadPoseState.pose.position.x,HeadPoseState.pose.position.y,HeadPoseState.pose.position.z,HeadPoseState.pose.rotation.x,HeadPoseState.pose.rotation.y,HeadPoseState.pose.rotation.z,HeadPoseState.pose.rotation.w);
}

#pragma region EyeTracking

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::isEyeTrackingEnabled()const
{
	return  0 != CVars::EyeTrackingEnabled;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::enableEyeTracking(bool b)
{
	CVars::EyeTrackingEnabled = b ? 1 : 0;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::GetEyePoseState(int32 DeviceId, GSXREyePoseState& EyePoseState)
{
	if (DeviceId != HMDDeviceId)
	{
		UE_LOG(LogSVR, Error, TEXT("FSnapdragonVRHMD::GetEyePoseState() - Device error %d != %d"), DeviceId, HMDDeviceId);
		return false;
	}

	/*if (!pSnapdragonVRBridge->bInVRMode)
	{
		UE_LOG(LogSVR, Error, TEXT("FSnapdragonVRHMD::GetEyePoseState() - Not in VRMode"));
		return false;
	}*/

	if ((SC::GSXR_nativeGetSupportSlamMode() & kGSXRTrackingEye) == 0)
	{
		UE_LOG(LogSVR, Error, TEXT("FSnapdragonVRHMD::GetEyePoseState() - EyeTracking not available"));
		return false;
	}

	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetEyePoseState() from %s thread for frame:%d/%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, GFrameNumberRenderThread);

	if (SC::GSXR_nativeGetEyePose(&EyePoseState) != GSXR_ERROR_NONE)
	{	// if you get here...
		// SC::GSXR_nativeGetEyePose will write out the error into the log  - look for "SC::GSXR_nativeGetEyePose Failed" because the call to the service failed...
		UE_LOG(LogSVR, Error, TEXT("FSnapdragonVRHMD::GetEyePoseState - SC::GSXR_nativeGetEyePose Failed"));
		return false;
	}

	//UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetEyePoseState() says left position is %s and is [%6.3f,%6.3f,%6.3f]"),
	//	isEyeGazePointValid(EyePoseState.leftEyePoseStatus) ? TEXT("Valid") : TEXT("Invalid"), EyePoseState.leftEyeGazePoint[0], EyePoseState.leftEyeGazePoint[1], EyePoseState.leftEyeGazePoint[2]);
	//UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetEyePoseState() says left direction is %s and is [%6.3f,%6.3f,%6.3f]"),
	//	isEyeGazeDirectionValid(EyePoseState.leftEyePoseStatus) ? TEXT("Valid") : TEXT("Invalid"), EyePoseState.leftEyeGazeVector[0], EyePoseState.leftEyeGazeVector[1], EyePoseState.leftEyeGazeVector[2]);

	return true;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::GetRelativeEyeDirection(int32 DeviceId, FVector& OutDirection)
{
	// Apply CS transform position SVR to UE4 (-Z[2], X[0], Y[1])
	FVector eyeDirections[3] =
	{
		FVector(-CachedEyePoseState.leftEyeGazeVector[2], CachedEyePoseState.leftEyeGazeVector[0], CachedEyePoseState.leftEyeGazeVector[1]),
		FVector(-CachedEyePoseState.rightEyeGazeVector[2], CachedEyePoseState.rightEyeGazeVector[0], CachedEyePoseState.rightEyeGazeVector[1]),
		FVector(-CachedEyePoseState.combinedEyeGazeVector[2], CachedEyePoseState.combinedEyeGazeVector[0], CachedEyePoseState.combinedEyeGazeVector[1])
	};

	bool bGazeVectorValidLeft  = isEyeGazeDirectionValid(CachedEyePoseState.leftEyePoseStatus);
	bool bGazeVectorValidRight = isEyeGazeDirectionValid(CachedEyePoseState.rightEyePoseStatus);
	bool bGazeVectorValidBoth  = isEyeGazeDirectionValid(CachedEyePoseState.combinedEyePoseStatus);

	// Currently, combined only valid if both left and right valid too 
	if (bGazeVectorValidBoth && bGazeVectorValidLeft && bGazeVectorValidRight) // Use combinedEyeDirection
	{
		OutDirection = eyeDirections[2];
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetRelativeEyeDirection combinedEyeDirection = x:%f, y:%f, z:%f"), OutDirection.X, OutDirection.Y, OutDirection.Z);
		return true;
	}
	else if ( bGazeVectorValidLeft || bGazeVectorValidRight )	// Average left/rightEyeDirection
	{
		OutDirection = FVector::ZeroVector;
		if (bGazeVectorValidLeft) OutDirection += eyeDirections[0];
		if (bGazeVectorValidRight) OutDirection += eyeDirections[1];
		OutDirection.Normalize();
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::GetRelativeEyeDirection averagedEyeDirection = x:%f, y:%f, z:%f"), OutDirection.X, OutDirection.Y, OutDirection.Z);
		return true;

		//dbgprint("FSnapdragonVRHMD::GetRelativeEyeDirection EyeDirection = x:%f, y:%f, z:%f", OutDirection.X, OutDirection.Y, OutDirection.Z);
	}
	return false;
}

//-----------------------------------------------------------------------------
// We don't really follow the instructions for this call....
bool FSnapdragonVRHMD::GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;
	if (DeviceId == HMDDeviceId && (Eye == eSSP_LEFT_EYE || Eye == eSSP_RIGHT_EYE))
	{
		OutPosition = FVector(0, (Eye == eSSP_LEFT_EYE ? -.5 : .5) * GetInterpupillaryDistance() * GetWorldToMetersScale(), 0);
		return true;
	}
	return false;
}

//-----------------------------------------------------------------------------
const GSXREyePoseState& FSnapdragonVRHMD::GetLatestEyePoseState() // GetCurrentEyePose()
{
	return CachedEyePoseState;
}

#pragma endregion EyeTracking
//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::EnableHMD(bool allow)
{
	// EnableStereo(allow);
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::GetHMDMonitorInfo(MonitorInfo& MonitorDesc)
{
    // if (IsInitialized())
    // {
    //     MonitorDesc.MonitorName = "";
    //     MonitorDesc.MonitorId = 0;
    //     MonitorDesc.DesktopX = MonitorDesc.DesktopY = 0;
    //     MonitorDesc.ResolutionX = RenderTargetSize.X;
    //     MonitorDesc.ResolutionY = RenderTargetSize.Y;
    //     return true;
    // }
    // else
    // {
	// 	MonitorDesc.MonitorName = "";
    //     MonitorDesc.MonitorId = 0;
    //     MonitorDesc.DesktopX = MonitorDesc.DesktopY = MonitorDesc.ResolutionX = MonitorDesc.ResolutionY = 0;
    // }

    return true;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const
{
	if(!bInitialized)
	{
		OutHFOVInDegrees = 90.0;
		OutVFOVInDegrees = 90.0;
		return;
	}
	GSXRDeviceInfo di = SC::GSXR_nativeGetDeviceInfo();
	
    OutHFOVInDegrees = FMath::RadiansToDegrees(di.targetFovXRad);
    OutVFOVInDegrees = FMath::RadiansToDegrees(di.targetFovYRad);
	// UE_LOG(LogSVR,Log,TEXT("xmh fov:(%f,%f)"),OutHFOVInDegrees,OutVFOVInDegrees);
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::DoesSupportPositionalTracking() const
{
	if(!bInitialized)
	{
		return false;
	}
	GSXRDeviceInfo di = SC::GSXR_nativeGetDeviceInfo();
    const unsigned int supportedTrackingModes = SC::GSXR_nativeGetSupportSlamMode();

    const bool bTrackingPosition = supportedTrackingModes & kGSXRTrackingPosition;
	UE_LOG(LogSVR,Log,TEXT("DoesSupportPositionalTracking:%d"),bTrackingPosition);
    return bTrackingPosition;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance)
{
	UE_LOG(LogSVR, Log, TEXT("SnapdragonVR - call to SetInterpupillaryDistance is nop"));
    //Do Nothing
}

float FSnapdragonVRHMD::GetInterpupillaryDistance() const
{
    return 0.064f;
}


// TSharedPtr<ISceneViewExtension, ESPMode::ThreadSafe> FSnapdragonVRHMD::GetViewExtension()
// {
//     TSharedPtr<FSnapdragonVRHMD, ESPMode::ThreadSafe> ptr(AsShared());
//     return StaticCastSharedPtr<ISceneViewExtension>(ptr);
// }


//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::IsChromaAbCorrectionEnabled() const
{
    return false;
}

//-----------------------------------------------------------------------------
// bool FSnapdragonVRHMD::IsHeadTrackingAllowed() const
// {
//     return true;
// }

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ResetOrientationAndPosition(float yaw)
{
    ResetOrientation(yaw);
    ResetPosition();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ResetOrientation(float yaw)
{
	UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ResetOrientation()"));
	SC::GSXR_nativeRecenterOrientation(true);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ResetPosition()
{
	UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ResetPosition()"));
	SC::GSXR_nativeRecenterPosition();
}

//-----------------------------------------------------------------------------
float FSnapdragonVRHMD::GetWorldToMetersScale() const
{
	return GWorld ? GWorld->GetWorldSettings()->WorldToMeters : 100.0f;  // RBF validate
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetBaseRotation(const FRotator& BaseRot)
{
	// SetBaseOrientation(FRotator(0.0f, BaseRot.Yaw, 0.0f).Quaternion());
}

//-----------------------------------------------------------------------------
FRotator FSnapdragonVRHMD::GetBaseRotation() const
{
	// return GetBaseOrientation().Rotator();
	return FRotator::ZeroRotator;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetBaseOrientation(const FQuat& BaseOrient)
{
	// BaseOrientation = BaseOrient;
}

//-----------------------------------------------------------------------------
FQuat FSnapdragonVRHMD::GetBaseOrientation() const
{
	// return BaseOrientation;
	return FQuat::Identity;
}

//-----------------------------------------------------------------------------
static void DrawOcclusionMesh(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass, const FHMDViewMesh MeshAssets[])
{
	check(InRenderThread());
	check(StereoPass != eSSP_FULL);

	const uint32 MeshIndex = (StereoPass == eSSP_LEFT_EYE) ? 0 : 1;
	const FHMDViewMesh& Mesh = MeshAssets[MeshIndex];
	check(Mesh.IsValid());

	RHICmdList.SetStreamSource(0, Mesh.VertexBufferRHI, 0);
	RHICmdList.DrawIndexedPrimitive(Mesh.IndexBufferRHI, 0, 0, Mesh.NumVertices, 0, Mesh.NumTriangles, 1);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::DrawHiddenAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const
{
	DrawOcclusionMesh(RHICmdList, StereoPass, HiddenAreaMeshes);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::DrawVisibleAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const
{
    //Do Nothing
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize)
{
    //Do Nothing
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::OnBeginPlay(FWorldContext& InWorldContext)
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnBeginPlay"));
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::OnEndPlay(FWorldContext& InWorldContext)
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnEndPlay"));
	///StopWebHelper();
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::OnStartGameFrame( FWorldContext& WorldContext )
{
	UE_LOG(LogSVR,Log,TEXT("xmh OnStartGameFrame"));
    //check(pSnapdragonVRBridge)

    //if sensor fusion is inactive, but the app has been resumed and initialized on both the main thread and graphics thread, then start sensor fusion
    /*if (bResumed && 
        pSnapdragonVRBridge->bContextInitialized && 
        !pSnapdragonVRBridge->bInVRMode &&
        bInitialized)
    {
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnStartGameFrame() - starting"));
        BeginVRMode();
    }*/

	// bool ReturnVal = true;  //  RBF cleanup

	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnStartGameFrame()"));


	// UpdatePoses(); // GetHeadPoseState
	// UpdateSensorData();	
	// cache the eye pose info if we're eyetracking
	// if ( isEyeTrackingEnabled() )
	// {
	// 	GetEyePoseState(IXRTrackingSystem::HMDDeviceId, CachedEyePoseState);
	// }

	// RefreshTrackingToWorldTransform(WorldContext);
	LastFrame_GameThread = CurrentFrame_GameThread;
	CurrentFrame_GameThread = LastFrame_RenderThread;
    return true;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::OnEndGameFrame( FWorldContext& WorldContext )
{
	SendEvents();
	ExecuteOnRenderThread_DoNotWait([this]()
	{
		LastFrame_RenderThread = CurrentFrame_RenderThread;
		//CurrentFrame_RenderThread = CurrentFrame_GameThread;
		this->bIsEndGameFrame = true;
	});
    return true;
}

//-----------------------------------------------------------------------------
/**
 * Called on the game thread when view family is about to be rendered.
 */
void FSnapdragonVRHMD::OnBeginRendering_GameThread()
{
	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnBeginRendering_GameThread()"));
}

//-----------------------------------------------------------------------------
/**
 * Called on the render thread at the start of rendering.
 */
void FSnapdragonVRHMD::OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::OnBeginRendering_RenderThread()"));
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::GetHeadPoseState(GSXRHeadPoseState& HeadPoseState)
{
	// Can only update the pose if vr has been started
	// if (!pSnapdragonVRBridge->bStartRendering)
	// {
	// 	return false;
	// }

	//! \brief Calculates a predicted head pose
	//! \param predictedTimeMs Time ahead of the current time in ms to predict a head pose for
	//! \return The predicted head pose and relevant pose state information
	float PredictedTime = SC::GSXR_nativeGetPredictedDisplayTime();

	// UE_LOG(LogSVR, Error, TEXT("sxr (%s) (Frame %llu) GetHeadPoseState() => Getting head pose for %0.2f ms"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	// sxrprint("(%s) (Frame %d) GetHeadPoseState() => Getting head pose for %0.2f ms", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	
	HeadPoseState = SC::GSXR_nativeGetPredictedPose(PredictedTime);

	FVector CurrentPosition;
	FQuat CurrentOrientation;
	PoseToOrientationAndPosition(HeadPoseState.pose, CurrentOrientation, CurrentPosition, GetWorldToMetersScale());
	CurHmdOrientation = LastHmdOrientation = BaseOrientation * CurrentOrientation;
	CurHmdPosition = LastHmdPosition = CurrentPosition;

	return true;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SendEvents()
{
	GSXREvent SxrEvent;
	while (SC::GSXR_nativePollEvent(&SxrEvent))
	{
		switch (SxrEvent.eventType)
		{
		case GSXREventType::kGSXREventSdkServiceStarting:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRSdkServiceDelegate.Broadcast(ESnapdragonVRSdkServiceEventType::EventSdkServiceStarting);
			break;
		case GSXREventType::kGSXREventSdkServiceStarted:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRSdkServiceDelegate.Broadcast(ESnapdragonVRSdkServiceEventType::EventSdkServiceStarted);
			break;
		case GSXREventType::kGSXREventSdkServiceStopped:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRSdkServiceDelegate.Broadcast(ESnapdragonVRSdkServiceEventType::EventSdkServiceStopped);
			break;
		case GSXREventType::kGSXREventThermal:
		{
			ESnapdragonVRThermalZone ThermalZone = static_cast<ESnapdragonVRThermalZone>(SxrEvent.eventData.thermal.zone);
			ESnapdragonVRThermalLevel ThermalLevel = static_cast<ESnapdragonVRThermalLevel>(SxrEvent.eventData.thermal.level);

			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRThermalDelegate.Broadcast(ThermalZone, ThermalLevel);
			break;
		}
		case GSXREventType::kGSXREventVrModeStarted:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRModeDelegate.Broadcast(ESnapdragonVRModeEventType::EventVrModeStarted);
			break;
		case GSXREventType::kGSXREventVrModeStopping:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRModeDelegate.Broadcast(ESnapdragonVRModeEventType::EventVrModeStopping);
			break;
		case GSXREventType::kGSXREventVrModeStopped:
			USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()->OnSnapdragonVRModeDelegate.Broadcast(ESnapdragonVRModeEventType::EventVrModeStopped);
			break;

		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::DrawDebug(class UCanvas* Canvas, class APlayerController*)
{
    Canvas->DrawText(GEngine->GetLargeFont(), FText::Format( FText::FromString("Debug Draw {0}"), FText::AsNumber(GFrameNumber)), 10.f, 10.f);

	DrawDebugTrackingCameraFrustum(GWorld, Canvas->SceneView->ViewRotation, Canvas->SceneView->ViewLocation);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::DrawDebugTrackingCameraFrustum(UWorld* World, const FRotator& ViewRotation, const FVector& ViewLocation)
{
#if 0
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FSnapdragonVRHMD::DrawDebugTrackingCameraFrustum"));

	const FColor c = (HasValidTrackingPosition() ? FColor::Green : FColor::Red);
	FVector origin;
	FQuat orient;
	float lfovDeg, rfovDeg, tfovDeg, bfovDeg, nearPlane, farPlane, cameraDist;
	uint32 nSensors = GetNumOfTrackingSensors();
	for (uint8 sensorIndex = 0; sensorIndex < nSensors; ++sensorIndex)
	{
		GetTrackingSensorProperties(sensorIndex, origin, orient, lfovDeg, rfovDeg, tfovDeg, bfovDeg, cameraDist, nearPlane, farPlane);

		FVector HeadPosition;
		FQuat HeadOrient;
		GetCurrentOrientationAndPosition(HeadOrient, HeadPosition);
		const FQuat DeltaControlOrientation = ViewRotation.Quaternion() * HeadOrient.Inverse();

		orient = DeltaControlOrientation * orient;

		origin = DeltaControlOrientation.RotateVector(origin);

		const float lfov = FMath::DegreesToRadians(lfovDeg);
		const float rfov = FMath::DegreesToRadians(rfovDeg);
		const float tfov = FMath::DegreesToRadians(tfovDeg);
		const float bfov = FMath::DegreesToRadians(bfovDeg);
		FVector coneTop(0, 0, 0);
		FVector coneBase1(-farPlane, farPlane * FMath::Tan(rfov), farPlane * FMath::Tan(tfov));
		FVector coneBase2(-farPlane, -farPlane * FMath::Tan(lfov), farPlane * FMath::Tan(tfov));
		FVector coneBase3(-farPlane, -farPlane * FMath::Tan(lfov), -farPlane * FMath::Tan(bfov));
		FVector coneBase4(-farPlane, farPlane * FMath::Tan(rfov), -farPlane * FMath::Tan(bfov));
		FMatrix m(FMatrix::Identity);
		m = orient * m;
		//m *= FScaleMatrix(frame->CameraScale3D);
		m *= FTranslationMatrix(origin);
		m *= FTranslationMatrix(ViewLocation); // to location of pawn
		coneTop = m.TransformPosition(coneTop);
		coneBase1 = m.TransformPosition(coneBase1);
		coneBase2 = m.TransformPosition(coneBase2);
		coneBase3 = m.TransformPosition(coneBase3);
		coneBase4 = m.TransformPosition(coneBase4);

		// draw a point at the camera pos
		DrawDebugPoint(World, coneTop, 5, c);

		// draw main pyramid, from top to base
		DrawDebugLine(World, coneTop, coneBase1, c);
		DrawDebugLine(World, coneTop, coneBase2, c);
		DrawDebugLine(World, coneTop, coneBase3, c);
		DrawDebugLine(World, coneTop, coneBase4, c);

		// draw base (far plane)				  
		DrawDebugLine(World, coneBase1, coneBase2, c);
		DrawDebugLine(World, coneBase2, coneBase3, c);
		DrawDebugLine(World, coneBase3, coneBase4, c);
		DrawDebugLine(World, coneBase4, coneBase1, c);

		// draw near plane
		FVector coneNear1(-nearPlane, nearPlane * FMath::Tan(rfov), nearPlane * FMath::Tan(tfov));
		FVector coneNear2(-nearPlane, -nearPlane * FMath::Tan(lfov), nearPlane * FMath::Tan(tfov));
		FVector coneNear3(-nearPlane, -nearPlane * FMath::Tan(lfov), -nearPlane * FMath::Tan(bfov));
		FVector coneNear4(-nearPlane, nearPlane * FMath::Tan(rfov), -nearPlane * FMath::Tan(bfov));
		coneNear1 = m.TransformPosition(coneNear1);
		coneNear2 = m.TransformPosition(coneNear2);
		coneNear3 = m.TransformPosition(coneNear3);
		coneNear4 = m.TransformPosition(coneNear4);
		DrawDebugLine(World, coneNear1, coneNear2, c);
		DrawDebugLine(World, coneNear2, coneNear3, c);
		DrawDebugLine(World, coneNear3, coneNear4, c);
		DrawDebugLine(World, coneNear4, coneNear1, c);

		// center line
		FVector centerLine(-cameraDist, 0, 0);
		centerLine = m.TransformPosition(centerLine);
		DrawDebugLine(World, coneTop, centerLine, FColor::Yellow);
		DrawDebugPoint(World, centerLine, 5, FColor::Yellow);
	}
#endif
}


//-----------------------------------------------------------------------------
// FSnapdragonVRHMD, IStereoRendering Implementation
//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::IsStereoEnabled() const
{
    return true;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::EnableStereo(bool stereo)
{

	// GEngine->bForceDisableFrameRateSmoothing = stereo;
    return true;
}

void FSnapdragonVRHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
{
	if(!bIsMobileMultiViewEnabled)
	{
		SizeX = GetIdealRenderTargetSize().X / 2;
	}else
	{
		SizeX = GetIdealRenderTargetSize().X;
	}

	SizeY = GetIdealRenderTargetSize().Y;
	if (StereoPass == eSSP_RIGHT_EYE)
	{
		X += SizeX;
	}
	if (bIsMobileMultiViewEnabled) {
		X = 0;
	}

	// UE_LOG(LogSVR, Log, TEXT("AdjustViewRectsiez(%d,%d)(%d,%d)"), X,Y, SizeX, SizeY);

}

//-----------------------------------------------------------------------------
// void FSnapdragonVRHMD::CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation)
// {
//     if( StereoPassType != eSSP_FULL)
//     {
//         const float EyeOffset = (GetInterpupillaryDistance() * 0.5f) * WorldToMeters;
//         const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? -EyeOffset : EyeOffset;
//         ViewLocation += ViewRotation.Quaternion().RotateVector(FVector(0,PassOffset,0));

// 		//UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::CalculateStereoViewOffset x:%f, y:%f, z:%f"), ViewLocation.X, ViewLocation.Y, ViewLocation.Z);
//     }
// }

//-----------------------------------------------------------------------------
FMatrix FSnapdragonVRHMD::GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const
{
	// GSXRDeviceInfo di = SC::GSXR_nativeGetDeviceInfo();

	// GSXRViewFrustum Frustum = (StereoPassType == eSSP_LEFT_EYE) ? di.leftEyeFrustum : di.rightEyeFrustum;

	// float InvNearZ = 1.0f / Frustum.near;

	// float Right = Frustum.right * InvNearZ;
	// float Left = Frustum.left * InvNearZ;
	// float Bottom = Frustum.bottom * InvNearZ;
	// float Top = Frustum.top * InvNearZ;

	// float ZNear = GNearClippingPlane;

	// float SumRL = (Right + Left);
	// float SumTB = (Top + Bottom);
	// float InvRL = (1.0f / (Right - Left));
	// float InvTB = (1.0f / (Top - Bottom));

	// return FMatrix(
	// 	FPlane((2.0f * InvRL), 0.0f, 0.0f, 0.0f),
	// 	FPlane(0.0f, (2.0f * InvTB), 0.0f, 0.0f),
	// 	FPlane((SumRL * InvRL), (SumTB * InvTB), 0.0f, 1.0f),
	// 	FPlane(0.0f, 0.0f, ZNear, 0.0f)
	// );

	float HFov,VFov;
	float ZNear = GNearClippingPlane;
	GetFieldOfView(HFov,VFov);
	float Left = -FPlatformMath::Tan(HFov/2);
	float Right = FPlatformMath::Tan(HFov/2);
	float Top = FPlatformMath::Tan(VFov/2);
	float Bottom = -FPlatformMath::Tan(VFov/2);
	float SumRL = (Right + Left);
	float SumTB = (Top + Bottom);
	float InvRL = (1.0f / (Right - Left));
	float InvTB = (1.0f / (Top - Bottom));
	FMatrix ProjectionMatrix = FMatrix(
			FPlane((2.0f * InvRL), 0.0f, 0.0f, 0.0f),
			FPlane(0.0f, (2.0f * InvTB), 0.0f, 0.0f),
			FPlane((SumRL * -InvRL), (SumTB * -InvTB), 0.0f, 1.0f),
			FPlane(0.0f, 0.0f, ZNear, 0.0f)
		);
	return ProjectionMatrix;
}

FIntPoint FSnapdragonVRHMD::GetIdealRenderTargetSize() const
{
	// UE_LOG(LogSVR,Log,TEXT("xmh GetIdealRenderTargetSize:(%d,%d)"),RenderTargetSize.X,RenderTargetSize.Y)
	return RenderTargetSize;
}

//-----------------------------------------------------------------------------
// void FSnapdragonVRHMD::InitCanvasFromView(FSceneView* InView, UCanvas* Canvas)
// {
//     //Do Nothing
// }

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
{
	if (Context.View.StereoPass == eSSP_LEFT_EYE)
	{
		EyeToSrcUVOffsetValue.X = 0.0f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}
	else
	{
		EyeToSrcUVOffsetValue.X = 0.5f;
		EyeToSrcUVOffsetValue.Y = 0.0f;

		EyeToSrcUVScaleValue.X = 0.5f;
		EyeToSrcUVScaleValue.Y = 1.0f;
	}
    // EyeToSrcUVOffsetValue = FVector2D::ZeroVector;
    // EyeToSrcUVScaleValue = FVector2D(1.0f, 1.0f);
	UE_LOG(LogSVR, Log, TEXT("GetEyeRenderParams_RenderThread(EyeToSrcUVOffsetValue.X:%f,EyeToSrcUVOffsetValue.Y:%f)(EyeToSrcUVScaleValue.X:%f,EyeToSrcUVScaleValue.Y:%f)"), EyeToSrcUVOffsetValue.X, EyeToSrcUVOffsetValue.Y, EyeToSrcUVScaleValue.X, EyeToSrcUVScaleValue.Y);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
    check(IsInGameThread());

	InOutSizeX = GetIdealRenderTargetSize().X;
	InOutSizeY = GetIdealRenderTargetSize().Y;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
{
    check(IsInGameThread());

    const uint32 InSizeX = Viewport.GetSizeXY().X;
    const uint32 InSizeY = Viewport.GetSizeXY().Y;

    FIntPoint RTSize;
	RTSize.X = Viewport.GetRenderTargetTexture()->GetSizeX();
	RTSize.Y = Viewport.GetRenderTargetTexture()->GetSizeY();

    uint32 NewSizeX = InSizeX, NewSizeY = InSizeY;
    CalculateRenderTargetSize(Viewport, NewSizeX, NewSizeY);
    if (NewSizeX != RTSize.X || NewSizeY != RTSize.Y)
    {
		// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::NeedReAllocateViewportRenderTarget returns true"));
		return true;
    }
    
	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::NeedReAllocateViewportRenderTarget returns false"));
	return false;
}

//-----------------------------------------------------------------------------
//bool FSnapdragonVRHMD::ShouldUseSeparateRenderTarget() const
//{
//
//	//UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ShouldUseSeparateRenderTarget() called"));
//    check(IsInGameThread());
//    return IsStereoEnabled();
//}

//-----------------------------------------------------------------------------
//void FSnapdragonVRHMD::UpdateViewport(bool bUseSeparateRenderTarget, const FViewport& InViewport, SViewport* ViewportWidget)
//{
//    check(IsInGameThread());
//    FRHIViewport* const ViewportRHI = InViewport.GetViewportRHI().GetReference();
//
//	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::UpdateViewport 1"));
//
//    if (!IsStereoEnabled())
//    {
//        if (!bUseSeparateRenderTarget)
//        {
//            ViewportRHI->SetCustomPresent(nullptr);
//			// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::UpdateViewport 2"));
//
//        }
//        return;
//    }
//	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::UpdateViewport 3"));
//	pSnapdragonVRBridge->UpdateViewport(InViewport, ViewportRHI);
//}

//-----------------------------------------------------------------------------
// FSnapdragonVRHMD, ISceneViewExtension Implementation
//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetupViewFamily(FSceneViewFamily& InViewFamily) 
{
#if WITH_EDITOR
	InViewFamily.EngineShowFlags.SetScreenPercentage(false);
#endif

	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::SetupViewFamily() and stereo is %s"), InViewFamily.EngineShowFlags.StereoRendering ? TEXT("enabled"):TEXT("disabled"));

}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
   
 	//InView.ViewRect = EyeRenderViewport[EyeIndex];
	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::SetupView()"));

}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{

}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
    check(IsInRenderingThread());

	// RBF - I suspect this has to be rewritten

	UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::PreRenderViewFamily_RenderThread()"));

    // pSnapdragonVRBridge->BeginRendering(RHICmdList, InViewFamily);
	ExecuteOnRHIThread([this]()
	{
		BeginFrame_RHIThread();
		PreRenderFrame_RHIThread();
	});

/*
	const FSceneView* MainView = InViewFamily.Views[0];
	const float WorldToMetersScale = MainView->WorldToMetersScale;

	FSnapdragonVRHMDCustomPresent::FPoseStateFrame* PoseState = pSnapdragonVRBridge->PoseHistory.Get();
	if (!PoseState)
	{
		return;
	}

	FQuat OldOrientation;
	FVector OldPosition;
	UE_LOG(LogSVR, Log, TEXT("ApplyLateUpdate-OldPose - RenderThread PoseTimestamp %lld ExpectedTime %f Frame %d", PoseState->Pose.poseTimeStampNs, PoseState->Pose.expectedDisplayTimeNs, PoseState->FrameNumber);
	PoseToOrientationAndPosition(PoseState->Pose.pose, OldOrientation, OldPosition, WorldToMetersScale);
	const FTransform OldRelativeTransform(OldOrientation, OldPosition);

	sxrHeadPoseState HeadPoseState;
	GetHeadPoseState(&HeadPoseState);

	UE_LOG(LogSVR, Log, TEXT("ApplyLateUpdate-NewPose - RenderThread PoseTimestamp %lld ExpectedTime %f Frame %d", PoseState->Pose.poseTimeStampNs, PoseState->Pose.expectedDisplayTimeNs, PoseState->FrameNumber);
	const sxrHeadPose Pose = HeadPoseState.pose;

	PoseState->Pose = HeadPoseState;

	FQuat NewOrientation;
	FVector NewPosition;
	PoseToOrientationAndPosition(Pose, NewOrientation, NewPosition, WorldToMetersScale);
	const FTransform NewRelativeTransform(NewOrientation, NewPosition);

	ApplyLateUpdate(InViewFamily.Scene, OldRelativeTransform, NewRelativeTransform);
*/
}
void FSnapdragonVRHMD::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	/**
	* Allows to render content after the 3D content scene, useful for debugging
	*/

	// ExecuteOnRHIThread([this]()
	// {
	// 	PostRenderFrame_RHIThread();
	// });
	
}
bool FSnapdragonVRHMD::IsActiveThisFrame(FViewport* InViewport) const
{
	return GEngine && GEngine->IsStereoscopic3D(InViewport);
}
#if ENGINE_MINOR_VERSION > 25
bool FSnapdragonVRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
#else
bool FSnapdragonVRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples /*= 1*/)
#endif
{
	UE_LOG(LogSVR,Log,TEXT("Use SwapChain"));
	TArray<uint32_t>TextureResources;
	TextureResources.SetNum(TextureQueueLength);		
	for (int i = 0; i < TextureQueueLength; ++i)
	{
		GLuint TextureID = 0;
		glGenTextures(1, &TextureID);
		GLenum textureType = bIsMobileMultiViewEnabled ? GL_TEXTURE_2D_ARRAY : GL_TEXTURE_2D;			
		glBindTexture(textureType, TextureID);			
		if (bIsMobileMultiViewEnabled) {
			glTexStorage3D(textureType, 1, GL_RGBA8, SizeX, SizeY, 2);
		}
		else {
			glTexStorage2D(textureType, 1, GL_RGBA8, SizeX, SizeY);
		}
		glTexParameteri(textureType, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(textureType, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
        glTexParameteri(textureType, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// GLfloat color[4]={0,0,0,1};
        // glTexParameterfv(textureType,GL_TEXTURE_BORDER_COLOR,color);
		glBindTexture(textureType, 0);
		TextureResources[i] = TextureID;
		UE_LOG(LogSVR,Log,TEXT("TextureResources[%d]:%d,siez(%d,%d)"),i,TextureResources[i], SizeX, SizeY);
	}	
	RenderBridge->CreateSwapChain(EPixelFormat(PF_R8G8B8A8),SizeX,SizeY,bIsMobileMultiViewEnabled?2:1,NumMips,NumSamples,Flags,TargetableTextureFlags,MobileMSAAValue,TextureResources);
	OutTargetableTexture = OutShaderResourceTexture =RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
	return true;	
}
void FSnapdragonVRHMD::StartRendering_RHIThread()
{
	if(bStartRendering)
		return;

	bStartRendering = true;
	uint32_t TrackingMode = kGSXRTrackingRotation | kGSXRTrackingPosition;
    SC::GSXR_nativeSetSlamType(TrackingMode);
    GSXRBeginParams beginParams;
    beginParams.cpuPerfLevel = GSXRPerfLevel::kGSXRPerfMaximum;
    beginParams.gpuPerfLevel = GSXRPerfLevel::kGSXRPerfMaximum;
    beginParams.nativeWindow = AndroidEGL::GetInstance()->GetNativeWindow();
    beginParams.mainThreadId = gettid();
    beginParams.colorSpace = kGSXRColorSpaceLinear;
	UE_LOG(LogSVR,Log,TEXT("xmh GSXR_nativeStartSlam"));
    GSXRResult result = SC::GSXR_nativeStartSlam(&beginParams);
	mFrameIndex = 0;
	UE_LOG(LogSVR,Log,TEXT("GSXR_nativeStartSlam:%d"),result);
}

void FSnapdragonVRHMD::StopRendering_RHIThread()
{
	if(!bStartRendering)
	{
		return;
	}
	UE_LOG(LogSVR,Log,TEXT("xmh StopRendering_RHIThread"));
	bStartRendering = false;
	SC::GSXR_nativeStopSlam();
	// SC::GSXR_nativeShutdown();
	// bInitialized = false;
}
void FSnapdragonVRHMD::BeginFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}
	RenderBridge->SwapChain->IncrementSwapChainIndex_RHIThread(0);
}

void FSnapdragonVRHMD::PreRenderFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}
	UpdateSensorData();
}
void FSnapdragonVRHMD::PostRenderFrame_RHIThread()
{

}
void CreateLayout(float centerX, float centerY, float radiusX, float radiusY, GSXRLayoutCoords *pLayout)
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

void FSnapdragonVRHMD::EndFrame_RHIThread()
{
	if(!bStartRendering)
		return;
	GSXRFrameParams FrameParams;
	memset(&FrameParams, 0, sizeof(FrameParams));

	FrameParams.frameIndex = mFrameIndex;//GFrameNumber
	if (bIsMobileMultiViewEnabled){
		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
		FrameParams.renderLayers[0].imageType = kGSXRTypeTextureArray;
		FrameParams.renderLayers[0].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].eyeMask = kGSXREyeMaskLeft;
		FrameParams.renderLayers[0].layerFlags |= kGSXRLayerFlagOpaque;

		// Right Eye
		FrameParams.renderLayers[1].imageType = kGSXRTypeTextureArray;
		FrameParams.renderLayers[1].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].eyeMask = kGSXREyeMaskRight;
		FrameParams.renderLayers[1].layerFlags |= kGSXRLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - rendering texture ID %d - from a texture array"), FrameParams.renderLayers[0].imageHandle);
	}else{
		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2D();
		FrameParams.renderLayers[0].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		FrameParams.renderLayers[0].imageType = kGSXRTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].imageCoords.LowerUVs[2] = 0.5f;
		FrameParams.renderLayers[0].imageCoords.UpperUVs[2] = 0.5f;
		FrameParams.renderLayers[0].eyeMask = kGSXREyeMaskLeft;
		// FrameParams.renderLayers[0].layerFlags |= kGSXRLayerFlagOpaque;

		FrameParams.renderLayers[1].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		FrameParams.renderLayers[1].imageType = kGSXRTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].imageCoords.LowerUVs[0] = 0.5f;
		FrameParams.renderLayers[1].imageCoords.UpperUVs[0] = 0.5f;
		FrameParams.renderLayers[1].eyeMask = kGSXREyeMaskRight;
		// FrameParams.renderLayers[1].layerFlags |= kGSXRLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - not using array"));
	}
	FrameParams.frameOptions = kGSXRDisableChromaticCorrection;
	FrameParams.headPoseState = CurrentFrame_RenderThread.headPoseState;
	UE_LOG(LogSVR,Log,TEXT("headPoseState:(%f,%f,%f),(%f,%f,%f,%f)"),FrameParams.headPoseState.pose.position.x,FrameParams.headPoseState.pose.position.y,FrameParams.headPoseState.pose.position.z,
	FrameParams.headPoseState.pose.rotation.x,FrameParams.headPoseState.pose.rotation.y,FrameParams.headPoseState.pose.rotation.z,FrameParams.headPoseState.pose.rotation.w);
	FrameParams.minVsyncs = 1;
	UE_LOG(LogSVR,Log,TEXT("(%s) (Frame %d) SubmitFrame(mRenderPose) => Calling SC::GSXR_nativeSubmitDataFrame(Frame %d),imageHandle=%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), FrameParams.frameIndex, FrameParams.frameIndex,FrameParams.renderLayers[0].imageHandle);
	SC::GSXR_nativeSubmitDataFrame(&FrameParams);
	RenderBridge->SwapChain->ReleaseCurrentImage_RHIThread();
	mFrameIndex++;
}
//-----------------------------------------------------------------------------
// FSnapdragonVRHMD Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FSnapdragonVRHMD Implementation
//-----------------------------------------------------------------------------
FSnapdragonVRHMD::FSnapdragonVRHMD(const FAutoRegister& AutoRegister) :
	FHeadMountedDisplayBase(nullptr),
	FSceneViewExtensionBase(AutoRegister),
	bInitialized(false),
    bResumed(false),
    CurHmdOrientation(FQuat::Identity),
    LastHmdOrientation(FQuat::Identity),
    DeltaControlRotation(FRotator::ZeroRotator),
    DeltaControlOrientation(FQuat::Identity),
	BaseOrientation(FQuat::Identity),
    CurHmdPosition(FVector::ZeroVector),
    LastHmdPosition(FVector::ZeroVector),
	bIsMobileMultiViewEnabled(false),
	RenderTargetSize(FIntPoint(1600,1600)),
	CurrentLayerId(0),
	bStartRendering(false),
	MobileMSAAValue(1),
	RHIString(FString()),
	mFrameIndex(0)
{

	memset(&CachedEyePoseState, 0, sizeof(CachedEyePoseState));
	memset(&CachedHeadPoseState, 0, sizeof(CachedHeadPoseState));

	// New for VR in 4.19
	static const auto PixelDensityCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("vr.PixelDensity"));
	if (PixelDensityCVar)
	{
		PixelDensityCVar->Set(1.0f);
	}

	static const auto ScreenPercentageCVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.ScreenPercentage"));
	if ( ScreenPercentageCVar )
	{
		ScreenPercentageCVar->Set(100.0f);
	}

	if (Startup())
	{
		SetupOcclusionMeshes();
	}
}
int32 FSnapdragonVRHMD::GetXRSystemFlags() const
{
	return EXRSystemFlags::IsHeadMounted;
}

//-----------------------------------------------------------------------------
FSnapdragonVRHMD::~FSnapdragonVRHMD()
{
    UE_LOG(LogSVR, Log, TEXT("~FSnapdragonVRHMD()"));

}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::IsInitialized() const
{
	if (bInitialized)
	{
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::IsInitialized() is true"));
	}
	else
	{
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::IsInitialized() is false"));
	}
	return bInitialized;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::InitializeExternalResources()
{
	UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::InitializeExternalResources()"));

	CVarPerfLevelCpu.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&PerfLevelCpuOnChangeByCvar));
	CVarPerfLevelGpu.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&PerfLevelGpuOnChangeByCvar));

    extern struct android_app* GNativeAndroidApp;

    GSXRInitParams initParams;

    initParams.javaVm = GJavaVM;
    initParams.javaEnv = FAndroidApplication::GetJavaEnv();
    initParams.javaActivityObject = GNativeAndroidApp->activity->clazz;

//#if WITH_TELEMETRY
//    if (sxrInitializeOptArgs(&initParams, TM_API_PTR) != GSXR_ERROR_NONE)
//#else
    if (SC::GSXR_nativeInitSlam(&initParams) != GSXR_ERROR_NONE)
//#endif
    {
        UE_LOG(LogSVR, Error, TEXT("sxrInitialize failed"));
        return false;
    }
    else
    {
        UE_LOG(LogSVR, Log, TEXT("sxrInitialize succeeeded"));

		///StartWebHelper();
        uint32 TrackingMode = kGSXRTrackingRotation | kGSXRTrackingPosition;
		CVars::EyeTrackingEnabled = false;
        // if (CVars::EyeTrackingEnabled)
        // {
        //     if ((SC::GSXR_nativeGetSupportSlamMode() & kGSXRTrackingEye) == 0)
        //     {
		// 		UE_LOG(LogSVR, Log, TEXT("Eye tracking is not supported on this device"));
        //         CVars::EyeTrackingEnabled = false;
        //     }
        //     else
        //     {
        //         TrackingMode |= kGSXRTrackingEye;
        //     }
        // }
		UE_LOG(LogSVR,Log,TEXT("GSXR_nativeSetSlamType:%d"),TrackingMode);
		SC::GSXR_nativeSetSlamType(TrackingMode);
        return true;
    }
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRHMD::Startup()
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::Startup(); this=%x"), this);

    bInitialized = InitializeExternalResources();
    if (!bInitialized)
    {
		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::Startup() - Initialization FAILED"), this);
        return false;
    }

	// pSnapdragonVRBridge = FSnapdragonVRHMDCustomPresent::Create(this);
	const FString HardwareDetails = FHardwareInfo::GetHardwareDetailsString();
	const FString RHILookup = NAME_RHI.ToString() + TEXT("=");
	if (!FParse::Value(*HardwareDetails, *RHILookup, RHIString))
	{
		UE_LOG(LogSVR,Log,TEXT("Get RHI String failed!"));
		check(0);
	}

	if (RHIString == TEXT("OpenGL"))
	{
		RenderBridge = CreateRenderBridge_OpenGL(this);
	}
	else
	{
		UE_LOG(LogSVR,Log,TEXT("Do not support this type!"));
	}
	UE_LOG(LogSVR,Log,TEXT("RHIString: %s"),*RHIString);
    //Register life cycle delegates
    FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FSnapdragonVRHMD::ApplicationWillEnterBackgroundDelegate);
    FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FSnapdragonVRHMD::ApplicationWillDeactivateDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationWillEnterBackgroundDelegate(), but cover the possibility that only this delegate is called
    FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FSnapdragonVRHMD::ApplicationHasEnteredForegroundDelegate);
    FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FSnapdragonVRHMD::ApplicationHasReactivatedDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationHasEnteredForegroundDelegate(), but cover the possibility that only this delegate is called

    //don't bother with ApplicationWillTerminateDelegate() -- "There is no guarantee that this will ever be called on a mobile device, save state when ApplicationWillEnterBackgroundDelegate is called instead." -- https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Misc/FCoreDelegates/ApplicationWillTerminateDelegate/index.html --June 10, 2016
    //OnExit() and OnPreExit() are not documented as being called on mobile -- https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Misc/FCoreDelegates/OnExit/index.html and https://docs.unrealengine.com/latest/INT/API/Runtime/Core/Misc/FCoreDelegates/OnPreExit/index.html --June 10, 2016

	static const auto  CVarFrustumPositionLeft = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SXR.ViewFrustumLeft"));
	FString Value = CVarFrustumPositionLeft->GetString();
	UE_LOG(LogSVR, Log, TEXT("------------>>>>>>>>>>>>>>>>> ViewFrustumLeft %s"), *Value);
//	float f[7];
//	FString s(Value);
//	uint32 n = CVars::ParseIntoFloats(s, f, 7);
//	UE_LOG(LogSVR, Log, TEXT("------------>>>>>>>>>>>>>>>>> ViewFrustumLeft [%f %f %f] [%f %f %f %f]"), f[0], f[1], f[2], f[3], f[4], f[5], f[6]);
#if ENGINE_MINOR_VERSION < 25
	const auto CVarMobileMultiView = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
	const auto CVarMobileMultiViewDirect = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView.Direct"));
	bIsMobileMultiViewEnabled = (CVarMobileMultiView && CVarMobileMultiView->GetValueOnAnyThread() != 0);
	if (bIsMobileMultiViewEnabled == true) {
		GSupportsMobileMultiView = true;
	}
	bIsMobileMultiViewEnabled = GSupportsMobileMultiView && bIsMobileMultiViewEnabled && (CVarMobileMultiViewDirect && CVarMobileMultiViewDirect->GetValueOnAnyThread() != 0);
#else
	const auto CVarMobileMultiView = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
	static const auto CVarMobileHDR = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileHDR"));
	const bool bMobileHDR = CVarMobileHDR && CVarMobileHDR->GetValueOnAnyThread() == 1;
	bIsMobileMultiViewEnabled =!bMobileHDR && (CVarMobileMultiView && CVarMobileMultiView->GetValueOnAnyThread() != 0);
#endif
	UE_LOG(LogSVR,Log,TEXT("bIsMobileMultiViewEnabled = %d"),bIsMobileMultiViewEnabled);

	GSXRDeviceInfo DeviceInfo = SC::GSXR_nativeGetDeviceInfo();
	if (!bIsMobileMultiViewEnabled) {
		RenderTargetSize.X = DeviceInfo.targetEyeWidthPixels * 2;
		RenderTargetSize.Y = DeviceInfo.targetEyeHeightPixels;
	}
	else
	{
		RenderTargetSize.X = DeviceInfo.targetEyeWidthPixels;
		RenderTargetSize.Y = DeviceInfo.targetEyeHeightPixels;
	}
	// RenderTargetSize.X = DeviceInfo.targetEyeWidthPixels * 2;
	// RenderTargetSize.Y = DeviceInfo.targetEyeHeightPixels;
// CTORNE ->
	// CTORNE: This is temporary so we can experiment with resolution vs foveation settings in SpaceDock[SG].
	//		   Handling of VR resolution was changed in 4.18 so this will have to change too. Changing the
	//		   scale factor at runtime is untested. 
	IConsoleVariable* CVarMobileContentScaleFactor = IConsoleManager::Get().FindConsoleVariable(TEXT("r.MobileContentScaleFactor"));
	if (CVarMobileContentScaleFactor && !FMath::IsNearlyEqual(CVarMobileContentScaleFactor->GetFloat(), 1.f))
	{
		RenderTargetSize.X = static_cast<uint32>(FMath::CeilToInt(CVarMobileContentScaleFactor->GetFloat() * RenderTargetSize.X));
		RenderTargetSize.Y = static_cast<uint32>(FMath::CeilToInt(CVarMobileContentScaleFactor->GetFloat() * RenderTargetSize.Y));

		UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::Startup(): r.MobileContentScaleFactor = %f -> eye buffer size is now = %u x %u"),
			CVarMobileContentScaleFactor->GetFloat(), RenderTargetSize.X, RenderTargetSize.Y);
	}
// <- CTORNE

	const int32 RTSizeX = RenderTargetSize.X;
	const int32 RTSizeY = RenderTargetSize.Y;
	EyeRenderViewport[0] = FIntRect(0, 0, RTSizeX / 2 - 1, RTSizeY - 1);
	EyeRenderViewport[1] = FIntRect(RTSizeX / 2 + 1, 1, RTSizeX - 1, RTSizeY - 1);

	// disable vsync
	static IConsoleVariable* CVSyncVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.VSync"));
	CVSyncVar->Set(false);

	// enforce finishcurrentframe
	static IConsoleVariable* CFCFVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.FinishCurrentFrame"));
	CFCFVar->Set(false);

	// eye tracking
	static IConsoleVariable* CETEVar = IConsoleManager::Get().FindConsoleVariable(TEXT("r.SVR.EyeTrackingEnabled"));
	CETEVar->Set(false);

	MobileMSAAValue = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileMSAA"))->GetValueOnAnyThread();
	UE_LOG(LogSVR, Log, TEXT("MSAA = %d"), MobileMSAAValue);
	return true;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ApplicationWillEnterBackgroundDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ApplicationWillEnterBackgroundDelegate"));
    CleanupIfNecessary();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::CleanupIfNecessary()
{
	if(!bResumed)
	{
		return;
	}
    bResumed = false;

    // if (pSnapdragonVRBridge->bInVRMode)
    // {
    //     UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::CleanupIfNecessary(): sxrEndXr() sxrShutdown()"));

    //     //stop consuming resources for VR until the app is resumed
	// 	SC::GSXR_nativeStopSlam();
    //     SC::GSXR_nativeShutdown();
    //     pSnapdragonVRBridge->bInVRMode = false;
    //     bInitialized = false;
    // }
	///StopWebHelper();

	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			StopRendering_RHIThread();
		});
	});
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ApplicationWillDeactivateDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ApplicationWillDeactivateDelegate"));
    CleanupIfNecessary();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ApplicationHasReactivatedDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ApplicationHasReactivatedDelegate"));
    InitializeIfNecessaryOnResume();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::ApplicationHasEnteredForegroundDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::ApplicationHasEnteredForegroundDelegate"));
    InitializeIfNecessaryOnResume();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::InitializeIfNecessaryOnResume()
{
    if (!bInitialized)//Upon application launch, FSnapdragonVRHMD::Startup() must initialize immediately, but Android lifecycle "resume" delegates will also be called -- don't double-initialize
    {
        bInitialized = InitializeExternalResources();
    }
    bResumed = true;
	ExecuteOnRenderThread([this]()
	{
		ExecuteOnRHIThread([this]()
		{
			StartRendering_RHIThread();
		});
	});
	FlushRenderingCommands();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetCPUAndGPULevels(const int32 InCPULevel, const int32 InGPULevel) const
{
	const GSXRPerfLevel SvrPerfLevelCPU = static_cast<const GSXRPerfLevel>(InCPULevel);
	const GSXRPerfLevel SvrPerfLevelGPU = static_cast<const GSXRPerfLevel>(InGPULevel);
	check(IsPerfLevelValid(SvrPerfLevelCPU));
	check(IsPerfLevelValid(SvrPerfLevelGPU));

	//sxr performance levels can be manipulated by render or game thread, so prevent race conditions
	// FScopeLock ScopeLock(&pSnapdragonVRBridge->PerfLevelCriticalSection);
	// PerfLevelCpuLastSet = SvrPerfLevelCPU;
	// PerfLevelGpuLastSet = SvrPerfLevelGPU;
// CTORNE ->
	// if (pSnapdragonVRBridge->bInVRMode)
	// {
	// 	SC::GSXR_nativeSetPerformanceLevels(PerfLevelCpuLastSet, PerfLevelGpuLastSet);
	// 	FSnapdragonVRHMD::PerfLevelLog(TEXT("SetCPUAndGPULevels"), PerfLevelCpuLastSet, PerfLevelGpuLastSet);
	// }
// <- CTORNE
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::PoseToOrientationAndPosition(const GSXRHeadPose& Pose, FQuat& OutCurrentOrientation, FVector& OutCurrentPosition, const float WorldToMetersScale)
{
	// UE_LOG(LogSVR, Log, TEXT("FSnapdragonVRHMD::PoseToOrientationAndPosition()"));

    if (bStartRendering)
    {
		OutCurrentOrientation = FQuat(-Pose.rotation.z, Pose.rotation.x, Pose.rotation.y, Pose.rotation.w);
		// OutCurrentOrientation = FQuat(0,0,0,1);
		// OutCurrentOrientation = BaseOrientation.Inverse() * OutCurrentOrientation;
		// OutCurrentOrientation.Normalize();	
		OutCurrentPosition = FVector(Pose.position.z * WorldToMetersScale, -Pose.position.x * WorldToMetersScale, -Pose.position.y * WorldToMetersScale);
		// OutCurrentPosition = FVector(0,0,0);
		// OutCurrentPosition = BaseOrientation.Inverse().RotateVector(OutCurrentPosition);
    }
    else
    {
        OutCurrentOrientation = FQuat(FRotator(0.0f, 0.0f, 0.0f));
        OutCurrentPosition = FVector(0.0f, 0.0f, 0.0f);
    }
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::BeginVRMode()
{
    // check(pSnapdragonVRBridge);
	//FAndroidMisc::UnlockAndroidWindow();
    // if (IsInRenderingThread())
    // {
    //     pSnapdragonVRBridge->DoBeginVR();
    // }
    // else
    // {
	// 	FSnapdragonVRHMDCustomPresent* m_SnapdragonVRBridge = pSnapdragonVRBridge;

	// 	ENQUEUE_RENDER_COMMAND(BeginVRMode)
	// 	(
	// 		[m_SnapdragonVRBridge](FRHICommandListImmediate& RHICmdList)
	// 		{
	// 			m_SnapdragonVRBridge->DoBeginVR();
	// 		}
	// 	);

    //     FlushRenderingCommands();
    // }
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::EndVRMode()
{
}

//-----------------------------------------------------------------------------
// uint32 FSnapdragonVRHMD::GetNumberOfBufferedFrames() const
// {
//     return 1;
// }

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SetupOcclusionMeshes()
{
	int TriangleCount = 0;
	int VertexStride = 0;
	if (SC::GSXR_nativeGetOcclusionMesh(kGSXRLeftEye, &TriangleCount, &VertexStride, nullptr) != GSXR_ERROR_NONE)
	{
		return;
	}

	const uint32 VertexCount = TriangleCount * 3;

	if (VertexCount > 0 && VertexStride > 0)
	{
		const uint32 VertexDataSize = VertexCount * VertexStride;

		float* const LeftEyeMesh = new float[VertexDataSize];
		float* const RightEyeMesh = new float[VertexDataSize];
		SC::GSXR_nativeGetOcclusionMesh(kGSXRLeftEye, &TriangleCount, &VertexStride, LeftEyeMesh);
		SC::GSXR_nativeGetOcclusionMesh(kGSXRRightEye, &TriangleCount, &VertexStride, RightEyeMesh);

		FVector2D* const LeftEyePositions = new FVector2D[VertexCount];
		FVector2D* const RightEyePositions = new FVector2D[VertexCount];

		uint32 SourceIndex = 0;
		uint32 DestIndex = 0;
		for (uint32 TriangleIter = 0; TriangleIter < TriangleCount; ++TriangleIter)
		{
			for (uint32 VertexIter = 0; VertexIter < 3; ++VertexIter)
			{
				const float* LeftSrc = &LeftEyeMesh[SourceIndex];
				const float* RightSrc = &RightEyeMesh[SourceIndex];

				FVector2D& LeftDst = LeftEyePositions[DestIndex];
				FVector2D& RightDst = RightEyePositions[DestIndex];

				LeftDst.X = (LeftSrc[0] + 1.f) * 0.5f;
				LeftDst.Y = (LeftSrc[1] + 1.f) * 0.5f;

				RightDst.X = (RightSrc[0] + 1.f) * 0.5f;
				RightDst.Y = (RightSrc[1] + 1.f) * 0.5f;

				SourceIndex += VertexStride;
				DestIndex++;
			}
		}

		HiddenAreaMeshes[0].BuildMesh(LeftEyePositions, VertexCount, FHMDViewMesh::MT_HiddenArea);
		HiddenAreaMeshes[1].BuildMesh(RightEyePositions, VertexCount, FHMDViewMesh::MT_HiddenArea);

		delete[] LeftEyePositions;
		delete[] RightEyePositions;

		delete[] LeftEyeMesh;
		delete[] RightEyeMesh;
	}
}

//-----------------------------------------------------------------------------
GSXRDeviceInfo FSnapdragonVRHMD::GetDeviceInfo()
{
	GSXRDeviceInfo di = SC::GSXR_nativeGetDeviceInfo();
	
	// if any coordinate system transformations are necessary, they should go here

	return di;
}


uint32 FSnapdragonVRHMD::CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	const uint32 LayerId = CurrentLayerId++;
	UE_LOG(LogSVR,Log,TEXT(" CreateLayer %d"),LayerId);
	LayerMap.Add(LayerId, MakeShareable(new FGSXRStereoLayer(LayerId, InLayerDesc)));
	return LayerId;
}

void FSnapdragonVRHMD::DestroyLayer(uint32 LayerId)
{
	check(IsInGameThread());
	UE_LOG(LogSVR,Log,TEXT(" DestroyLayer %d"),LayerId);
	LayerMap.Remove(LayerId);
}

void FSnapdragonVRHMD::SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	FGSXRLayerPtr* LayerFound = LayerMap.Find(LayerId);

	if (LayerFound)
	{
		LayerFound->Get()->SetLayerDesc(InLayerDesc);
	}
}

bool FSnapdragonVRHMD::GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc)
{
	check(IsInGameThread());
	FGSXRLayerPtr* LayerFound = LayerMap.Find(LayerId);
	if (LayerFound)
	{
		OutLayerDesc = (*LayerFound)->GetLayerDesc();
		return true;
	}
	return false;
}
void FSnapdragonVRHMD::MarkTextureForUpdate(uint32 LayerId)
{
	check(IsInGameThread());
}

void FSnapdragonVRHMD::UpdateSplashScreen()
{
}

IStereoLayers::FLayerDesc FSnapdragonVRHMD::GetDebugCanvasLayerDesc(FTextureRHIRef Texture)
{
	IStereoLayers::FLayerDesc StereoLayerDesc;
	StereoLayerDesc.Priority = INT_MAX;
	StereoLayerDesc.Transform = FTransform(FVector(180.f, 0, 0));
	StereoLayerDesc.Transform.SetScale3D(FVector(1,1,-1));
	StereoLayerDesc.QuadSize = FVector2D(180.f, 180.f);
	StereoLayerDesc.LayerSize = Texture->GetTexture2D()->GetSizeXY();
	StereoLayerDesc.PositionType = IStereoLayers::ELayerType::FaceLocked;
#if ENGINE_MINOR_VERSION >24
	StereoLayerDesc.SetShape<FQuadLayer>();
#else
	StereoLayerDesc.ShapeType = IStereoLayers::ELayerShape::QuadLayer;
#endif
	StereoLayerDesc.Texture = Texture;
	StereoLayerDesc.Flags = IStereoLayers::ELayerFlags::LAYER_FLAG_TEX_CONTINUOUS_UPDATE;
	StereoLayerDesc.Flags |= IStereoLayers::ELayerFlags::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO;
	return StereoLayerDesc;
}
void FSnapdragonVRHMD::GetAllocatedTexture(uint32 LayerId, FTextureRHIRef& Texture, FTextureRHIRef& LeftTexture)
{
	Texture = LeftTexture = nullptr;
}

FXRRenderBridge* FSnapdragonVRHMD::GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget)
{
	return RenderBridge;
}


#if 0 // webhelper stuff
//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::StartWebHelper()
{
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	extern struct android_app* GNativeAndroidApp;
	jstring StringArg1 = Env->NewStringUTF("SpaceDock");
	jstring StringArg2 = Env->NewStringUTF("score");
	jstring StringArg3 = Env->NewStringUTF("0");

	jclass FoundClass = FAndroidApplication::FindJavaClass("com/qualcomm/acg/webhelper/WebServiceStarter");
	FAndroidApplication::CheckJavaException();
	//UE_LOG(LogSVR, Log, TEXT("StartWebHelper after FoundClass  %x "), FoundClass);
	webHelperClass = FoundClass;

	jmethodID methodid = Env->GetStaticMethodID(FoundClass, "start", "(Ljava/lang/String;Landroid/app/Activity;)V");
	//UE_LOG(LogSVR, Log, TEXT("StartWebHelper after GetStaticMethodID : start  %x "), methodid);

	Env->CallStaticVoidMethod(FoundClass, methodid, StringArg1, GNativeAndroidApp->activity->clazz);
	//UE_LOG(LogSVR, Log, TEXT("StartWebHelper after CallStaticVoidMethod :start"));

	webHelperSendScore = Env->GetStaticMethodID(FoundClass, "sendMessage", "(Ljava/lang/String;Ljava/lang/String;)V");
	UE_LOG(LogSVR, Log, TEXT("StartScore start=%x sendMessage=%x "), webHelperClass, webHelperSendScore);

	//Env->CallStaticVoidMethod(FoundClass, webHelperSendScore, StringArg2, StringArg3);
	//UE_LOG(LogSVR, Log, TEXT("StartWebHelper after CallStaticVoidMethod sendMessage"));

	Env->DeleteLocalRef(StringArg1);
	Env->DeleteLocalRef(StringArg2);
	Env->DeleteLocalRef(StringArg3);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::SendMessage(const int32 score) const
{
	char buf[64];
	sprintf(buf, "%d", score);
	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jstring StringArg1 = Env->NewStringUTF(buf);
	jstring StringArg2 = Env->NewStringUTF("score");

	Env->CallStaticVoidMethod(webHelperClass, webHelperSendScore, StringArg2, StringArg1);
	UE_LOG(LogSVR, Log, TEXT("Score: After SendMessage %d"), score);

	Env->DeleteLocalRef(StringArg1);
	Env->DeleteLocalRef(StringArg2);
}

//-----------------------------------------------------------------------------
void FSnapdragonVRHMD::StopWebHelper()
{

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jmethodID methodid = Env->GetStaticMethodID(webHelperClass, "stop", "()V");
	//UE_LOG(LogSVR, Log, TEXT("StopWebHelper after GetStaticMethodID : stop  %x "), methodid);

	Env->CallStaticVoidMethod(webHelperClass, methodid);
	UE_LOG(LogSVR, Log, TEXT("StopScore after :stop"));

}

#endif // webhelper

#endif //SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
