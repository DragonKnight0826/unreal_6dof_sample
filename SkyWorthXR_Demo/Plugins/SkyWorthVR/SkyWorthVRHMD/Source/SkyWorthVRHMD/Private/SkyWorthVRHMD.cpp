//=============================================================================
// FILE: SkyWorthVRHMD.cpp
//
//                  Copyright (c) 2016 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#include "SkyWorthVRHMD.h"

#include "HardwareInfo.h"
#include "../Public/SkyWorthVRHMDFunctionLibrary.h"

#include "../Classes/SkyWorthVRHMDEventManager.h"

#include "SkyWorthXR_CVars.h"

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
#include "SXRLoader.h"
#include "sxrApi.h"
//#include "SkyWorthVROther/Public/MotionUtilsImple.h"

//#define GL_FOVEATION_ENABLE_BIT_QCOM 0x01
//#define GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM 0x02
#define GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM 0x8BFB
#define GL_TEXTURE_FOVEATED_FEATURE_QUERY_QCOM 0x8BFD
#define GL_FRAMEBUFFER_INCOMPLETE_FOVEATION_QCOM 0x8BFF
#define GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM 0x8BFC
#define GL_TEXTURE_FOVEATED_NUM_FOCAL_POINTS_QUERY_QCOM 0x8BFE
#define GL_CHECK(x) \
    x; \
    { \
        GLenum glError = glGetError(); \
        if(glError != GL_NO_ERROR) { \
            XR_FAIL("glGetError() = %i (0x%.8x) at %s:%i\n", glError, glError, __FILE__, __LINE__); \
        } \
    }

bool InGameThread()
{
	if (GIsGameThreadIdInitialized)
	{
		return FPlatformTLS::GetCurrentThreadId() == GGameThreadId;
	}
	return true;
}

bool InRenderThread()
{
	if (GIsThreadedRendering && !GIsRenderingThreadSuspended.Load(EMemoryOrder::Relaxed))
	{
		return IsInActualRenderingThread();
	}
	return InGameThread();
}

DECLARE_CYCLE_STAT(TEXT("Update Player Camera"), STAT_UpdatePlayerCamera, STATGROUP_Tickables);

#if PLATFORM_ANDROID

extern "C" SxrResult GSXRInitializeOptArgs(const sxrInitParams* pInitParams, void* pTmAPI);

/*
It appears that we must ensure that sxrEndVr() (which is always called on the CPU thread) never overlaps with
sxrSubmitFrame() (which is always called on the render thread) by mutexing bInVRMode in those cases.  Otherwise, we can
get mysterious deadlocks during suspend/resume (seemingly only if the phone is not plugged into USB!  Plugged in USB
phones never reproduced this deadlock).

It is equally important that we do *not* try to use this thread synchronization construct to guard against other times
bInVRMode is used (for example, at startup), or else a different mysterious deadlock on app launch occurs.  @todo NTF:
understand what's really going on here

*/
//FCriticalSection FSkyWorthVRHMDCustomPresent::InVRModeCriticalSection;
//FCriticalSection FSkyWorthVRHMDCustomPresent::PerfLevelCriticalSection;

#include <android_native_app_glue.h>
#include "Android/AndroidEGL.h"
#include "Android/AndroidJNI.h"
#include "Android/AndroidApplication.h"
#endif // PLATFORM_ANDROID

#include "RHIStaticStates.h"
#include "Engine/Public/Slate/SceneViewport.h"

#include "DrawDebugHelpers.h"

#include "sxrApi.h"
DEFINE_LOG_CATEGORY(LogSVR);

#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS

typedef void (GL_APIENTRYP PFNGLTEXTUREFOVEATIONPARAMETERSEXT)(GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea);

PFNGLTEXTUREFOVEATIONPARAMETERSEXT	glTextureFoveationParametersQCOM = NULL;

static const TCHAR* const GSXRPerfLevelSystemCStr = TEXT("sys");
static const TCHAR* const GSXRPerfLevelMinimumCStr = TEXT("min");
static const TCHAR* const GSXRPerfLevelMediumCStr = TEXT("med");
static const TCHAR* const GSXRPerfLevelMaximumCStr = TEXT("max");

//keep these two lines in sync (eg pass the string that corresponds to the enum)
static const TCHAR* const GSXRPerfLevelDefaultCStr = GSXRPerfLevelSystemCStr;
/*static*/ enum sxrPerfLevel FSkyWorthVRHMD::GetCVarSkyWorthVrPerfLevelDefault() { return kPerfMedium; }

//these variables default values can be overridden by adding a line like this to Engine\Config\ConsoleVariables.ini: sxr.perfCpu=max
static TAutoConsoleVariable<FString> CVarPerfLevelCpu(TEXT("sxr.perfCpu"), GSXRPerfLevelDefaultCStr, TEXT("Set SkyWorthVr CPU performance consumption to one of the following: sys, min, med, max.  Note that if this value is set by Blueprint, the cvar will continue to report the last value it was set to, and will not reflect the value set by Blueprint"));
static TAutoConsoleVariable<FString> CVarPerfLevelGpu(TEXT("sxr.perfGpu"), GSXRPerfLevelDefaultCStr, TEXT("Set SkyWorthVr GPU performance consumption to one of the following: sys, min, med, max.  Note that if this value is set by Blueprint, the cvar will continue to report the last value it was set to, and will not reflect the value set by Blueprint"));

//BEG_Q3D_AUDIO_HACK
#if !UE_BUILD_SHIPPING
static TAutoConsoleVariable<int32> CVarQ3D_DrawSound(TEXT("q3d.drawSound"), 0, TEXT("Draws red tetrahedrons centered at each spatialized sound's position (hurts performance significantly)"));
static TAutoConsoleVariable<int32> CVarQ3D_DrawSoundName(TEXT("q3d.drawSoundName"), 0, TEXT("Draws red textual names of spatialized sounds near their positions (hurts performance dramatically)"));
static TAutoConsoleVariable<int32> CVarQ3D_LogSoundName(TEXT("q3d.logSoundName"), 0, TEXT("Logs sound names and positions of spatialized sounds each frame"));
static TAutoConsoleVariable<int32> CVarQ3D_LogSoundNumber(TEXT("q3d.logSoundNumber"), 0, TEXT("Logs the number of spatialized sounds playing on a frame"));
#endif//#if !UE_BUILD_SHIPPING
//END_Q3D_AUDIO_HACK


//-----------------------------------------------------------------------------

void initGLExt() {
	glTextureFoveationParametersQCOM = (PFNGLTEXTUREFOVEATIONPARAMETERSEXT)eglGetProcAddress(
		"glTextureFoveationParametersQCOM");
}

static bool FStringToSvrPerfLevel(enum sxrPerfLevel* OutPerfLevel, const FString& InValueReceived)
{
    if (InValueReceived.Compare(FString(GSXRPerfLevelSystemCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kPerfSystem;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMinimumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kPerfMinimum;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMediumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kPerfMedium;
        return true;
    }
    else if (InValueReceived.Compare(FString(GSXRPerfLevelMaximumCStr, ESearchCase::IgnoreCase)) == 0)
    {
        *OutPerfLevel = kPerfMaximum;
        return true;
    }
    else
    {
        return false;
    }
}

//-----------------------------------------------------------------------------
static bool SvrPerfLevelToFString(FString* const OutPerfLevelFString, const enum sxrPerfLevel InPerfLevel)
{
    switch (InPerfLevel)
    {
    case kPerfSystem:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelSystemCStr);
        return true;
    }
    case kPerfMinimum:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelMinimumCStr);
        return true;
    }
    case kPerfMedium:
    {
        *OutPerfLevelFString = FString(GSXRPerfLevelMediumCStr);
        return true;
    }
    case kPerfMaximum:
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
static bool IsPerfLevelValid(const enum sxrPerfLevel InPerfLevel)
{
    return InPerfLevel >= kPerfSystem && InPerfLevel <= kPerfMaximum;
}

