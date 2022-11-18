//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "Kismet/BlueprintFunctionLibrary.h"
#include "IMotionController.h"
#include "SkyWorthVRControllerEventManager.h"
#include "SkyWorthVRControllerFunctionLibrary.generated.h"



/**
* SkyWorth VR Controller Extensions Function Library
*/
UCLASS()
class USkyWorthVRControllerFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_UCLASS_BODY()

public:

	UFUNCTION(BlueprintPure, Category = "SkyWorthVRController")
		static bool GetControllerOrientationAndPosition(int32 ControllerIndex, EControllerHand Hand, FRotator& OutOrientation, FVector& OutPosition);

	UFUNCTION(BlueprintPure, Category = "SkyWorthVRController")
		static bool GetControllerBatteryLevel(int32 ControllerIndex, EControllerHand Hand, int32 & OutBatteryLevel);

	UFUNCTION(BlueprintCallable, Category = "SkyWorthVRController")
		static bool SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude, float Time);

	UFUNCTION(BlueprintCallable, Category = "SkyWorthVRController")
		static bool Recenter(int32 ControllerIndex, EControllerHand Hand);

	UFUNCTION(BlueprintCallable, Category = "SkyWorthVRController")
		static ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, EControllerHand DeviceHand);
	UFUNCTION(BlueprintCallable, Category = "SkyWorthVRController", meta = (Keywords = "SkyWorthVRController"))
		static USkyWorthVRControllerEventManager* GetSkyWorthVRControllerEventManager();
};
