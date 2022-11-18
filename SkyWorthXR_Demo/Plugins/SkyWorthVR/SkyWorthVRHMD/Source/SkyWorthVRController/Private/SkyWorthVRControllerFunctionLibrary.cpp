//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "SkyWorthVRControllerFunctionLibrary.h"
#include "SkyWorthVRController.h"
#include "kismet/KismetMathLibrary.h"

//-----------------------------------------------------------------------------
USkyWorthVRControllerFunctionLibrary::USkyWorthVRControllerFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//-----------------------------------------------------------------------------
FSkyWorthVRController* GetSkyWorthVRController()
{
	TArray<IMotionController*> MotionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	for (auto MotionController : MotionControllers)
	{
		if (MotionController != nullptr && MotionController->GetMotionControllerDeviceTypeName() == FSkyWorthVRController::DeviceTypeName)
		{
			return static_cast<FSkyWorthVRController*>(MotionController);
		}
	}

	return nullptr;
}
//-----------------------------------------------------------------------------
bool USkyWorthVRControllerFunctionLibrary::GetControllerOrientationAndPosition(int32 ControllerIndex, EControllerHand Hand, FRotator& OutOrientation, FVector& OutPosition)
{
	bool Result = false;

	FSkyWorthVRController* SkyWorthVRController = GetSkyWorthVRController();
	if (SkyWorthVRController)
	{
		Result = SkyWorthVRController->GetControllerOrientationAndPosition(ControllerIndex, Hand, OutOrientation, OutPosition, SkyWorthVRController->GetWorldToMetersScale());
		if (Result)
		{
			FVector euler = OutOrientation.Euler();
			UE_LOG(LogSkyWorthVRController, Log, TEXT("USkyWorthVRControllerFunctionLibrary::GetControllerOrientationAndPosition Controller IDX=%d, Hand=%s Raw O&P=[%f,%f,%f] [%f,%f,%f,]"), ControllerIndex,
				Hand == EControllerHand::Left ? TEXT("Left") :
				Hand == EControllerHand::Right ? TEXT("Right") : TEXT("Unknown"),
				euler.X, euler.Y, euler.Z, OutPosition.X, OutPosition.Y, OutPosition.Z);
		}
		else
		{
			Result = false;
			UE_LOG(LogSkyWorthVRController, Log, TEXT("USkyWorthVRControllerFunctionLibrary::GetControllerOrientationAndPosition failed"));
		}

	}

	return Result;

}

//-----------------------------------------------------------------------------
ETrackingStatus USkyWorthVRControllerFunctionLibrary::GetControllerTrackingStatus(int32 ControllerIndex, EControllerHand Hand)
{
	return	nullptr != GetSkyWorthVRController() ?
		GetSkyWorthVRController()->GetControllerTrackingStatus(ControllerIndex, Hand) :
		ETrackingStatus::NotTracked;
}

//-----------------------------------------------------------------------------
bool USkyWorthVRControllerFunctionLibrary::GetControllerBatteryLevel(int32 ControllerIndex, EControllerHand Hand, int32 & OutBatteryLevel)
{
	bool Result = false;

	FSkyWorthVRController* SkyWorthVRController = GetSkyWorthVRController();
	if (SkyWorthVRController)
	{
		OutBatteryLevel = SkyWorthVRController->GetBatteryLevel(ControllerIndex, Hand);

		if (OutBatteryLevel > 0)
		{
			Result = true;
		}
	}

	return Result;
}

//-----------------------------------------------------------------------------
bool USkyWorthVRControllerFunctionLibrary::Recenter(int32 ControllerIndex, EControllerHand Hand)
{
	bool Result = false;

	FSkyWorthVRController* SkyWorthVRController = GetSkyWorthVRController();
	if (SkyWorthVRController)
	{
		Result = SkyWorthVRController->Recenter(ControllerIndex, Hand);
	}

	return Result;
}

bool USkyWorthVRControllerFunctionLibrary::SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude, float Time)
{
	bool Result = false;

	FSkyWorthVRController* SkyWorthVRController = GetSkyWorthVRController();
	if (SkyWorthVRController)
	{
		Result = SkyWorthVRController->SetControllerVibrate(
			Hand,
			Open,
			UKismetMathLibrary::FClamp(Frequency,0.f,1.f),
			UKismetMathLibrary::FClamp(Amplitude, 0.f, 1.f),
			UKismetMathLibrary::FClamp(Time, 0.f, 3.f));
	}

	return Result;
}
//-----------------------------------------------------------------------------
USkyWorthVRControllerEventManager* USkyWorthVRControllerFunctionLibrary::GetSkyWorthVRControllerEventManager()
{
	return USkyWorthVRControllerEventManager::GetInstance();
}