// CTORNE ->
/*static*/ enum sxrPerfLevel FSkyWorthVRHMD::PerfLevelCpuLastSet = kNumPerfLevels, FSkyWorthVRHMD::PerfLevelGpuLastSet = kNumPerfLevels;
// <- CTORNE

//-----------------------------------------------------------------------------
/*static*/ void PerfLevelLastSet(enum sxrPerfLevel* const OutPerfLevel, const enum sxrPerfLevel InPerfLevel)
{
	check(IsPerfLevelValid(InPerfLevel));
	*OutPerfLevel = InPerfLevel;
}

//-----------------------------------------------------------------------------
/*static*/ void FSkyWorthVRHMD::PerfLevelCpuWrite(const enum sxrPerfLevel InPerfLevel)
{
	PerfLevelLastSet(&PerfLevelCpuLastSet, InPerfLevel);
}

//-----------------------------------------------------------------------------
/*static*/ void FSkyWorthVRHMD::PerfLevelGpuWrite(const enum sxrPerfLevel InPerfLevel)
{
	PerfLevelLastSet(&PerfLevelGpuLastSet, InPerfLevel);
}

//-----------------------------------------------------------------------------
/*static*/ void FSkyWorthVRHMD::PerfLevelLog(const TCHAR* const InPrefix,enum sxrPerfLevel InPerfLevelCpu, enum sxrPerfLevel InPerfLevelGpu)
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
	enum sxrPerfLevel* OutPerfLevel,
	const TAutoConsoleVariable<FString>& InCVar,
	const enum sxrPerfLevel InPerfLevelDefault)
{
	const bool bReadSucceeded = FStringToSvrPerfLevel(OutPerfLevel, InCVar.GetValueOnAnyThread());
	if (!bReadSucceeded)
	{
		*OutPerfLevel = InPerfLevelDefault;
	}

	return bReadSucceeded;
}

//-----------------------------------------------------------------------------
/*static*/ bool FSkyWorthVRHMD::PerfLevelsLastSetByCvarRead(
	enum sxrPerfLevel* OutPerfLevelCpuCurrent,
	enum sxrPerfLevel* OutPerfLevelGpuCurrent,
	const enum sxrPerfLevel InPerfLevelCpuDefault,
	const enum sxrPerfLevel InPerfLevelGpuDefault)
{
	bool bValid = PerfLevelLastSetByCvarRead(OutPerfLevelCpuCurrent, CVarPerfLevelCpu, InPerfLevelCpuDefault);
	bValid &= PerfLevelLastSetByCvarRead(OutPerfLevelGpuCurrent, CVarPerfLevelGpu, InPerfLevelGpuDefault);
	return bValid;
}

//-----------------------------------------------------------------------------
static void PerfLevelOnChangeByCvar(enum sxrPerfLevel* const OutPerfLevelToSet, const IConsoleVariable* const InConsoleVar, const TCHAR* const InLogPrefix)
{
	//sxr performance levels can be manipulated by render or game thread, so prevent race conditions
	//FScopeLock ScopeLock(&FSkyWorthVRHMDCustomPresent::PerfLevelCriticalSection);

	enum sxrPerfLevel PerfLevel;
	const bool bReadSucceeded = FStringToSvrPerfLevel(&PerfLevel, InConsoleVar->GetString());
	if (bReadSucceeded)
	{
		PerfLevelLastSet(OutPerfLevelToSet, PerfLevel);
		FSkyWorthVRHMD::PerfLevelLog(InLogPrefix, FSkyWorthVRHMD::PerfLevelCpuLastSet, FSkyWorthVRHMD::PerfLevelGpuLastSet);
		sxrSetPerformanceLevels(FSkyWorthVRHMD::PerfLevelCpuLastSet, FSkyWorthVRHMD::PerfLevelGpuLastSet);
	}
}

//-----------------------------------------------------------------------------
static void PerfLevelCpuOnChangeByCvar(IConsoleVariable* InVar)
{
	PerfLevelOnChangeByCvar(&FSkyWorthVRHMD::PerfLevelCpuLastSet, InVar, TEXT("sxr.perfCpu cvar"));
}

//-----------------------------------------------------------------------------
static void PerfLevelGpuOnChangeByCvar(IConsoleVariable* InVar)
{
	PerfLevelOnChangeByCvar(&FSkyWorthVRHMD::PerfLevelGpuLastSet, InVar, TEXT("sxr.perfGpu cvar"));
}
#endif//#if SkyWorthVR_SUPPORTED_PLATFORMS

const FName FSkyWorthVRHMD::SkyWorthVRHMDSystemName(TEXT("SkyWorthVRHMD"));


//-----------------------------------------------------------------------------
// FSkyWorthVRHMD, IHeadMountedDisplay Implementation
//-----------------------------------------------------------------------------

void FSkyWorthVRHMD::RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList,
                                                  class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture,
                                                  FVector2D WindowSize) const
{
	// was empty originally
	if (SpectatorScreenController)
	{
		SpectatorScreenController->RenderSpectatorScreen_RenderThread(RHICmdList, BackBuffer, SrcTexture, WindowSize);
	}
	// UE_LOG(LogSVR,Log,TEXT("RenderTexture_RenderThread Textureid:%d "),(*((uint32_t *)SrcTexture->GetNativeResource())));
}

#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS

//-----------------------------------------------------------------------------
// Grab a pointer to the SkyWorth XR system
FSkyWorthVRHMD* FSkyWorthVRHMD::GetSkyWorthHMD()
{
	if (GEngine->XRSystem.IsValid() && GEngine->XRSystem->GetSystemName() == FSkyWorthVRHMD::SkyWorthVRHMDSystemName)
	{
		return static_cast<FSkyWorthVRHMD*>(GEngine->XRSystem.Get());
	}

	return nullptr;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::IsHMDEnabled() const
{
    return true;
}
//-----------------------------------------------------------------------------
FString FSkyWorthVRHMD::GetVersionString() const
{
	FString s = FString::Printf(TEXT("SkyWorthVR - %s, built %s, %s"),
		*FEngineVersion::Current().ToString(), UTF8_TO_TCHAR(__DATE__), UTF8_TO_TCHAR(__TIME__));
	return s;
}
//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::HasValidTrackingPosition()
{
	// this actually tries to get one
	return DoesSupportPositionalTracking();
}

//-----------------------------------------------------------------------------
// void FSkyWorthVRHMD::SetFinalViewRect(const enum EStereoscopicPass StereoPass, const FIntRect& FinalViewRect)
// {
// //	CheckInRenderThread();

// 	//UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::SetFinalViewRect()"));

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
bool FSkyWorthVRHMD::EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type /*= EXRTrackedDeviceType::Any*/)
{
	if (Type == EXRTrackedDeviceType::Any || Type == EXRTrackedDeviceType::HeadMountedDisplay)
	{
		OutDevices.Add(IXRTrackingSystem::HMDDeviceId);
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::EnumerateTrackedDevices() - added device"));
		return true;
	}
	UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::EnumerateTrackedDevices() - no device added"));
	return false;
 }

//-----------------------------------------------------------------------------
void  FSkyWorthVRHMD::UpdatePoses()
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
void FSkyWorthVRHMD::UpdateMyPoses(sxrHeadPoseState& Pose)
{
	GetHeadPoseState(Pose);
}

//-----------------------------------------------------------------------------
// rbf validate
bool FSkyWorthVRHMD::GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;

	if (DeviceId != HMDDeviceId)
	{
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetCurrentPose() - Device error %d != %d"), DeviceId, HMDDeviceId);
		return false;
	}

	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetCurrentPose() from %s thread for frame:%d/%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, GFrameNumberRenderThread );

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

		RenderBridge->SwapChain->ReleaseCurrentImage_RHIThread();

		FlushRenderingCommands();
	}

	return true;
}

