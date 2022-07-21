//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "SnapdragonVRControllerFunctionLibrary.h"
#include "SnapdragonVRControllerPrivate.h"
#include "SnapdragonVRController.h"
#include "kismet/KismetMathLibrary.h"

//-----------------------------------------------------------------------------
USnapdragonVRControllerFunctionLibrary::USnapdragonVRControllerFunctionLibrary(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
}

//-----------------------------------------------------------------------------
FSnapdragonVRController* GetSnapdragonVRController()
{
	TArray<IMotionController*> MotionControllers = IModularFeatures::Get().GetModularFeatureImplementations<IMotionController>(IMotionController::GetModularFeatureName());
	for (auto MotionController : MotionControllers)
	{
		if (MotionController != nullptr && MotionController->GetMotionControllerDeviceTypeName() == FSnapdragonVRController::DeviceTypeName)
		{
			return static_cast<FSnapdragonVRController*>(MotionController);
		}
	}

	return nullptr;
}
//-----------------------------------------------------------------------------
bool USnapdragonVRControllerFunctionLibrary::GetControllerOrientationAndPosition(int32 ControllerIndex, EControllerHand Hand, FRotator& OutOrientation, FVector& OutPosition)
{
	bool Result = false;

	FSnapdragonVRController* SnapdragonVRController = GetSnapdragonVRController();
	if (SnapdragonVRController)
	{
		Result = SnapdragonVRController->GetControllerOrientationAndPosition(ControllerIndex, Hand, OutOrientation, OutPosition, SnapdragonVRController->GetWorldToMetersScale());
		if (Result)
		{
			FVector euler = OutOrientation.Euler();
			UE_LOG(LogSnapdragonVRController, Log, TEXT("USnapdragonVRControllerFunctionLibrary::GetControllerOrientationAndPosition Controller IDX=%d, Hand=%s Raw O&P=[%f,%f,%f] [%f,%f,%f,]"), ControllerIndex,
				Hand == EControllerHand::Left ? TEXT("Left") :
				Hand == EControllerHand::Right ? TEXT("Right") : TEXT("Unknown"),
				euler.X, euler.Y, euler.Z, OutPosition.X, OutPosition.Y, OutPosition.Z);
		}
		else
		{
			Result = false;
			UE_LOG(LogSnapdragonVRController, Log, TEXT("USnapdragonVRControllerFunctionLibrary::GetControllerOrientationAndPosition failed"));
		}

	}

	return Result;

}

//-----------------------------------------------------------------------------
ETrackingStatus USnapdragonVRControllerFunctionLibrary::GetControllerTrackingStatus(int32 ControllerIndex, EControllerHand Hand)
{
	return	nullptr != GetSnapdragonVRController() ?
		GetSnapdragonVRController()->GetControllerTrackingStatus(ControllerIndex, Hand) :
		ETrackingStatus::NotTracked;
}

//-----------------------------------------------------------------------------
bool USnapdragonVRControllerFunctionLibrary::GetControllerBatteryLevel(int32 ControllerIndex, EControllerHand Hand, int32 & OutBatteryLevel)
{
	bool Result = false;

	FSnapdragonVRController* SnapdragonVRController = GetSnapdragonVRController();
	if (SnapdragonVRController)
	{
		OutBatteryLevel = SnapdragonVRController->GetBatteryLevel(ControllerIndex, Hand);

		if (OutBatteryLevel > 0)
		{
			Result = true;
		}
	}

	return Result;
}

//-----------------------------------------------------------------------------
bool USnapdragonVRControllerFunctionLibrary::Recenter(int32 ControllerIndex, EControllerHand Hand)
{
	bool Result = false;

	FSnapdragonVRController* SnapdragonVRController = GetSnapdragonVRController();
	if (SnapdragonVRController)
	{
		Result = SnapdragonVRController->Recenter(ControllerIndex, Hand);
	}

	return Result;
}

bool USnapdragonVRControllerFunctionLibrary::SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude, float Time)
{
	bool Result = false;

	FSnapdragonVRController* SnapdragonVRController = GetSnapdragonVRController();
	if (SnapdragonVRController)
	{
		Result = SnapdragonVRController->SetControllerVibrate(
			Hand,
			Open,
			UKismetMathLibrary::FClamp(Frequency,0.f,1.f),
			UKismetMathLibrary::FClamp(Amplitude, 0.f, 1.f),
			UKismetMathLibrary::FClamp(Time, 0.f, 3.f));
	}

	return Result;
}
//-----------------------------------------------------------------------------
USnapdragonVRControllerEventManager* USnapdragonVRControllerFunctionLibrary::GetSnapdragonVRControllerEventManager()
{
	return USnapdragonVRControllerEventManager::GetInstance();
}

