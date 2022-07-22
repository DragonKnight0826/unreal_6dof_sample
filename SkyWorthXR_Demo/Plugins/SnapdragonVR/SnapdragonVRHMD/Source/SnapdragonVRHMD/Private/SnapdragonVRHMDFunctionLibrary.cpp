//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "../Public/SnapdragonVRHMDFunctionLibrary.h"
#include "SnapdragonVRHMD.h"
#include "SnapdragonXR_CVars.h"

//-----------------------------------------------------------------------------
USnapdragonVRHMDFunctionLibrary::USnapdragonVRHMDFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
	
}


//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::SetCpuAndGpuLevelsSVR(const int32 CpuLevel, const int32 GpuLevel)
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();

	// This is probably how we get the system w/o using engine
	//auto FSnapdragonVRHMDp = FSceneViewExtensions::NewExtension<FSnapdragonVRHMD>();

	if (HMD)
	{
		HMD->SetCPUAndGPULevels(CpuLevel, GpuLevel);
	}
#endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
USnapdragonVRHMDEventManager* USnapdragonVRHMDFunctionLibrary::GetSnapdragonVRHMDEventManager()
{
	return USnapdragonVRHMDEventManager::GetInstance();
}

#pragma region EyeTracking
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::GetEyeGazeDirection(FVector& OutDirection, bool& Success)
{
	Success = false;

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();

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


#endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::GetEyeOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotation, bool& Success)
{
	Success = false;
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();

	if (nullptr == HMD || false == HMD->isEyeTrackingEnabled())
	{
		return;
	}

	// doesn't care about eye parameter....
	Success =  HMD->GetRelativeEyePose(HMD->HMDDeviceId, EStereoscopicPass::eSSP_LEFT_EYE, OutOrientation, OutPosition);

	FQuat HMD_Orientation;
	FVector HMD_Position;

	if (Success && (Success = HMD->GetCurrentPose(HMD->HMDDeviceId, HMD_Orientation, HMD_Position)))
	{
		OutPosition = HMD_Orientation * OutPosition + HMD_Position;
		OutOrientation = HMD_Orientation * OutOrientation;
		OutRotation = OutOrientation.Rotator();
	}

#endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
// void USnapdragonVRHMDFunctionLibrary::GetRelativeEyePose(FVector& OutDirection, FQuat& OutOrientation, bool& Success)
// {
// 	Success = false;
// #if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
// 	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
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
// #endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
// 	return;
// }


//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::GetEyeOpenness(ESnapdragonVRHMD_Eye WhichEye, float& Openness, bool& Success)
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	Success = false;

	if (nullptr == HMD || HMD->isEyeTrackingEnabled())
	{
		return;
	}



	const GSXREyePoseState eps = HMD->GetLatestEyePoseState();

	// Return either eye or averaged value
	Openness = WhichEye == ESnapdragonVRHMD_Eye::LeftEye ? eps.leftEyeOpenness :
		(WhichEye == ESnapdragonVRHMD_Eye::RightEye ? eps.rightEyeOpenness : (eps.leftEyeOpenness + eps.rightEyeOpenness)*0.5f);

	Success = true;

#endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::GetPupilDialation(ESnapdragonVRHMD_Eye WhichEye, float& Dialation, bool& Success)
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	Success = false;

	if (nullptr == HMD || HMD->isEyeTrackingEnabled())
	{
		return;
	}

	const GSXREyePoseState eps = HMD->GetLatestEyePoseState();

	// Return either eye or averaged value
	Dialation = WhichEye == ESnapdragonVRHMD_Eye::LeftEye ? eps.leftEyePupilDilation :
		(WhichEye == ESnapdragonVRHMD_Eye::RightEye ? eps.rightEyePupilDilation : (eps.leftEyePupilDilation + eps.rightEyePupilDilation)*0.5f);

	Success = true;

#endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	return;
}

//-----------------------------------------------------------------------------
// void USnapdragonVRHMDFunctionLibrary::IsEyetrackingEnabled(bool& IsEnabled)
// {
// #if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
// 	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
// 
// 	IsEnabled = (nullptr != HMD && HMD->isEyeTrackingEnabled()) ? true : false;
// 
// #endif//#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
// 
// }

#pragma endregion EyeTracking

#pragma region HMD

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::GetHeadOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotator, bool& Success)
{
	Success = false;
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	if (HMD)
	{
		Success = HMD->GetCurrentPose(HMD->HMDDeviceId, OutOrientation, OutPosition);
		OutRotator = OutOrientation.Rotator();
	}
#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

	return;
}

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::RecenterOrientation( void )
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	if (HMD)
	{
		HMD->ResetOrientation(0.0f);
	}
#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

	return;
}

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::RecenterPosition( void )
{
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	if (HMD)
	{
		HMD->ResetPosition();
	}
#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS

	return;
}