void FSkyWorthVRHMD::UpdateSensorData()
{
	float PredictedTime = sxrGetPredictedDisplayTime();

	
	// sxrprint("(%s) (Frame %d) GetHeadPoseState() => Getting head pose for %0.2f ms", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	
	sxrHeadPoseState HeadPoseState = sxrGetPredictedHeadPose(PredictedTime);

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
bool FSkyWorthVRHMD::isEyeTrackingEnabled()const
{
	return  0 != CVars::EyeTrackingEnabled;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::enableEyeTracking(bool b)
{
	CVars::EyeTrackingEnabled = b ? 1 : 0;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::GetEyePoseState(int32 DeviceId, sxrEyePoseState& EyePoseState)
{
	if (DeviceId != HMDDeviceId)
	{
		UE_LOG(LogSVR, Error, TEXT("FSkyWorthVRHMD::GetEyePoseState() - Device error %d != %d"), DeviceId, HMDDeviceId);
		return false;
	}

	/*if (!pSkyWorthVRBridge->bInVRMode)
	{
		UE_LOG(LogSVR, Error, TEXT("FSkyWorthVRHMD::GetEyePoseState() - Not in VRMode"));
		return false;
	}*/

	if ((sxrGetSupportedTrackingModes() & kTrackingEye) == 0)
	{
		UE_LOG(LogSVR, Error, TEXT("FSkyWorthVRHMD::GetEyePoseState() - EyeTracking not available"));
		return false;
	}

	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetEyePoseState() from %s thread for frame:%d/%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, GFrameNumberRenderThread);

	if (sxrGetEyePose(&EyePoseState) != SXR_ERROR_NONE)
	{	// if you get here...
		// sxrGetEyePose will write out the error into the log  - look for "sxrGetEyePose Failed" because the call to the service failed...
		UE_LOG(LogSVR, Error, TEXT("FSkyWorthVRHMD::GetEyePoseState - sxrGetEyePose Failed"));
		return false;
	}

	//UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetEyePoseState() says left position is %s and is [%6.3f,%6.3f,%6.3f]"),
	//	isEyeGazePointValid(EyePoseState.leftEyePoseStatus) ? TEXT("Valid") : TEXT("Invalid"), EyePoseState.leftEyeGazePoint[0], EyePoseState.leftEyeGazePoint[1], EyePoseState.leftEyeGazePoint[2]);
	//UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetEyePoseState() says left direction is %s and is [%6.3f,%6.3f,%6.3f]"),
	//	isEyeGazeDirectionValid(EyePoseState.leftEyePoseStatus) ? TEXT("Valid") : TEXT("Invalid"), EyePoseState.leftEyeGazeVector[0], EyePoseState.leftEyeGazeVector[1], EyePoseState.leftEyeGazeVector[2]);

	return true;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::GetRelativeEyeDirection(int32 DeviceId, FVector& OutDirection)
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
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetRelativeEyeDirection combinedEyeDirection = x:%f, y:%f, z:%f"), OutDirection.X, OutDirection.Y, OutDirection.Z);
		return true;
	}
	else if ( bGazeVectorValidLeft || bGazeVectorValidRight )	// Average left/rightEyeDirection
	{
		OutDirection = FVector::ZeroVector;
		if (bGazeVectorValidLeft) OutDirection += eyeDirections[0];
		if (bGazeVectorValidRight) OutDirection += eyeDirections[1];
		OutDirection.Normalize();
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::GetRelativeEyeDirection averagedEyeDirection = x:%f, y:%f, z:%f"), OutDirection.X, OutDirection.Y, OutDirection.Z);
		return true;

		//dbgprint("FSkyWorthVRHMD::GetRelativeEyeDirection EyeDirection = x:%f, y:%f, z:%f", OutDirection.X, OutDirection.Y, OutDirection.Z);
	}
	return false;
}

//-----------------------------------------------------------------------------
// We don't really follow the instructions for this call....
#if ENGINE_MAJOR_VERSION == 5
bool FSkyWorthVRHMD::GetRelativeEyePose(int32 DeviceId, int32 ViewIndex, FQuat& OutOrientation, FVector& OutPosition)
{
	OutOrientation = FQuat::Identity;
	OutPosition = FVector::ZeroVector;
	if (DeviceId == HMDDeviceId && (ViewIndex == eSSE_LEFT_EYE || ViewIndex == eSSE_RIGHT_EYE))
	{
		OutPosition = FVector(0, (ViewIndex == eSSE_LEFT_EYE ? -.5 : .5) * GetInterpupillaryDistance() * GetWorldToMetersScale(), 0);
		return true;
	}
	return false;
}
#else
bool FSkyWorthVRHMD::GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition)
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
#endif
//-----------------------------------------------------------------------------
const sxrEyePoseState& FSkyWorthVRHMD::GetLatestEyePoseState() // GetCurrentEyePose()
{
	return CachedEyePoseState;
}

