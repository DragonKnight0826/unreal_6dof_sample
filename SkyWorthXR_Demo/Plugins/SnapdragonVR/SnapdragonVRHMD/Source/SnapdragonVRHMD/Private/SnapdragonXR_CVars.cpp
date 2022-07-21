//=============================================================================
//
//                  Copyright (c) 2019 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#define DEFINE_CVAR 1

#include "SnapdragonXR_CVars.h"
#include "SnapdragonVRHMD.h"
#include "Core/Public/HAL/IConsoleManager.h"


// pass in a string (will get written over), and a pointer to a vector of floats to get written into
// plus the max number to write into
// returns the number actually written
uint32 CVars::ParseIntoFloats(FString& TheString, float* loc, const uint32 maxNumber)// const FString& TheRotation)
{
	TCHAR* Stream = (TCHAR*)(*TheString); // cast away const-ness, strtok needs to mangle the string

	UE_LOG(LogSVR, Log, TEXT("------------>>>>>>>>>>>>>>>>> ParseIntoFloats string is %s"), Stream);

	// the delimiters
	const TCHAR* delimiters = TEXT(" ,");  // space comma

	// mangle string into tokens
	TCHAR *ContextStr = nullptr;
	TCHAR *strToken = FCString::Strtok(Stream, delimiters, &ContextStr);
	uint32 numberWritten = 0;

	// while we have a token, keep churning them into floats
	while (strToken && numberWritten < maxNumber)
	{
		UE_LOG(LogSVR, Log, TEXT("------------>>>>>>>>>>>>>>>>> ParseIntoFloats token %d is %s"), numberWritten +1, strToken );

		*loc++ = FCString::Atof(strToken); // ascii to float
		strToken = FCString::Strtok(nullptr, delimiters, &ContextStr); // next token
		numberWritten++;
	}

	if (numberWritten != maxNumber)
	{
		UE_LOG(LogSVR, Log, TEXT("-----CVars::ParseIntoFloats notice: expected %d  but found %d"), maxNumber, numberWritten);
	}

	return numberWritten;
}

#pragma region Foveation

