//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "Delegates/Delegate.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "SkyWorthVRHMDEventManager.generated.h"

UENUM(BlueprintType)
enum class ESkyWorthVRSdkServiceEventType : uint8
{
	EventSdkServiceStarting,
	EventSdkServiceStarted,
	EventSdkServiceStopped
};

UENUM(BlueprintType)
enum class ESkyWorthVRModeEventType : uint8
{
	EventVrModeStarted,
	EventVrModeStopping,
	EventVrModeStopped
};

UENUM(BlueprintType)
enum class ESkyWorthVRSensorEventType : uint8
{
	EventSensorError,
	EventMagnometerUncalibrated
};

UENUM(BlueprintType)
enum class ESkyWorthVRThermalLevel : uint8
{
	Safe,
	Level1,
	Level2,
	Level3,
	Critical
};


UENUM(BlueprintType)
enum class ESkyWorthVRThermalZone : uint8
{
	Cpu,
	Gpu,
	Skin
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkyWorthVRSdkServiceDelegate, ESkyWorthVRSdkServiceEventType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkyWorthVRThermalDelegate, ESkyWorthVRThermalZone, Zone, ESkyWorthVRThermalLevel, Level);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkyWorthVRModeDelegate, ESkyWorthVRModeEventType, Type);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FSkyWorthVRSensorDelegate, ESkyWorthVRSensorEventType, Type);

/**
* SkyWorthVRHMD Extensions Function Library
*/
UCLASS()
class USkyWorthVRHMDEventManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRSdkServiceDelegate OnSkyWorthVRSdkServiceDelegate;

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRThermalDelegate OnSkyWorthVRThermalDelegate;

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRModeDelegate OnSkyWorthVRModeDelegate;

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRSensorDelegate OnSkyWorthVRSensorDelegate;

public:
	static USkyWorthVRHMDEventManager* GetInstance();
};