#pragma endregion EyeTracking
//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::EnableHMD(bool allow)
{
	// EnableStereo(allow);
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::GetHMDMonitorInfo(MonitorInfo& MonitorDesc)
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
void FSkyWorthVRHMD::GetFieldOfView(float& OutHFOVInDegrees, float& OutVFOVInDegrees) const
{
	if(!bInitialized)
	{
		OutHFOVInDegrees = 90.0;
		OutVFOVInDegrees = 90.0;
		return;
	}
	sxrDeviceInfo di = sxrGetDeviceInfo();
	
    OutHFOVInDegrees = FMath::RadiansToDegrees(di.targetFovXRad);
    OutVFOVInDegrees = FMath::RadiansToDegrees(di.targetFovYRad);
	// UE_LOG(LogSVR,Log,TEXT("xmh fov:(%f,%f)"),OutHFOVInDegrees,OutVFOVInDegrees);
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::DoesSupportPositionalTracking() const
{
	if(!bInitialized)
	{
		return false;
	}
	sxrDeviceInfo di = sxrGetDeviceInfo();
    const unsigned int supportedTrackingModes = sxrGetSupportedTrackingModes();

    const bool bTrackingPosition = supportedTrackingModes & kTrackingPosition;
	UE_LOG(LogSVR,Log,TEXT("DoesSupportPositionalTracking:%d"),bTrackingPosition);
    return bTrackingPosition;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetInterpupillaryDistance(float NewInterpupillaryDistance)
{
	UE_LOG(LogSVR, Log, TEXT("SkyWorthVR - call to SetInterpupillaryDistance is nop"));
    //Do Nothing
}

float FSkyWorthVRHMD::GetInterpupillaryDistance() const
{
    return 0.064f;
}


// TSharedPtr<ISceneViewExtension, ESPMode::ThreadSafe> FSkyWorthVRHMD::GetViewExtension()
// {
//     TSharedPtr<FSkyWorthVRHMD, ESPMode::ThreadSafe> ptr(AsShared());
//     return StaticCastSharedPtr<ISceneViewExtension>(ptr);
// }


//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::IsChromaAbCorrectionEnabled() const
{
    return false;
}

//-----------------------------------------------------------------------------
// bool FSkyWorthVRHMD::IsHeadTrackingAllowed() const
// {
//     return true;
// }

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ResetOrientationAndPosition(float yaw)
{
    ResetOrientation(yaw);
    ResetPosition();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ResetOrientation(float yaw)
{
	UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ResetOrientation()"));
	sxrRecenterOrientation(true);
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ResetPosition()
{
	UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ResetPosition()"));
	sxrRecenterPosition();
}

//-----------------------------------------------------------------------------
float FSkyWorthVRHMD::GetWorldToMetersScale() const
{
	return GWorld ? GWorld->GetWorldSettings()->WorldToMeters : 100.0f;  // RBF validate
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetBaseRotation(const FRotator& BaseRot)
{
	// SetBaseOrientation(FRotator(0.0f, BaseRot.Yaw, 0.0f).Quaternion());
}

bool FSkyWorthVRHMD::HandleInputKey(UPlayerInput*, const FKey& Key, EInputEvent EventType, float AmountDepressed, bool bGamepad)
{
	UE_LOG(LogSVR, Log, TEXT("AndroidKey = %s"),*Key.ToString());
	return false;
}

//-----------------------------------------------------------------------------
FRotator FSkyWorthVRHMD::GetBaseRotation() const
{
	// return GetBaseOrientation().Rotator();
	return FRotator::ZeroRotator;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetBaseOrientation(const FQuat& BaseOrient)
{
	// BaseOrientation = BaseOrient;
}

//-----------------------------------------------------------------------------
FQuat FSkyWorthVRHMD::GetBaseOrientation() const
{
	// return BaseOrientation;
	return FQuat::Identity;
}

//-----------------------------------------------------------------------------
static void DrawOcclusionMesh(FRHICommandList& RHICmdList, int32 StereoPass, const FHMDViewMesh MeshAssets[])
{
	check(InRenderThread());
#if ENGINE_MAJOR_VERSION == 5
	check(StereoPass != eSSE_MONOSCOPIC);
	const uint32 MeshIndex = (StereoPass == eSSE_LEFT_EYE) ? 0 : 1;
#else
	check(StereoPass != eSSP_FULL);
	const uint32 MeshIndex = (StereoPass == eSSP_LEFT_EYE) ? 0 : 1;
#endif

	
	const FHMDViewMesh& Mesh = MeshAssets[MeshIndex];
	check(Mesh.IsValid());

	RHICmdList.SetStreamSource(0, Mesh.VertexBufferRHI, 0);
	RHICmdList.DrawIndexedPrimitive(Mesh.IndexBufferRHI, 0, 0, Mesh.NumVertices, 0, Mesh.NumTriangles, 1);
}
#if ENGINE_MAJOR_VERSION == 5
//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawHiddenAreaMesh(class FRHICommandList& RHICmdList, int32 ViewIndex) const
{
	//DrawOcclusionMesh(RHICmdList, ViewIndex, HiddenAreaMeshes);
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawVisibleAreaMesh(class FRHICommandList& RHICmdList, int32 ViewIndex) const
{
    //Do Nothing
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawDistortionMesh_RenderThread(struct FHeadMountedDisplayPassContext& Context, const FIntPoint& TextureSize)
{
    //Do Nothing
}
#else
void FSkyWorthVRHMD::DrawHiddenAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const
{
	DrawOcclusionMesh(RHICmdList, StereoPass, HiddenAreaMeshes);
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawVisibleAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const
{
	//Do Nothing
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize)
{
	//Do Nothing
}
#endif
//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::OnBeginPlay(FWorldContext& InWorldContext)
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::OnBeginPlay"));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::OnEndPlay(FWorldContext& InWorldContext)
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::OnEndPlay"));
	///StopWebHelper();
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::OnStartGameFrame( FWorldContext& WorldContext )
{
	UE_LOG(LogSVR,Log,TEXT("xmh OnStartGameFrame"));

	LastFrame_GameThread = CurrentFrame_GameThread;
	CurrentFrame_GameThread = LastFrame_RenderThread;
    return true;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::OnEndGameFrame( FWorldContext& WorldContext )
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
void FSkyWorthVRHMD::OnBeginRendering_GameThread()
{
	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::OnBeginRendering_GameThread()"));
}

//-----------------------------------------------------------------------------
/**
 * Called on the render thread at the start of rendering.
 */
void FSkyWorthVRHMD::OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily)
{
	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::OnBeginRendering_RenderThread()"));
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::GetHeadPoseState(sxrHeadPoseState& HeadPoseState)
{
	// Can only update the pose if vr has been started
	// if (!pSkyWorthVRBridge->bStartRendering)
	// {
	// 	return false;
	// }

	//! \brief Calculates a predicted head pose
	//! \param predictedTimeMs Time ahead of the current time in ms to predict a head pose for
	//! \return The predicted head pose and relevant pose state information
	float PredictedTime = sxrGetPredictedDisplayTime();

	// UE_LOG(LogSVR, Error, TEXT("sxr (%s) (Frame %llu) GetHeadPoseState() => Getting head pose for %0.2f ms"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	// sxrprint("(%s) (Frame %d) GetHeadPoseState() => Getting head pose for %0.2f ms", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, PredictedTime);
	
	HeadPoseState = sxrGetPredictedHeadPose(PredictedTime);

	FVector CurrentPosition;
	FQuat CurrentOrientation;
	PoseToOrientationAndPosition(HeadPoseState.pose, CurrentOrientation, CurrentPosition, GetWorldToMetersScale());
	CurHmdOrientation = LastHmdOrientation = BaseOrientation * CurrentOrientation;
	CurHmdPosition = LastHmdPosition = CurrentPosition;

	return true;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SendEvents()
{
	sxrEvent SxrEvent;
	while (sxrPollEvent(&SxrEvent))
	{
		switch (SxrEvent.eventType)
		{
		case sxrEventType::kEventSdkServiceStarting:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRSdkServiceDelegate.Broadcast(ESkyWorthVRSdkServiceEventType::EventSdkServiceStarting);
			break;
		case sxrEventType::kEventSdkServiceStarted:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRSdkServiceDelegate.Broadcast(ESkyWorthVRSdkServiceEventType::EventSdkServiceStarted);
			break;
		case sxrEventType::kEventSdkServiceStopped:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRSdkServiceDelegate.Broadcast(ESkyWorthVRSdkServiceEventType::EventSdkServiceStopped);
			break;
		case sxrEventType::kEventThermal:
		{
			ESkyWorthVRThermalZone ThermalZone = static_cast<ESkyWorthVRThermalZone>(SxrEvent.eventData.thermal.zone);
			ESkyWorthVRThermalLevel ThermalLevel = static_cast<ESkyWorthVRThermalLevel>(SxrEvent.eventData.thermal.level);

			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRThermalDelegate.Broadcast(ThermalZone, ThermalLevel);
			break;
		}
		case sxrEventType::kEventVrModeStarted:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRModeDelegate.Broadcast(ESkyWorthVRModeEventType::EventVrModeStarted);
			break;
		case sxrEventType::kEventVrModeStopping:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRModeDelegate.Broadcast(ESkyWorthVRModeEventType::EventVrModeStopping);
			break;
		case sxrEventType::kEventVrModeStopped:
			USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()->OnSkyWorthVRModeDelegate.Broadcast(ESkyWorthVRModeEventType::EventVrModeStopped);
			break;

		default:
			break;
		}
	}
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawDebug(class UCanvas* Canvas, class APlayerController*)
{
    Canvas->DrawText(GEngine->GetLargeFont(), FText::Format( FText::FromString("Debug Draw {0}"), FText::AsNumber(GFrameNumber)), 10.f, 10.f);

	DrawDebugTrackingCameraFrustum(GWorld, Canvas->SceneView->ViewRotation, Canvas->SceneView->ViewLocation);
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::DrawDebugTrackingCameraFrustum(UWorld* World, const FRotator& ViewRotation, const FVector& ViewLocation)
{
#if 0
	FPlatformMisc::LowLevelOutputDebugStringf(TEXT("FSkyWorthVRHMD::DrawDebugTrackingCameraFrustum"));

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
// FSkyWorthVRHMD, IStereoRendering Implementation
//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::IsStereoEnabled() const
{
    return true;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::EnableStereo(bool stereo)
{

	// GEngine->bForceDisableFrameRateSmoothing = stereo;
    return true;
}

#if ENGINE_MAJOR_VERSION == 5
void FSkyWorthVRHMD::AdjustViewRect(int32 ViewIndex, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
#else
void FSkyWorthVRHMD::AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const
#endif
{
	if(!bIsMobileMultiViewEnabled)
	{
		SizeX = GetIdealRenderTargetSize().X / 2;
	}else
	{
		SizeX = GetIdealRenderTargetSize().X;
	}

	SizeY = GetIdealRenderTargetSize().Y;

#if ENGINE_MAJOR_VERSION == 5
	if (ViewIndex == eSSE_RIGHT_EYE)
#else
	if (StereoPass == eSSP_RIGHT_EYE)
#endif
	{
		X += SizeX;
	}
	if (bIsMobileMultiViewEnabled) {
		X = 0;
	}

	// UE_LOG(LogSVR, Log, TEXT("AdjustViewRectsiez(%d,%d)(%d,%d)"), X,Y, SizeX, SizeY);

}

//-----------------------------------------------------------------------------
// void FSkyWorthVRHMD::CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation)
// {
//     if( StereoPassType != eSSP_FULL)
//     {
//         const float EyeOffset = (GetInterpupillaryDistance() * 0.5f) * WorldToMeters;
//         const float PassOffset = (StereoPassType == eSSP_LEFT_EYE) ? -EyeOffset : EyeOffset;
//         ViewLocation += ViewRotation.Quaternion().RotateVector(FVector(0,PassOffset,0));

// 		//UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::CalculateStereoViewOffset x:%f, y:%f, z:%f"), ViewLocation.X, ViewLocation.Y, ViewLocation.Z);
//     }
// }

//-----------------------------------------------------------------------------
#if ENGINE_MAJOR_VERSION == 5
FMatrix FSkyWorthVRHMD::GetStereoProjectionMatrix(const int32 ViewIndex) const
#else
FMatrix FSkyWorthVRHMD::GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const
#endif
{
	 sxrDeviceInfo di = sxrGetDeviceInfo();

#if ENGINE_MAJOR_VERSION == 5
	 sxrViewFrustum Frustum = (ViewIndex == eSSE_LEFT_EYE) ? di.leftEyeFrustum : di.rightEyeFrustum;
#else
	 sxrViewFrustum Frustum = (StereoPassType == eSSP_LEFT_EYE) ? di.leftEyeFrustum : di.rightEyeFrustum;

#endif

	 float InvNearZ = 1.0f / Frustum.near;

	 float Right = Frustum.right * InvNearZ;
	 float Left = Frustum.left * InvNearZ;
	 float Bottom = Frustum.bottom * InvNearZ;
	 float Top = Frustum.top * InvNearZ;

	 float ZNear = GNearClippingPlane;

	 float SumRL = (Right + Left);
	 float SumTB = (Top + Bottom);
	 float InvRL = (1.0f / (Right - Left));
	 float InvTB = (1.0f / (Top - Bottom));

	 return FMatrix(
	 	FPlane((2.0f * InvRL), 0.0f, 0.0f, 0.0f),
	 	FPlane(0.0f, (2.0f * InvTB), 0.0f, 0.0f),
	 	FPlane((SumRL * InvRL), (SumTB * InvTB), 0.0f, 1.0f),
	 	FPlane(0.0f, 0.0f, ZNear, 0.0f)
	 );
}

FIntPoint FSkyWorthVRHMD::GetIdealRenderTargetSize() const
{
	// UE_LOG(LogSVR,Log,TEXT("xmh GetIdealRenderTargetSize:(%d,%d)"),RenderTargetSize.X,RenderTargetSize.Y)
	return RenderTargetSize;
}

//-----------------------------------------------------------------------------
// void FSkyWorthVRHMD::InitCanvasFromView(FSceneView* InView, UCanvas* Canvas)
// {
//     //Do Nothing
// }

//-----------------------------------------------------------------------------
#if ENGINE_MAJOR_VERSION == 5
void FSkyWorthVRHMD::GetEyeRenderParams_RenderThread(const struct FHeadMountedDisplayPassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
#else
void FSkyWorthVRHMD::GetEyeRenderParams_RenderThread(const FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const
#endif
{
#if ENGINE_MAJOR_VERSION == 5
	if (Context.View.StereoViewIndex == eSSE_LEFT_EYE)
#else
	if (Context.View.StereoPass == eSSP_LEFT_EYE)
#endif
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
void FSkyWorthVRHMD::CalculateRenderTargetSize(const class FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY)
{
    check(IsInGameThread());

	InOutSizeX = GetIdealRenderTargetSize().X;
	InOutSizeY = GetIdealRenderTargetSize().Y;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::NeedReAllocateViewportRenderTarget(const FViewport& Viewport)
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
		// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::NeedReAllocateViewportRenderTarget returns true"));
		return true;
    }
    
	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::NeedReAllocateViewportRenderTarget returns false"));
	return false;
}

//-----------------------------------------------------------------------------
//bool FSkyWorthVRHMD::ShouldUseSeparateRenderTarget() const
//{
//
//	//UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ShouldUseSeparateRenderTarget() called"));
//    check(IsInGameThread());
//    return IsStereoEnabled();
//}

//-----------------------------------------------------------------------------
//void FSkyWorthVRHMD::UpdateViewport(bool bUseSeparateRenderTarget, const FViewport& InViewport, SViewport* ViewportWidget)
//{
//    check(IsInGameThread());
//    FRHIViewport* const ViewportRHI = InViewport.GetViewportRHI().GetReference();
//
//	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::UpdateViewport 1"));
//
//    if (!IsStereoEnabled())
//    {
//        if (!bUseSeparateRenderTarget)
//        {
//            ViewportRHI->SetCustomPresent(nullptr);
//			// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::UpdateViewport 2"));
//
//        }
//        return;
//    }
//	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::UpdateViewport 3"));
//	pSkyWorthVRBridge->UpdateViewport(InViewport, ViewportRHI);
//}

//-----------------------------------------------------------------------------
// FSkyWorthVRHMD, ISceneViewExtension Implementation
//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetupViewFamily(FSceneViewFamily& InViewFamily) 
{
#if WITH_EDITOR
	InViewFamily.EngineShowFlags.SetScreenPercentage(false);
#endif

	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::SetupViewFamily() and stereo is %s"), InViewFamily.EngineShowFlags.StereoRendering ? TEXT("enabled"):TEXT("disabled"));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView)
{
 	//InView.ViewRect = EyeRenderViewport[EyeIndex];
	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::SetupView()"));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::BeginRenderViewFamily(FSceneViewFamily& InViewFamily)
{

}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
    check(IsInRenderingThread());

	// RBF - I suspect this has to be rewritten

	UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::PreRenderViewFamily_RenderThread()"));

    // pSkyWorthVRBridge->BeginRendering(RHICmdList, InViewFamily);
	ExecuteOnRHIThread([this]()
	{
		BeginFrame_RHIThread();
		PreRenderFrame_RHIThread();
	});
}
void FSkyWorthVRHMD::PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily)
{
	/**
	* Allows to render content after the 3D content scene, useful for debugging
	*/

	// ExecuteOnRHIThread([this]()
	// {
	// 	PostRenderFrame_RHIThread();
	// });
	
}
#if ENGINE_MINOR_VERSION < 27 && ENGINE_MAJOR_VERSION < 5
bool FSkyWorthVRHMD::IsActiveThisFrame(FViewport* InViewport) const
{
	return GEngine && GEngine->IsStereoscopic3D(InViewport);
}
#else
bool FSkyWorthVRHMD::IsActiveThisFrame_Internal(const FSceneViewExtensionContext& Context) const
{
	return GEngine && GEngine->IsStereoscopic3D(Context.Viewport);
}
#endif

#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION == 5
bool FSkyWorthVRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples)
#else
bool FSkyWorthVRHMD::AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples /*= 1*/)
#endif
{
	UE_LOG(LogSVR, Log, TEXT("Use SwapChain"));
	initGLExt();
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

		if (bEnableFoveation)
		{
			glTexParameteri(textureType,
				GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM,
				GL_FOVEATION_ENABLE_BIT_QCOM | GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM);
		}
		/*glTexParameteri(textureType, GL_TEXTURE_PREVIOUS_SOURCE_TEXTURE_QCOM, 0);
		glTexParameterf(textureType, GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM, 0.0625f);*/
		// GLfloat color[4]={0,0,0,1};
		// glTexParameterfv(textureType,GL_TEXTURE_BORDER_COLOR,color);
		glBindTexture(textureType, 0);
		TextureResources[i] = TextureID;
		UE_LOG(LogSVR, Log, TEXT("TextureResources[%d]:%d,siez(%d,%d)"), i, TextureResources[i], SizeX, SizeY);
	}
	RenderBridge->CreateSwapChain(EPixelFormat(PF_R8G8B8A8), SizeX, SizeY, bIsMobileMultiViewEnabled ? 2 : 1, NumMips, NumSamples, Flags, TargetableTextureFlags, MobileMSAAValue, TextureResources);
	OutTargetableTexture = OutShaderResourceTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
	return true;
}


void FSkyWorthVRHMD::StartRendering_RHIThread()
{
	if(bStartRendering)
		return;

	UE_LOG(LogSVR, Log, TEXT("StartRendering ; InRenderThread = %s, InRHIThread = %s"), InRenderThread() ? TEXT("True") : TEXT("Flase"), IsInRHIThread() ? TEXT("True") : TEXT("Flase"));
	bStartRendering = true;
	uint32_t TrackingMode = kTrackingRotation | kTrackingPosition;
	sxrSetTrackingMode(TrackingMode);
    sxrBeginParams beginParams;
    beginParams.cpuPerfLevel = sxrPerfLevel::kPerfMaximum;
    beginParams.gpuPerfLevel = sxrPerfLevel::kPerfMaximum;
    beginParams.nativeWindow = AndroidEGL::GetInstance()->GetNativeWindow();
    beginParams.mainThreadId = gettid();
    beginParams.colorSpace = kColorSpaceLinear;

	UE_LOG(LogSVR,Log,TEXT("xmh GSXR_nativeStartSlam: cpuPerfLevel = %d, gpuPerfLevel = %d, nativeWindow = %d, mainThreadId = %d, colorSpace = %d"),
		beginParams.cpuPerfLevel,
		beginParams.gpuPerfLevel,
		beginParams.nativeWindow,
		beginParams.mainThreadId,
		beginParams.colorSpace);

    SxrResult result = sxrBeginXr(&beginParams);
	mFrameIndex = 0;
	UE_LOG(LogSVR,Log,TEXT("GSXR_nativeStartSlam:%d"),result);
}

void FSkyWorthVRHMD::StopRendering_RHIThread()
{
	if(!bStartRendering)
	{
		return;
	}
	UE_LOG(LogSVR,Log,TEXT("xmh StopRendering_RHIThread"));
	bStartRendering = false;
	sxrEndXr();
	// SC::GSXR_nativeShutdown();
	// bInitialized = false;
}

void FSkyWorthVRHMD::BeginFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}

#if ENGINE_MINOR_VERSION < 27 && ENGINE_MAJOR_VERSION < 5
	RenderBridge->SwapChain->IncrementSwapChainIndex_RHIThread(0);
#else
	RenderBridge->SwapChain->IncrementSwapChainIndex_RHIThread();
#endif
	
}

void FSkyWorthVRHMD::PreRenderFrame_RHIThread()
{
	if (!bStartRendering)
	{
		return;
	}
	UpdateSensorData();
}
void FSkyWorthVRHMD::PostRenderFrame_RHIThread()
{

}
void CreateLayout(float centerX, float centerY, float radiusX, float radiusY, sxrLayoutCoords *pLayout)
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

void FSkyWorthVRHMD::EndFrame_RHIThread()
{
	if(!bStartRendering)
		return;
	sxrFrameParams FrameParams;
	memset(&FrameParams, 0, sizeof(FrameParams));

	FrameParams.frameIndex = mFrameIndex;//GFrameNumber

	float FoveationGainX, FoveationGainY, FoveationArea;

	switch (FoveationLevel.GetValue())
	{
	case EFoveationLevel::Type::Low: {
		FoveationGainX = 3.f * 2.f;
		FoveationGainY = 3.f;
		FoveationArea = 10.f; }break;
	case EFoveationLevel::Type::Med: {
		FoveationGainX = 4.f;
		FoveationGainY = 4.f;
		FoveationArea = 10.f; }break;
	case EFoveationLevel::Type::High: {
		FoveationGainX = 6.f;
		FoveationGainY = 6.f;
		FoveationArea = 10.f; }break;
	case EFoveationLevel::Type::TopHigh: {
		FoveationGainX = 8.f * 2.f;
		FoveationGainY = 8.f;
		FoveationArea = 0.f; }break;
	}

	if (bIsMobileMultiViewEnabled){
		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2DArray() ? RenderBridge->SwapChain->GetTexture2DArray() : RenderBridge->SwapChain->GetTexture2D();
		FrameParams.renderLayers[0].imageType = kTypeTextureArray;
		FrameParams.renderLayers[0].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].eyeMask = kEyeMaskLeft;
		FrameParams.renderLayers[0].layerFlags |= kLayerFlagOpaque;

		// Right Eye
		FrameParams.renderLayers[1].imageType = kTypeTextureArray;
		FrameParams.renderLayers[1].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].eyeMask = kEyeMaskRight;
		FrameParams.renderLayers[1].layerFlags |= kLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - rendering texture ID %d - from a texture array"), FrameParams.renderLayers[0].imageHandle);
		
		if (glTextureFoveationParametersQCOM == 0)
		{
			UE_LOG(LogSVR, Log, TEXT("glTextureFoveationParametersQCOM is not supported!"));
		}
		else
		{
			glTextureFoveationParametersQCOM(*(GLuint*)SwapChainTexture->GetNativeResource(), 0, 0, -0.5f, 0.f, FoveationGainX * 2.f, FoveationGainY, FoveationArea);
			glTextureFoveationParametersQCOM(*(GLuint*)SwapChainTexture->GetNativeResource(), 0, 1, 0.5f, 0.f, FoveationGainX * 2.f, FoveationGainY, FoveationArea);
		}
	}
	else
	{

		const FRHITexture2D* const SwapChainTexture = RenderBridge->SwapChain->GetTexture2D();
		FrameParams.renderLayers[0].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		FrameParams.renderLayers[0].imageType = kTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].imageCoords.LowerUVs[2] = 0.5f;
		FrameParams.renderLayers[0].imageCoords.UpperUVs[2] = 0.5f;
		FrameParams.renderLayers[0].eyeMask = kEyeMaskLeft;
		//FrameParams.renderLayers[0].layerFlags |= kLayerFlagOpaque;

		FrameParams.renderLayers[1].imageHandle = *(GLuint *)SwapChainTexture->GetNativeResource();
		FrameParams.renderLayers[1].imageType = kTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].imageCoords.LowerUVs[0] = 0.5f;
		FrameParams.renderLayers[1].imageCoords.UpperUVs[0] = 0.5f;
		FrameParams.renderLayers[1].eyeMask = kEyeMaskRight;
		
		//FrameParams.renderLayers[1].layerFlags |= kLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - not using array"));

		if (glTextureFoveationParametersQCOM == 0)
		{
			UE_LOG(LogSVR, Log, TEXT("glTextureFoveationParametersQCOM is not supported!"));
		}
		else
		{
			glTextureFoveationParametersQCOM(*(GLuint*)SwapChainTexture->GetNativeResource(), 0, 0, -0.5f, 0.f, FoveationGainX * 2.f, FoveationGainY, FoveationArea);
			glTextureFoveationParametersQCOM(*(GLuint*)SwapChainTexture->GetNativeResource(), 0, 1, 0.5f, 0.f, FoveationGainX * 2.f, FoveationGainY, FoveationArea);
		}
	}

	
	
	FrameParams.frameOptions = kDisableChromaticCorrection;
	FrameParams.headPoseState = CurrentFrame_RenderThread.headPoseState;
	UE_LOG(LogSVR,Log,TEXT("headPoseState:(%f,%f,%f),(%f,%f,%f,%f)"),FrameParams.headPoseState.pose.position.x,FrameParams.headPoseState.pose.position.y,FrameParams.headPoseState.pose.position.z,
	FrameParams.headPoseState.pose.rotation.x,FrameParams.headPoseState.pose.rotation.y,FrameParams.headPoseState.pose.rotation.z,FrameParams.headPoseState.pose.rotation.w);
	FrameParams.minVsyncs = 1;
	UE_LOG(LogSVR,Log,TEXT("(%s) (Frame %d) SubmitFrame(mRenderPose) => Calling sxrSubmitFrame(Frame %d),imageHandle=%d"), IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), FrameParams.frameIndex, FrameParams.frameIndex,FrameParams.renderLayers[0].imageHandle);
	UE_LOG(LogSVR, Log, TEXT("SubmitFrame : frameIndex = %d"), FrameParams.frameIndex);
	UE_LOG(LogSVR, Log, TEXT("SubmitFrame : minVsyncs = %d"), FrameParams.minVsyncs);

	
	
	sxrSubmitFrame(&FrameParams);

	
	mFrameIndex++;
}

//-----------------------------------------------------------------------------
// FSkyWorthVRHMD Implementation
//-----------------------------------------------------------------------------

//-----------------------------------------------------------------------------
// FSkyWorthVRHMD Implementation
//-----------------------------------------------------------------------------
FSkyWorthVRHMD::FSkyWorthVRHMD(const FAutoRegister& AutoRegister) :
	FHeadMountedDisplayBase(nullptr),
	FSceneViewExtensionBase(AutoRegister),
	bInitialized(false),
    bResumed(false),
#if ENGINE_MINOR_VERSION == 27
	bIsStandaloneStereoOnlyDevice(false),
#endif
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
	sxrSetting = GetMutableDefault<USXR_Settings>();

	//m_MotionUtilsImple = new MotionUtilsImple(GJavaVM, GNativeAndroidApp->activity->clazz);

	XRLoaderInit();
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

#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION ==5
int32 FSkyWorthVRHMD::GetXRSystemFlags() const
{
	return EXRSystemFlags::IsHeadMounted;
}
#endif


//-----------------------------------------------------------------------------
FSkyWorthVRHMD::~FSkyWorthVRHMD()
{
    UE_LOG(LogSVR, Log, TEXT("~FSkyWorthVRHMD()"));

}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::IsInitialized() const
{
	if (bInitialized)
	{
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::IsInitialized() is true"));
	}
	else
	{
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::IsInitialized() is false"));
	}
	return bInitialized;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::InitializeExternalResources()
{
	UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::InitializeExternalResources()"));

	CVarPerfLevelCpu.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&PerfLevelCpuOnChangeByCvar));
	CVarPerfLevelGpu.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&PerfLevelGpuOnChangeByCvar));

    extern struct android_app* GNativeAndroidApp;

    sxrInitParams initParams;

    initParams.javaVm = GJavaVM;
    initParams.javaEnv = FAndroidApplication::GetJavaEnv();
    initParams.javaActivityObject = GNativeAndroidApp->activity->clazz;

//#if WITH_TELEMETRY
//    if (sxrInitializeOptArgs(&initParams, TM_API_PTR) != SXR_ERROR_NONE)
//#else
    if (sxrInitialize(&initParams) != SXR_ERROR_NONE)
//#endif
    {
        UE_LOG(LogSVR, Error, TEXT("sxrInitialize failed"));
        return false;
    }
    else
    {
        UE_LOG(LogSVR, Log, TEXT("sxrInitialize succeeeded"));

		///StartWebHelper();
        uint32 TrackingMode = kTrackingRotation | kTrackingPosition;
		CVars::EyeTrackingEnabled = false;
        // if (CVars::EyeTrackingEnabled)
        // {
        //     if ((sxrGetSupportedTrackingModes() & kTrackingEye) == 0)
        //     {
		// 		UE_LOG(LogSVR, Log, TEXT("Eye tracking is not supported on this device"));
        //         CVars::EyeTrackingEnabled = false;
        //     }
        //     else
        //     {
        //         TrackingMode |= kTrackingEye;
        //     }
        // }
		UE_LOG(LogSVR,Log,TEXT("GSXR_nativeSetSlamType:%d"),TrackingMode);
		sxrSetTrackingMode(TrackingMode);
        return true;
    }
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRHMD::Startup()
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::Startup(); this=%x"), this);

    bInitialized = InitializeExternalResources();
    if (!bInitialized)
    {
		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::Startup() - Initialization FAILED"), this);
        return false;
    }

	// pSkyWorthVRBridge = FSkyWorthVRHMDCustomPresent::Create(this);
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
    FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FSkyWorthVRHMD::ApplicationWillEnterBackgroundDelegate);
    FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FSkyWorthVRHMD::ApplicationWillDeactivateDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationWillEnterBackgroundDelegate(), but cover the possibility that only this delegate is called
    FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FSkyWorthVRHMD::ApplicationHasEnteredForegroundDelegate);
    FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FSkyWorthVRHMD::ApplicationHasReactivatedDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationHasEnteredForegroundDelegate(), but cover the possibility that only this delegate is called

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

	sxrDeviceInfo DeviceInfo = sxrGetDeviceInfo();
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

		UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::Startup(): r.MobileContentScaleFactor = %f -> eye buffer size is now = %u x %u"),
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
	bEnableFoveation = sxrSetting->bEnableFoveation;
	UE_LOG(LogSVR, Log, TEXT("bEnableFoveation is %d"), bEnableFoveation);
	FoveationLevel = sxrSetting->FoveationLevel;
	UE_LOG(LogSVR, Log, TEXT("FoveationLevel is %d"), FoveationLevel.GetValue());
#if ENGINE_MAJOR_VERSION == 5
	MobileMSAAValue = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.Mobile.AntiAliasing"))->GetValueOnAnyThread() == 3;
#else
	MobileMSAAValue = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("r.MobileMSAA"))->GetValueOnAnyThread();
#endif
	UE_LOG(LogSVR, Log, TEXT("MSAA = %d"), MobileMSAAValue);
	return true;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ApplicationWillEnterBackgroundDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ApplicationWillEnterBackgroundDelegate"));
    CleanupIfNecessary();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::CleanupIfNecessary()
{
	if(!bResumed)
	{
		return;
	}
    bResumed = false;

    // if (pSkyWorthVRBridge->bInVRMode)
    // {
    //     UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::CleanupIfNecessary(): sxrEndXr() sxrShutdown()"));

    //     //stop consuming resources for VR until the app is resumed
	// 	sxrEndXr();
    //     SC::GSXR_nativeShutdown();
    //     pSkyWorthVRBridge->bInVRMode = false;
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
void FSkyWorthVRHMD::ApplicationWillDeactivateDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ApplicationWillDeactivateDelegate"));
    CleanupIfNecessary();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ApplicationHasReactivatedDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ApplicationHasReactivatedDelegate"));
    InitializeIfNecessaryOnResume();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::ApplicationHasEnteredForegroundDelegate()
{
    UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::ApplicationHasEnteredForegroundDelegate"));
    InitializeIfNecessaryOnResume();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::InitializeIfNecessaryOnResume()
{
    if (!bInitialized)//Upon application launch, FSkyWorthVRHMD::Startup() must initialize immediately, but Android lifecycle "resume" delegates will also be called -- don't double-initialize
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
	UE_LOG(LogSVR, Log, TEXT("StartRendering ; bStartRendering = %s"), bStartRendering ? TEXT("True") : TEXT("Flase"));
	FlushRenderingCommands();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetCPUAndGPULevels(const int32 InCPULevel, const int32 InGPULevel) const
{
	const sxrPerfLevel SvrPerfLevelCPU = static_cast<const sxrPerfLevel>(InCPULevel);
	const sxrPerfLevel SvrPerfLevelGPU = static_cast<const sxrPerfLevel>(InGPULevel);
	check(IsPerfLevelValid(SvrPerfLevelCPU));
	check(IsPerfLevelValid(SvrPerfLevelGPU));

	//sxr performance levels can be manipulated by render or game thread, so prevent race conditions
	// FScopeLock ScopeLock(&pSkyWorthVRBridge->PerfLevelCriticalSection);
	// PerfLevelCpuLastSet = SvrPerfLevelCPU;
	// PerfLevelGpuLastSet = SvrPerfLevelGPU;
// CTORNE ->
	// if (pSkyWorthVRBridge->bInVRMode)
	// {
	// 	sxrSetPerformanceLevels(PerfLevelCpuLastSet, PerfLevelGpuLastSet);
	// 	FSkyWorthVRHMD::PerfLevelLog(TEXT("SetCPUAndGPULevels"), PerfLevelCpuLastSet, PerfLevelGpuLastSet);
	// }
// <- CTORNE
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::PoseToOrientationAndPosition(const sxrHeadPose& Pose, FQuat& OutCurrentOrientation, FVector& OutCurrentPosition, const float WorldToMetersScale)
{
	// UE_LOG(LogSVR, Log, TEXT("FSkyWorthVRHMD::PoseToOrientationAndPosition()"));

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
void FSkyWorthVRHMD::BeginVRMode()
{
    // check(pSkyWorthVRBridge);
	//FAndroidMisc::UnlockAndroidWindow();
    // if (IsInRenderingThread())
    // {
    //     pSkyWorthVRBridge->DoBeginVR();
    // }
    // else
    // {
	// 	FSkyWorthVRHMDCustomPresent* m_SkyWorthVRBridge = pSkyWorthVRBridge;

	// 	ENQUEUE_RENDER_COMMAND(BeginVRMode)
	// 	(
	// 		[m_SkyWorthVRBridge](FRHICommandListImmediate& RHICmdList)
	// 		{
	// 			m_SkyWorthVRBridge->DoBeginVR();
	// 		}
	// 	);

    //     FlushRenderingCommands();
    // }
}

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::EndVRMode()
{
}

//-----------------------------------------------------------------------------
// uint32 FSkyWorthVRHMD::GetNumberOfBufferedFrames() const
// {
//     return 1;
// }

//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::SetupOcclusionMeshes()
{
	int TriangleCount = 0;
	int VertexStride = 0;
	if (sxrGetOcclusionMesh(kLeftEye, &TriangleCount, &VertexStride, nullptr) != SXR_ERROR_NONE)
	{
		return;
	}

	const uint32 VertexCount = TriangleCount * 3;

	if (VertexCount > 0 && VertexStride > 0)
	{
		const uint32 VertexDataSize = VertexCount * VertexStride;

		float* const LeftEyeMesh = new float[VertexDataSize];
		float* const RightEyeMesh = new float[VertexDataSize];
		sxrGetOcclusionMesh(kLeftEye, &TriangleCount, &VertexStride, LeftEyeMesh);
		sxrGetOcclusionMesh(kRightEye, &TriangleCount, &VertexStride, RightEyeMesh);

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
sxrDeviceInfo FSkyWorthVRHMD::GetDeviceInfo()
{
	sxrDeviceInfo di = sxrGetDeviceInfo();
	
	// if any coordinate system transformations are necessary, they should go here

	return di;
}


uint32 FSkyWorthVRHMD::CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	const uint32 LayerId = CurrentLayerId++;
	UE_LOG(LogSVR,Log,TEXT(" CreateLayer %d"),LayerId);
	LayerMap.Add(LayerId, MakeShareable(new FGSXRStereoLayer(LayerId, InLayerDesc)));
	return LayerId;
}

void FSkyWorthVRHMD::DestroyLayer(uint32 LayerId)
{
	check(IsInGameThread());
	UE_LOG(LogSVR,Log,TEXT(" DestroyLayer %d"),LayerId);
	LayerMap.Remove(LayerId);
}

void FSkyWorthVRHMD::SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc)
{
	check(IsInGameThread());
	FGSXRLayerPtr* LayerFound = LayerMap.Find(LayerId);

	if (LayerFound)
	{
		LayerFound->Get()->SetLayerDesc(InLayerDesc);
	}
}

bool FSkyWorthVRHMD::GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc)
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
void FSkyWorthVRHMD::MarkTextureForUpdate(uint32 LayerId)
{
	check(IsInGameThread());
}

void FSkyWorthVRHMD::UpdateSplashScreen()
{
}

IStereoLayers::FLayerDesc FSkyWorthVRHMD::GetDebugCanvasLayerDesc(FTextureRHIRef Texture)
{
	IStereoLayers::FLayerDesc StereoLayerDesc;
	StereoLayerDesc.Priority = INT_MAX;
	StereoLayerDesc.Transform = FTransform(FVector(180.f, 0, 0));
	StereoLayerDesc.Transform.SetScale3D(FVector(1,1,-1));
	StereoLayerDesc.QuadSize = FVector2D(180.f, 180.f);
	StereoLayerDesc.LayerSize = Texture->GetTexture2D()->GetSizeXY();
	StereoLayerDesc.PositionType = IStereoLayers::ELayerType::FaceLocked;
#if ENGINE_MINOR_VERSION >24 || ENGINE_MAJOR_VERSION == 5
	StereoLayerDesc.SetShape<FQuadLayer>();
#else
	StereoLayerDesc.ShapeType = IStereoLayers::ELayerShape::QuadLayer;
#endif
	StereoLayerDesc.Texture = Texture;
	StereoLayerDesc.Flags = IStereoLayers::ELayerFlags::LAYER_FLAG_TEX_CONTINUOUS_UPDATE;
	StereoLayerDesc.Flags |= IStereoLayers::ELayerFlags::LAYER_FLAG_QUAD_PRESERVE_TEX_RATIO;
	return StereoLayerDesc;
}
void FSkyWorthVRHMD::GetAllocatedTexture(uint32 LayerId, FTextureRHIRef& Texture, FTextureRHIRef& LeftTexture)
{
	Texture = LeftTexture = nullptr;
}

FXRRenderBridge* FSkyWorthVRHMD::GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget)
{
	return RenderBridge;
}


#if 0 // webhelper stuff
//-----------------------------------------------------------------------------
void FSkyWorthVRHMD::StartWebHelper()
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
void FSkyWorthVRHMD::SendMessage(const int32 score) const
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
void FSkyWorthVRHMD::StopWebHelper()
{

	JNIEnv* Env = FAndroidApplication::GetJavaEnv();
	jmethodID methodid = Env->GetStaticMethodID(webHelperClass, "stop", "()V");
	//UE_LOG(LogSVR, Log, TEXT("StopWebHelper after GetStaticMethodID : stop  %x "), methodid);

	Env->CallStaticVoidMethod(webHelperClass, methodid);
	UE_LOG(LogSVR, Log, TEXT("StopScore after :stop"));

}

#endif // webhelper

#endif //SkyWorthVR_HMD_SUPPORTED_PLATFORMS