/*   parts of the eye struct we've done nothing with yet...
struct GSXREyePoseState
{
	float               leftEyePositionGuide[3];    //!< Position of the inner corner of the left eye in meters from the HMD center-eye coordinate system's origin.
	float               rightEyePositionGuide[3];   //!< Position of the inner corner of the right eye in meters from the HMD center-eye coordinate system's origin.
};
*/

#pragma endregion HMD


#pragma region Foveation
//-----------------------------------------------------------------------------
// Foveation


//-----------------------------------------------------------------------------
bool USnapdragonVRHMDFunctionLibrary::isTemporalFoveationEnabled()
{
	return CVars::TemporalFoveationEnabled == 1;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::enableTemporalFoveation(bool t)
{
	CVars::TemporalFoveationEnabled = t ? 1 : 0;
}



//-----------------------------------------------------------------------------
bool USnapdragonVRHMDFunctionLibrary::isTextureFoveationEnabled()
{
	return CVars::TextureFoveationEnabled == 1;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::enableTextureFoveation(bool t)
{
	CVars::TextureFoveationEnabled = t ? 1 : 0;
}


//-----------------------------------------------------------------------------
bool USnapdragonVRHMDFunctionLibrary::isTextureFoveationFocusEnabled()
{
	return CVars::TextureFoveationFocusEnabled == 1;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::enableTextureFoveationFocus(bool t)
{
	CVars::TextureFoveationFocusEnabled = t ? 1 : 0;
}


//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::getTextureFoveationGain(float& x, float& y)
{
	x = CVars::TextureFoveationGainX;
	y = CVars::TextureFoveationGainY;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureFoveationGain(float  x, float y)
{
	CVars::TextureFoveationGainX = x;
	CVars::TextureFoveationGainY = y;
}


//-----------------------------------------------------------------------------
float USnapdragonVRHMDFunctionLibrary::getTextureFoveationArea()
{
	return CVars::TextureFoveationArea;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureFoveationArea(float  x)
{
	CVars::TextureFoveationArea = x;
}
//-----------------------------------------------------------------------------


//-----------------------------------------------------------------------------
float USnapdragonVRHMDFunctionLibrary::getTextureMinimumPixelDensity()
{
	return CVars::TextureFoveationMinPixelDensity;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureMinimumPixelDensity(float  x)
{
	CVars::TextureFoveationMinPixelDensity = x;
}



//-----------------------------------------------------------------------------
float USnapdragonVRHMDFunctionLibrary::getTextureFoveationFocusAmplitude()
{
	return CVars::TextureFoveationFocusAmplitude;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureFoveationFocusAmplitude(float  x)
{
	CVars::TextureFoveationFocusAmplitude = x;
}


//-----------------------------------------------------------------------------
float USnapdragonVRHMDFunctionLibrary::getTextureFoveationFocusFrequency()
{
	return CVars::TextureFoveationFocusFrequency;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureFoveationFocusFrequency(float  x)
{
	CVars::TextureFoveationFocusFrequency = x;
}



//-----------------------------------------------------------------------------
//r.SVR.TextureFoveationFocusFrequencyRho
float USnapdragonVRHMDFunctionLibrary::getTextureFoveationFocusFrequencyRho()
{
	return CVars::TextureFoveationFocusFrequencyRho;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setTextureFoveationFocusFrequencyRho(float  x)
{
	CVars::TextureFoveationFocusFrequencyRho = x;
}

#pragma region Eyetracking


//-----------------------------------------------------------------------------
// no set...
void USnapdragonVRHMDFunctionLibrary::getAverageGazeDirection(float& x, float& y)
{
	CVars::AverageGazeDirectionX = x;
	CVars::AverageGazeDirectionY = y;
}

//-----------------------------------------------------------------------------
//r.SVR.EyeMarkerRadius
float USnapdragonVRHMDFunctionLibrary::getEyetrackingMarkerRadius() 
{
	return CVars::EyeMarkerRadius;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::setEyetrackingMarkerRadius(float  x)
{
	CVars::EyeMarkerRadius = x;
}





//-----------------------------------------------------------------------------
bool USnapdragonVRHMDFunctionLibrary::isEyetrackingMarkerEnabled()
{
	return CVars::EyeMarkerEnabled == 1;
}
//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::enableEyetrackingMarker(bool t)
{
	CVars::EyeMarkerEnabled = t ? 1 : 0;
}


//-----------------------------------------------------------------------------
//r.SVR.EyeTrackingEnabled
// no set
bool USnapdragonVRHMDFunctionLibrary::isEyetrackingEnabled()
{
	return CVars::EyeTrackingEnabled == 1;
}

#pragma endregion


#if 0 // webhelper

//-----------------------------------------------------------------------------
void USnapdragonVRHMDFunctionLibrary::SendMessage(const int32 score)
{
	FSnapdragonVRHMD* HMD = FSnapdragonVRHMD::GetSnapdragonHMD();
	if (HMD)
	{
		HMD->SendMessage(score);
	}
}

#endif // webhelper
