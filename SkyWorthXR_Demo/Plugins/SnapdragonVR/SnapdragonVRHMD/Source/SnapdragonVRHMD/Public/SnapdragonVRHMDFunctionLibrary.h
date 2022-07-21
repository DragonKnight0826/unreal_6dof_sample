//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Classes/SnapdragonVRHMDEventManager.h"
//#include "SnapdragonVRHMD.h"
#include "SnapdragonVRHMDFunctionLibrary.generated.h"


extern int32 EyeTrackingEnabled;


UENUM(BlueprintType)
enum class ESnapdragonVRHMD_Eye : uint8
{
	LeftEye = 0,
	RightEye,
	AverageEye
};


UCLASS()
class USnapdragonVRHMDFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status) TEnumAsByte<ESnapdragonVRHMD_Eye> Eye;


	/**
	* Sets Snapdragon VR CPU and GPU performance Levels
	*
	* @param CpuLevel			(in) 0 = kPerfSystem = system-defined; 1 = kPerfMinimum = Minimum performance level (default 30-50% of max frequency); 2 = kPerfMedium = Medium performance level (default 51-80% of max frequency), 3 = kPerfMaximum = Maximum performance level (default 81-100% of max frequency)
	* @param GpuLevel			(in) 0 = kPerfSystem = system-defined; 1 = kPerfMinimum = Minimum performance level (default 30-50% of max frequency); 2 = kPerfMedium = Medium performance level (default 51-80% of max frequency), 3 = kPerfMaximum = Maximum performance level (default 81-100% of max frequency)
	*/
	UFUNCTION(BlueprintCallable, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR"))
		static void SetCpuAndGpuLevelsSVR(const int32 CpuLevel, const int32 GpuLevel);

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR"))
		static USnapdragonVRHMDEventManager* GetSnapdragonVRHMDEventManager();

#pragma region Eyetracking
	// Eye Tracking

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking", Tooltip = "Gaze Direction in World Coordinates"))
		static void GetEyeGazeDirection(FVector& OutDirection, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking", Tooltip = "Direction and Orientation in World Coordinates"))
		static void GetEyeOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotation, bool& Success);

//	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking", Tooltip = "Direction and Orientation of current pose in World coordinate system"))
//		static void GetRelativeEyePose(FVector& OutPosition, FQuat& OutOrientation, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking"))
		static void GetEyeOpenness(ESnapdragonVRHMD_Eye WhichEye, float& Openness, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking"))
		static void GetPupilDialation(ESnapdragonVRHMD_Eye WhichEye, float& Openness, bool& Success);

//	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR Eyetracking"))
//		static void IsEyetrackingEnabled(bool& IsEnabled);

#pragma endregion EyeTracking

#pragma region HMD
	// HMD 

	UFUNCTION(BlueprintPure, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR HMD", Tooltip = "Direction and Orientation/Rotator of current head pose in World coordinate system"))
		static void GetHeadOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotator, bool& Success);

//	UFUNCTION(BlueprintCallable, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR HMD", Tooltip = "Reset the HDM Orientation, AroundVerticalAxis = true then just around the verical axis (no tilt)"))
	UFUNCTION(BlueprintCallable, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR HMD", Tooltip = "Recenter the HDM's Orientation"))
		static void RecenterOrientation( );

	UFUNCTION(BlueprintCallable, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR HMD", Tooltip = "Recenter the HMD's position"))
		static void RecenterPosition( );

#pragma endregion HMD

#pragma endregion Foveation
	// FOVEATION

	UFUNCTION(BlueprintPure, Category = "Temporal|Foveation|SnapdragonXR", meta = (DisplayName = "is Temporal Foveation Enabled?", Keywords = "Foveation"))
		static bool isTemporalFoveationEnabled();

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "is Texture Foveation Enabled?", Keywords = "Foveation"))
		static bool isTextureFoveationEnabled();

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "is Texture Foveation Focus Enabled?", Keywords = "Foveation"))
		static bool isTextureFoveationFocusEnabled();


	UFUNCTION(BlueprintCallable, Category = "Temporal|Foveation|SnapdragonXR", meta = (DisplayName = "enable Temporal Foveation", Keywords = "Foveation"))
		static void enableTemporalFoveation(bool t);

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "enable Texture Foveation", Keywords = "Foveation"))
		static void enableTextureFoveation(bool t);

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "enable Texture Foveation Focus", Keywords = "Foveation"))
		static void enableTextureFoveationFocus(bool t);


	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Foveation Gain", Keywords = "Foveation"))
		static void getTextureFoveationGain(float& x, float& y);


	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Foveation Gain", Keywords = "Foveation"))
		static void setTextureFoveationGain(float  x, float y);

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Foveation Area", Keywords = "Foveation"))
		static float getTextureFoveationArea();

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Foveation Area", Keywords = "Foveation"))
		static void setTextureFoveationArea(float  x);

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Minimum Pixel Density", Keywords = "Foveation"))
		static float getTextureMinimumPixelDensity();

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Minimum Pixel Density", Keywords = "Foveation"))
		static void setTextureMinimumPixelDensity(float  x);



	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Foveation Focus Amplitude", Keywords = "Foveation"))
		static float getTextureFoveationFocusAmplitude();

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Foveation Focus Amplitude", Keywords = "Foveation"))
		static void setTextureFoveationFocusAmplitude(float  x);

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Foveation Focus Frequency", Keywords = "Foveation"))
		static float getTextureFoveationFocusFrequency();

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Foveation Focus Frequency", Keywords = "Foveation"))
		static void setTextureFoveationFocusFrequency(float  x);

	UFUNCTION(BlueprintPure, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "get Texture Foveation Focus Frequency Rho", Keywords = "Foveation"))
		static float getTextureFoveationFocusFrequencyRho();

	UFUNCTION(BlueprintCallable, Category = "Texture|Foveation|SnapdragonXR", meta = (DisplayName = "set Texture Foveation Focus Frequency Rho", Keywords = "Foveation"))
		static void setTextureFoveationFocusFrequencyRho(float  Rho);

#pragma endregion HMD

	// eyetracking

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "get average gaze direction", Keywords = "Eyetracking"))
		static void getAverageGazeDirection(float& x, float& y);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "get Texture Eyetracking Marker Radius", Keywords = "Eyetracking"))
		static float getEyetrackingMarkerRadius();

	UFUNCTION(BlueprintCallable, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "set Texture Eyetracking Marker Radius", Keywords = "Eyetracking"))
		static void setEyetrackingMarkerRadius(float  x);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "is EyetrackingMarker enabled", Keywords = "Eyetracking"))
		static bool isEyetrackingMarkerEnabled();

	UFUNCTION(BlueprintCallable, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "enable Eyetracking Marker?", Keywords = "Eyetracking"))
		static void enableEyetrackingMarker(bool v);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SnapdragonXR", meta = (DisplayName = "is Eyetracking enabled", Keywords = "Eyetracking"))
		static bool isEyetrackingEnabled();

// Webhelper
//	UFUNCTION(BlueprintCallable, Category = "SnapdragonVR", meta = (Keywords = "SnapdragonVR"))
//		static void SendMessage(const int32 score);
};
