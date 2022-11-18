//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "../Classes/SkyWorthVRHMDEventManager.h"
//#include "SkyWorthVRHMD.h"
#include "SkyWorthVRHMDFunctionLibrary.generated.h"


extern int32 EyeTrackingEnabled;


UENUM(BlueprintType)
enum class ESkyWorthVRHMD_Eye : uint8
{
	LeftEye = 0,
	RightEye,
	AverageEye
};

UENUM(BlueprintType)
enum class ESkyWorthXRFoveationLevel :uint8
{
	Low,
	Medium,
	High,
	TopHigh
};

UCLASS()
class USkyWorthVRHMDFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:


	//UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Status) TEnumAsByte<ESkyWorthVRHMD_Eye> Eye;


	/**
	* Sets SkyWorth VR CPU and GPU performance Levels
	*
	* @param CpuLevel			(in) 0 = kPerfSystem = system-defined; 1 = kPerfMinimum = Minimum performance level (default 30-50% of max frequency); 2 = kPerfMedium = Medium performance level (default 51-80% of max frequency), 3 = kPerfMaximum = Maximum performance level (default 81-100% of max frequency)
	* @param GpuLevel			(in) 0 = kPerfSystem = system-defined; 1 = kPerfMinimum = Minimum performance level (default 30-50% of max frequency); 2 = kPerfMedium = Medium performance level (default 51-80% of max frequency), 3 = kPerfMaximum = Maximum performance level (default 81-100% of max frequency)
	*/
	UFUNCTION(BlueprintCallable, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR"))
		static void SetCpuAndGpuLevelsSVR(const int32 CpuLevel, const int32 GpuLevel);

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR"))
		static USkyWorthVRHMDEventManager* GetSkyWorthVRHMDEventManager();

#pragma region Eyetracking
	// Eye Tracking

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking", Tooltip = "Gaze Direction in World Coordinates"))
		static void GetEyeGazeDirection(FVector& OutDirection, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking", Tooltip = "Direction and Orientation in World Coordinates"))
		static void GetEyeOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotation, bool& Success);

//	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking", Tooltip = "Direction and Orientation of current pose in World coordinate system"))
//		static void GetRelativeEyePose(FVector& OutPosition, FQuat& OutOrientation, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking"))
		static void GetEyeOpenness(ESkyWorthVRHMD_Eye WhichEye, float& Openness, bool& Success);

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking"))
		static void GetPupilDialation(ESkyWorthVRHMD_Eye WhichEye, float& Openness, bool& Success);

//	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR Eyetracking"))
//		static void IsEyetrackingEnabled(bool& IsEnabled);

#pragma endregion EyeTracking

#pragma region HMD
	// HMD 

	UFUNCTION(BlueprintPure, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR HMD", Tooltip = "Direction and Orientation/Rotator of current head pose in World coordinate system"))
		static void GetHeadOrientationAndPosition(FVector& OutPosition, FQuat& OutOrientation, FRotator& OutRotator, bool& Success);

//	UFUNCTION(BlueprintCallable, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR HMD", Tooltip = "Reset the HDM Orientation, AroundVerticalAxis = true then just around the verical axis (no tilt)"))
	UFUNCTION(BlueprintCallable, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR HMD", Tooltip = "Recenter the HDM's Orientation"))
		static void RecenterOrientation( );

	UFUNCTION(BlueprintCallable, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR HMD", Tooltip = "Recenter the HMD's position"))
		static void RecenterPosition( );

#pragma endregion HMD

#pragma region Foveation
	

#pragma endregion Foveation

	// eyetracking

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "get average gaze direction", Keywords = "Eyetracking"))
		static void getAverageGazeDirection(float& x, float& y);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "get Texture Eyetracking Marker Radius", Keywords = "Eyetracking"))
		static float getEyetrackingMarkerRadius();

	UFUNCTION(BlueprintCallable, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "set Texture Eyetracking Marker Radius", Keywords = "Eyetracking"))
		static void setEyetrackingMarkerRadius(float  x);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "is EyetrackingMarker enabled", Keywords = "Eyetracking"))
		static bool isEyetrackingMarkerEnabled();

	UFUNCTION(BlueprintCallable, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "enable Eyetracking Marker?", Keywords = "Eyetracking"))
		static void enableEyetrackingMarker(bool v);

	UFUNCTION(BlueprintPure, Category = "Eyetracking|SkyWorthXR", meta = (DisplayName = "is Eyetracking enabled", Keywords = "Eyetracking"))
		static bool isEyetrackingEnabled();

// Webhelper
//	UFUNCTION(BlueprintCallable, Category = "SkyWorthVR", meta = (Keywords = "SkyWorthVR"))
//		static void SendMessage(const int32 score);
};
