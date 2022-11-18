//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "../Public/SkyWorthVRHMDFunctionLibrary.h"
#include "SkyWorthVRHMD.h"
#include "SkyWorthXR_CVars.h"
#include "SXR_Settings.h"

typedef enum
{
	SXR_FOVEATION_LEVEL_NONE = -1,
	SXR_FOVEATION_LEVEL_LOW = 0,
	SXR_FOVEATION_LEVEL_MID = 1,
	SXR_FOVEATION_LEVEL_HIGH = 2,
	SXR_FOVEATION_LEVEL_TOP_HIGH = 3
} sxrFoveationLevel;

//-----------------------------------------------------------------------------
USkyWorthVRHMDFunctionLibrary::USkyWorthVRHMDFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::SetCpuAndGpuLevelsSVR(const int32 CpuLevel, const int32 GpuLevel)
{
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();

	// This is probably how we get the system w/o using engine
	//auto FSkyWorthVRHMDp = FSceneViewExtensions::NewExtension<FSkyWorthVRHMD>();

	if (HMD)
	{
		HMD->SetCPUAndGPULevels(CpuLevel, GpuLevel);
	}
#endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
USkyWorthVRHMDEventManager* USkyWorthVRHMDFunctionLibrary::GetSkyWorthVRHMDEventManager()
{
	return USkyWorthVRHMDEventManager::GetInstance();
}

#pragma region EyeTracking
//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::GetEyeGazeDirection(FVector& OutDirection, bool& Success)
{
	Success = false;

#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();

	if (nullptr == HMD || false == HMD->isEyeTrackingEnabled())
	{
		return;
	}

	Success = HMD->GetRelativeEyeDirection(HMD->HMDDeviceId, OutDirection);

	FQuat HMD_Orientation;
	FVector HMD_Position;

	// rotate eye gaze by HMD rotation
	if (Success &&  (Success = HMD->GetCurrentPose(HMD->HMDDeviceId, HMD_Orientation, HMD_Position) ) )
	{
		OutDirection = HMD_Orientation * OutDirection;

	}


#endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::GetEyeOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotation, bool& Success)
{
	Success = false;
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();

	if (nullptr == HMD || false == HMD->isEyeTrackingEnabled())
	{
		return;
	}

	// doesn't care about eye parameter....
#if ENGINE_MAJOR_VERSION == 5
	Success =  HMD->GetRelativeEyePose(HMD->HMDDeviceId, EStereoscopicEye::eSSE_LEFT_EYE, OutOrientation, OutPosition);
#else
	Success = HMD->GetRelativeEyePose(HMD->HMDDeviceId, EStereoscopicPass::eSSP_LEFT_EYE, OutOrientation, OutPosition);
#endif
	FQuat HMD_Orientation;
	FVector HMD_Position;

	if (Success && (Success = HMD->GetCurrentPose(HMD->HMDDeviceId, HMD_Orientation, HMD_Position)))
	{
		OutPosition = HMD_Orientation * OutPosition + HMD_Position;
		OutOrientation = HMD_Orientation * OutOrientation;
		OutRotation = OutOrientation.Rotator();
	}

#endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
// void USkyWorthVRHMDFunctionLibrary::GetRelativeEyePose(FVector& OutDirection, FQuat& OutOrientation, bool& Success)
// {
// 	Success = false;
// #if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
// 	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
// 
// 	if (nullptr == HMD || false == HMD->isEyeTrackingEnabled())
// 	{
// 		return;
// 	}
//  unfinished
// 
// 	FVector position, orientation;
// 
// 	if (HMD->GetRelativeEyePose(HMD->HMDDeviceId, EStereoscopicPass.eSSP_LEFT_EYE, OutOrientation, OutPosition);
// 
// 	if ( HMD->GetRelativeEyePose(HMD->HMDDeviceId, EStereoscopicPass.eSSP_LEFT_EYE, OutOrientation, OutPosition);
// 
// #endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
// 	return;
// }


//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::GetEyeOpenness(ESkyWorthVRHMD_Eye WhichEye, float& Openness, bool& Success)
{
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	Success = false;

	if (nullptr == HMD || HMD->isEyeTrackingEnabled())
	{
		return;
	}



	const sxrEyePoseState eps = HMD->GetLatestEyePoseState();

	// Return either eye or averaged value
	Openness = WhichEye == ESkyWorthVRHMD_Eye::LeftEye ? eps.leftEyeOpenness :
		(WhichEye == ESkyWorthVRHMD_Eye::RightEye ? eps.rightEyeOpenness : (eps.leftEyeOpenness + eps.rightEyeOpenness)*0.5f);

	Success = true;

#endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::GetPupilDialation(ESkyWorthVRHMD_Eye WhichEye, float& Dialation, bool& Success)
{
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	Success = false;

	if (nullptr == HMD || HMD->isEyeTrackingEnabled())
	{
		return;
	}

	const sxrEyePoseState eps = HMD->GetLatestEyePoseState();

	// Return either eye or averaged value
	Dialation = WhichEye == ESkyWorthVRHMD_Eye::LeftEye ? eps.leftEyePupilDilation :
		(WhichEye == ESkyWorthVRHMD_Eye::RightEye ? eps.rightEyePupilDilation : (eps.leftEyePupilDilation + eps.rightEyePupilDilation)*0.5f);

	Success = true;

#endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
// void USkyWorthVRHMDFunctionLibrary::IsEyetrackingEnabled(bool& IsEnabled)
// {
// #if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
// 	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
// 
// 	IsEnabled = (nullptr != HMD && HMD->isEyeTrackingEnabled()) ? true : false;
// 
// #endif//#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
// 
// }

#pragma endregion EyeTracking

#pragma region HMD

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::GetHeadOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotator, bool& Success)
{
	Success = false;
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	if (HMD)
	{
		Success = HMD->GetCurrentPose(HMD->HMDDeviceId, OutOrientation, OutPosition);
		OutRotator = OutOrientation.Rotator();
	}
#endif // SkyWorthVR_HMD_SUPPORTED_PLATFORMS

	return;
}

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::RecenterOrientation( void )
{
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	if (HMD)
	{
		HMD->ResetOrientation(0.0f);
	}
#endif // SkyWorthVR_HMD_SUPPORTED_PLATFORMS

	return;
}

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::RecenterPosition( void )
{
#if SkyWorthVR_HMD_SUPPORTED_PLATFORMS
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	if (HMD)
	{
		HMD->ResetPosition();
	}
#endif // SkyWorthVR_HMD_SUPPORTED_PLATFORMS

	return;
}


/*   parts of the eye struct we've done nothing with yet...
struct sxrEyePoseState
{
	float               leftEyePositionGuide[3];    //!< Position of the inner corner of the left eye in meters from the HMD center-eye coordinate system's origin.
	float               rightEyePositionGuide[3];   //!< Position of the inner corner of the right eye in meters from the HMD center-eye coordinate system's origin.
};
*/

#pragma endregion HMD


#pragma region Foveation
//-----------------------------------------------------------------------------
// Foveation


#pragma region Eyetracking


//-----------------------------------------------------------------------------
// no set...
void USkyWorthVRHMDFunctionLibrary::getAverageGazeDirection(float& x, float& y)
{
	CVars::AverageGazeDirectionX = x;
	CVars::AverageGazeDirectionY = y;
}

//-----------------------------------------------------------------------------
//r.SVR.EyeMarkerRadius
float USkyWorthVRHMDFunctionLibrary::getEyetrackingMarkerRadius() 
{
	return CVars::EyeMarkerRadius;
}
//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::setEyetrackingMarkerRadius(float  x)
{
	CVars::EyeMarkerRadius = x;
}





//-----------------------------------------------------------------------------
bool USkyWorthVRHMDFunctionLibrary::isEyetrackingMarkerEnabled()
{
	return CVars::EyeMarkerEnabled == 1;
}
//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::enableEyetrackingMarker(bool t)
{
	CVars::EyeMarkerEnabled = t ? 1 : 0;
}


//-----------------------------------------------------------------------------
//r.SVR.EyeTrackingEnabled
// no set
bool USkyWorthVRHMDFunctionLibrary::isEyetrackingEnabled()
{
	return CVars::EyeTrackingEnabled == 1;
}

#pragma endregion


#if 0 // webhelper

//-----------------------------------------------------------------------------
void USkyWorthVRHMDFunctionLibrary::SendMessage(const int32 score)
{
	FSkyWorthVRHMD* HMD = FSkyWorthVRHMD::GetSkyWorthHMD();
	if (HMD)
	{
		HMD->SendMessage(score);
	}
}

#endif // webhelper