static FAutoConsoleVariableRef CVarSVRTextureFoveationArea(
	TEXT("r.SVR.TextureFoveationArea"),
	CVars::TextureFoveationArea,
	TEXT("Minimum size of the fovea region, the area before the quality starts to fall off.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationGainX(
	TEXT("r.SVR.TextureFoveationGainX"),
	CVars::TextureFoveationGainX,
	TEXT("Controls how quickly the quality falls off as you get further away from the focal point on the X axis.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationGainY(
	TEXT("r.SVR.TextureFoveationGainY"),
	CVars::TextureFoveationGainY,
	TEXT("Controls how quickly the quality falls off as you get further away from the focal point on the Y axis.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationMinPixelDensity(
	TEXT("r.SVR.TextureFoveationMinPixelDensity"),
	CVars::TextureFoveationMinPixelDensity,
	TEXT("Minimum bin pixel density regardless of gain and fovea region, for example\n")
	TEXT("0.25 will ensure no bin gets renderer at less than quarter res.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTemporalFoveationEnabled(
	TEXT("r.SVR.TemporalFoveationEnabled"),
	CVars::TemporalFoveationEnabled,
	TEXT("0: Disable temporal foveation (default).\n")
	TEXT("1: Pass texture ID from previous swapchain image to the driver to enable TAA with foveation\n"),
	ECVF_Default
);


//---------------
// Debug options
static FAutoConsoleVariableRef CVarSVRTextureFoveationFocusEnabled(
	TEXT("r.SVR.TextureFoveationFocusEnabled"),
	CVars::TextureFoveationFocusEnabled,
	TEXT("0: Disable gaze simulation (default).\n")
	TEXT("1: Enable gaze simulation by moving focal point each frame according to the following equation:\n")
	TEXT("	FocalPoint = |sin(Time * FrequencyRho)| * (cos(Time * Frequency) * Amplitude, sin(Time * Frequency) * Amplitude)\n")
	TEXT("The pattern formed is an Archimedean spiralFrequencyRho"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationFocusAmplitude(
	TEXT("r.SVR.TextureFoveationFocusAmplitude"),
	CVars::TextureFoveationFocusAmplitude,
	TEXT("How far from the center of each eye buffer the focal point is.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationFocusFrequency(
	TEXT("r.SVR.TextureFoveationFocusFrequency"),
	CVars::TextureFoveationFocusFrequency,
	TEXT("How fast the focal point completes a revolution around the center of the eye buffer.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRTextureFoveationFocusFrequencyRho(
	TEXT("r.SVR.TextureFoveationFocusFrequencyRho"),
	CVars::TextureFoveationFocusFrequencyRho,
	TEXT("If set to anything but zero, the focal point pattern will be a spiral instead of a circle.\n")
	TEXT("Default is 0.f"),
	ECVF_Default
);



#pragma endregion Foveation

#pragma region Eyetracking

static FAutoConsoleVariableRef CVarSVREyeTrackingEnabled(
	TEXT("r.SVR.EyeTrackingEnabled"),
	CVars::EyeTrackingEnabled,
	TEXT("0: Disable eye tracking.\n")
	TEXT("1: Get eye pose from sxr every frame, use resulting focal point in texture foveation if enabled (default).\n"),
	ECVF_Default
);


static FAutoConsoleVariableRef CVarSVRAverageGazeDirectionX(
	TEXT("r.SVR.AverageGazeDirectionX"),
	CVars::AverageGazeDirectionX,
	TEXT("0: Use different X values for each eye (default).\n")
	TEXT("1: Use combined eye gaze data for the X coordinate of the gaze direction\n")
	TEXT("2: Use an average of the left and right eye data for the X coordinate of the gaze direction\n"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRAverageGazeDirectionY(
	TEXT("r.SVR.AverageGazeDirectionY"),
	CVars::AverageGazeDirectionY,
	TEXT("0: Use different Y values for each eye (default).\n")
	TEXT("1: Use combined eye gaze data for the Y coordinate of the gaze direction\n")
	TEXT("2: Use an average of the left and right eye data for the Y coordinate of the gaze direction\n"),
	ECVF_Default
);


static FAutoConsoleVariableRef CVarSVREyeMarkerEnabled(
	TEXT("r.SVR.EyeMarkerEnabled"),
	CVars::EyeMarkerEnabled,
	TEXT("0: Don't draw eye marker (default).\n")
	TEXT("1: Draw a quad to visualize the current focal point (either from TextureFoveationFocus or eye tracking)\n"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVREyeMarkerRadius(
	TEXT("r.SVR.EyeMarkerRadius"),
	CVars::EyeMarkerRadius,
	TEXT("The size of the reticle layer.\n")
	TEXT("Default is 0.025f"),
	ECVF_Default
);

static FAutoConsoleVariableRef CVarSVRLogEyePoseData(
	TEXT("r.SVR.LogEyePoseData"),
	CVars::LogEyePoseData,
	TEXT("0: Don't log eye pose data (default).\n")
	TEXT("1: If eye tracking and either foveation or gaze reticle are enabled, logs the gaze vector and status from each eye\n"),
	ECVF_Default
);

#pragma endregion Eyetracking

static FAutoConsoleVariableRef CVarSVRNumSwapchainImages(
	TEXT("r.SVR.NumSwapchainImages"),
	CVars::GNumSwapchainImages,
	TEXT("Number of images in swapchain. FSnapdragonVRHMDCustomPresent passes a different one of these to UE4 for rendering each frame.\n")
	TEXT(" 3 is the default"),
	ECVF_Default
);


// can't use FAutoConsoleVariableRef because it only supports int32 and float
static TAutoConsoleVariable<FString> SXRViewFrustumLeft_string(
	TEXT("r.SXR.ViewFrustumLeft"),
	TEXT("0.3,0.2 0.1 0.1 0.2 0.3 0.4"), // default value
	TEXT(" The first three values define the position, the next three define the rotation.\n")
	TEXT("Example:\n")
	TEXT(" FreezeAtPosition 2819.5520 416.2633 75.1500 65378 -25879 0"),
	ECVF_Default);
