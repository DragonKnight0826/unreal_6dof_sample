//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "SkyWorthVRController.h"
#include "Kismet/KismetMathLibrary.h"
#include "SkyWorthVRControllerFunctionLibrary.h"
#include "Core/Public/Misc/CoreDelegates.h"
#include "IHeadMountedDisplay.h"
#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/UnrealMemory.h"
#include "sxrApi.h"
#include "HeadMountedDisplay/Public/HeadMountedDisplayFunctionLibrary.h"
#include "ApplicationCore/Public/GenericPlatform/IInputInterface.h"
#include "Kismet/GameplayStatics.h"
////////////////////////////////////////////////////////////////////////////////////////////////////////
/*		_____  ______          _____    __  __ ______
		|  __ \|  ____|   /\   |  __ \  |  \/  |  ____|
		| |__) | |__     /  \  | |  | | | \  / | |__
		|  _  /|  __|   / /\ \ | |  | | | |\/| |  __|
		| | \ \| |____ / ____ \| |__| | | |  | | |____
		|_|  \_\______/_/    \_\_____/  |_|  |_|______|

In an effort to keep things clear - THIS is the only file that should be "adjusting" things from svr controller
space to Unreal - esp things like coordinates, etc. So if you are looking for (or looking to add) functions that
deal with things like that - please make sure they are only here.
*/
////////////////////////////////////////////////////////////////////////////////////////////////////////

#define LOCTEXT_NAMESPACE "SkyWorthVRController"
//-----------------------------------------------------------------------------

uint32_t TempButtonState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER] = {};

uint32_t TempiousTouchState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER] = {};

sxrVector2 TempAxis2D[CONTROLLERS_PER_PLAYER][4] = {};

float TempAxis[CONTROLLERS_PER_PLAYER][8] = {};

class FSkyWorthVRControllerPlugin : public ISkyWorthVRControllerPlugin
{
	virtual void StartupModule() override
	{
		UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRControllerPlugin::StartupModule"));
		IInputDeviceModule::StartupModule();
		FSkyWorthVRController::PreInit();
	}

	virtual TSharedPtr<class IInputDevice> CreateInputDevice(
		const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler) override
	{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS || (PLATFORM_WINDOWS && WINVER > 0x0502)
		UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRControllerPlugin::CreateInputDevice"));

		return MakeShared<FSkyWorthVRController>(InMessageHandler);
#else
		return nullptr;
#endif
	}
};

IMPLEMENT_MODULE(FSkyWorthVRControllerPlugin, SkyWorthVRController)

//-----------------------------------------------------------------------------
FName FSkyWorthVRController::DeviceTypeName(TEXT("SkyWorthVRController"));

//-----------------------------------------------------------------------------
FSkyWorthVRController::FSkyWorthVRController(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
	: bIsTracking(false),
	  MessageHandler(InMessageHandler)
{
	UE_LOG(LogSkyWorthVRController, Log, TEXT("SkyWorthVRController Created"));

	// Register the motion controller! This is the secret sauce of how to get Motion Controllers working natively.
	// You must MANUALLY call IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this) in your implementation!
	// This allows motion controllers to be both piggy-backed off HMD devices which support them, as well as standing alone.
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

	StartTracking();
	for (int i = 0; i < CONTROLLERS_PER_PLAYER; ++i)
	{
		for (int j = 0; j < ESkyWorthVRControllerButton::TotalCount; ++j)
		{
			Touches[i][j] = Buttons[i][j] = FGamepadKeyNames::Type("None");
		}
	}

	// Setup input mappings
	//Touch
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_Left_Menu] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Menu_Touch;
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_X] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_X_Touch;
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_Y] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_Y_Touch;
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_Left_JoyStick] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Touch;
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_Left_Grip] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Grip_Touch;
	Touches[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerTouch::Button_Left_Trigger] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Trigger_Touch;

	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_Right_Menu] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Menu_Touch;
	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_A] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_A_Touch;
	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_B] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_B_Touch;
	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_Right_JoyStick] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Touch;
	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_Right_Grip] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Grip_Touch;
	Touches[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerTouch::Button_Right_Trigger] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Trigger_Touch;

	//Buttons
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_Left_Menu] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Menu;
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_X] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_X;
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_Y] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_Y;
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_Left_JoyStick] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_JoyStick;
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_Left_Grip] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Grip;
	Buttons[static_cast<int32>(EControllerHand::Left)][ESkyWorthVRControllerButton::Button_Left_Trigger] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Trigger;

	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_Right_Menu] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Menu;
	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_A] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_A;
	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_B] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Button_B;
	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_Right_JoyStick] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_JoyStick;
	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_Right_Grip] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Grip;
	Buttons[static_cast<int32>(EControllerHand::Right)][ESkyWorthVRControllerButton::Button_Right_Trigger] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Trigger;

	Buttons[static_cast<int32>(EControllerHand::AnyHand)][ESkyWorthVRControllerHMDButton::Button_Enter] =
		SkyWorthVRControllerKeyNames::SkyWorthXRController_HMD_Enter;

	// Register life cycle delegates
	// See "Multi-Window Lifecycle" https://developer.android.com/guide/topics/ui/multi-window 
	// Instead of handling APP_CMD_RESUME and APP_CMD_PAUSE, handle APP_CMD_START and APP_CMD_STOP
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(
		this, &FSkyWorthVRController::ApplicationStopDelegate);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(
		this, &FSkyWorthVRController::ApplicationStartDelegate);
	//FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FSkyWorthVRController::ApplicationStopDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationWillEnterBackgroundDelegate(), but cover the possibility that only this delegate is called
	//FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FSkyWorthVRController::ApplicationStartDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationHasEnteredForegroundDelegate(), but cover the possibility that only this delegate is called
}

//-----------------------------------------------------------------------------
FSkyWorthVRController::~FSkyWorthVRController()
{
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);

	StopTracking();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::PreInit()
{
	//Add custom keys

	// Warning - any user name longer than 32 chars will not be that visible as it will get cut off...

	// Touch
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Menu_Touch,
	                          LOCTEXT("SkyWorthXRController_Left_Menu_Touch", "SkyWorthXR Controller (L) Touch Menu"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Trigger_Touch,
	                          LOCTEXT("SkyWorthXRController_Left_Trigger_Touch",
	                                  "SkyWorthXR Controller (L) Touch Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Grip_Touch,
	                          LOCTEXT("SkyWorthXRController_Left_Grip_Touch", "SkyWorthXR Controller (L) Touch Grip"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick_Touch,
	                          LOCTEXT("SkyWorthXRController_Left_JoyStick_Touch",
	                                  "SkyWorthXR Controller (L) Touch JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_X_Touch,
	                          LOCTEXT("SkyWorthXRController_Button_X_Touch",
	                                  "SkyWorthXR Controller (L) Touch Button X"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_Y_Touch,
	                          LOCTEXT("SkyWorthXRController_Button_Y_Touch",
	                                  "SkyWorthXR Controller (L) Touch Button Y"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Menu_Touch,
	                          LOCTEXT("SkyWorthXRController_Right_Menu_Touch", "SkyWorthXR Controller (R) Touch Menu"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Trigger_Touch,
	                          LOCTEXT("SkyWorthXRController_Right_Trigger_Touch",
	                                  "SkyWorthXR Controller (R) Touch Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Grip_Touch,
	                          LOCTEXT("SkyWorthXRController_Right_Grip_Touch", "SkyWorthXR Controller (R) Touch Grip"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick_Touch,
	                          LOCTEXT("SkyWorthXRController_Right_JoyStick_Touch",
	                                  "SkyWorthXR Controller (R) Touch JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_A_Touch,
	                          LOCTEXT("SkyWorthXRController_Button_A_Touch",
	                                  "SkyWorthXR Controller (R) Touch Button A"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_B_Touch,
	                          LOCTEXT("SkyWorthXRController_Button_B_Touch",
	                                  "SkyWorthXR Controller (R) Touch Button B"), FKeyDetails::GamepadKey));

#if ENGINE_MINOR_VERSION < 26
	// Axis 1D
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Trigger_Axis,
		LOCTEXT("SkyWorthXRController_Left_Trigger_Axis", "SkyWorthXR Controller (L) Trigger Axis"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Grip_Axis, 
		LOCTEXT("SkyWorthXRController_Left_Grip_Axis", "SkyWorthXR Controller (L) Grip Axis"),
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Trigger_Axis, 
		LOCTEXT("SkyWorthXRController_Right_Trigger_Axis", "SkyWorthXR Controller (R) Trigger Axis"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Grip_Axis,
		LOCTEXT("SkyWorthXRController_Right_Grip_Axis", "SkyWorthXR Controller (R) Grip_Axis"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));

	//Axis 2D
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_X, 
		LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_X", "SkyWorthXR Controller (L) JoyStick Axis X"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_Y, 
		LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_Y", "SkyWorthXR Controller (L) JoyStick Axis Y"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_X,
		LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_X", "SkyWorthXR Controller (R) JoyStick Axis X"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_Y, 
		LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_Y", "SkyWorthXR Controller (R) JoyStick Axis Y"), 
		FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
#else
	// Axis 1D
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Trigger_Axis,
	                          LOCTEXT("SkyWorthXRController_Left_Trigger_Axis",
	                                  "SkyWorthXR Controller (L) Trigger Axis"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Grip_Axis,
	                          LOCTEXT("SkyWorthXRController_Left_Grip_Axis", "SkyWorthXR Controller (L) Grip Axis"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Trigger_Axis,
	                          LOCTEXT("SkyWorthXRController_Right_Trigger_Axis",
	                                  "SkyWorthXR Controller (R) Trigger Axis"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Grip_Axis,
	                          LOCTEXT("SkyWorthXRController_Right_Grip_Axis", "SkyWorthXR Controller (R) Grip_Axis"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));

	//Axis 2D
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_X,
	                          LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_X",
	                                  "SkyWorthXR Controller (L) JoyStick Axis X"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_Y,
	                          LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_Y",
	                                  "SkyWorthXR Controller (L) JoyStick Axis Y"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_X,
	                          LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_X",
	                                  "SkyWorthXR Controller (R) JoyStick Axis X"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_Y,
	                          LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_Y",
	                                  "SkyWorthXR Controller (R) JoyStick Axis Y"),
	                          FKeyDetails::GamepadKey | FKeyDetails::Axis1D));
#endif

	//Buttons
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Menu,
	                          LOCTEXT("SkyWorthXRController_Left_Menu", "SkyWorthXR Controller (L) Menu"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Trigger,
	                          LOCTEXT("SkyWorthXRController_Left_Trigger", "SkyWorthXR Controller (L) Trigger"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_Grip,
	                          LOCTEXT("SkyWorthXRController_Left_Grip", "SkyWorthXR Controller (L) Grip"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Left_JoyStick,
	                          LOCTEXT("SkyWorthXRController_Left_JoyStick", "SkyWorthXR Controller (L) JoyStick"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_X,
	                          LOCTEXT("SkyWorthXRController_Button_X", "SkyWorthXR Controller (L) Button X"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_Y,
	                          LOCTEXT("SkyWorthXRController_Button_Y", "SkyWorthXR Controller (L) Button Y"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Menu,
	                          LOCTEXT("SkyWorthXRController_Right_Menu", "SkyWorthXR Controller (R) Menu"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Trigger,
	                          LOCTEXT("SkyWorthXRController_Right_Trigger", "SkyWorthXR Controller (R) Trigger"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_Grip,
	                          LOCTEXT("SkyWorthXRController_Right_Grip", "SkyWorthXR Controller (R) Grip"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Right_JoyStick,
	                          LOCTEXT("SkyWorthXRController_Right_JoyStick", "SkyWorthXR Controller (R) JoyStick"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_A,
	                          LOCTEXT("SkyWorthXRController_Button_A", "SkyWorthXR Controller (R) Button_A"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_Button_B,
	                          LOCTEXT("SkyWorthXRController_Button_B", "SkyWorthXR Controller (R) Button_B"),
	                          FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SkyWorthVRControllerKeys::SkyWorthXRController_HMD_Enter,
	                          LOCTEXT("SkyWorthXRController_HMD_Enter", "SkyWorthXR Controller (R) Button_Enter"),
	                          FKeyDetails::GamepadKey));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::ApplicationStopDelegate()
{
	UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ApplicationStopDelegate()"));
	StopTracking();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::ApplicationStartDelegate()
{
	UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ApplicationStartDelegate()"));

	StartTracking();
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::StartTracking()
{
	if (bIsTracking)
	{
		UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::StartTracking return"));
		return;
	}

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
			ControllerAndHandIndexToDeviceIdMap[i][j] = sxrControllerStartTracking(NULL);
#endif

			if (ControllerAndHandIndexToDeviceIdMap[i][j] > -1)
			{
				bIsTracking = true; // one or more == isTracking
			}

			FMemory::Memset(&currentState[i][j], 0, sizeof(sxrControllerState));
			FMemory::Memset(&currentCaps[i][j], 0, sizeof(sxrControllerCaps));
			previousButtonState[i][j] = 0;
			previousTouchState[i][j] = 0;
			previousConnectionState[i][j] = kNotInitialized;
		}
	}
	UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::bIsTracking: %d"), bIsTracking);
	if (bIsTracking)
	{
		UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::StartTracking"));
	}
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::StopTracking()
{
	if (!bIsTracking)
	{
		return;
	}

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			if (ControllerAndHandIndexToDeviceIdMap[i][j] > -1)
			{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
				sxrControllerStopTracking(ControllerAndHandIndexToDeviceIdMap[i][j]);
#endif
				ControllerAndHandIndexToDeviceIdMap[i][j] = -1;
			}
		}
	}

	bIsTracking = false;

	UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::StopTracking"));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::PollController()
{
	if (!bIsTracking)
	{
		StartTracking();
	}

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			previousButtonState[i][j] = currentState[i][j].buttonState;
			previousTouchState[i][j] = currentState[i][j].isTouching;
			previousConnectionState[i][j] = currentState[i][j].connectionState;

#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

			if (currentState[i][j].connectionState == sxrControllerConnectionState::kConnected)
			{
				int bytesWritten = sxrControllerQuery(ControllerAndHandIndexToDeviceIdMap[i][j], sxrControllerQueryType::kControllerQueryControllerCaps, &currentCaps[i][j], sizeof(sxrControllerCaps));

				if ( bytesWritten == 0 )
				{
					UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::PollController call for controller caps failed to get any information = zero bytes written"));
				}
				UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::PollController CONNECTED controller %d %d"), i, j);

			}
			else
			{
				//UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::PollController NOT CONNECTED"));
			}

			currentState[i][j] = sxrControllerGetState(ControllerAndHandIndexToDeviceIdMap[i][j], 0);

#endif
			UE_LOG(LogSkyWorthVRController, Log,
			       TEXT(
				       "FSkyWorthVRController::PollController -  SC::GSXR_nativeControllerGetState(caps=%d) for Player[%d], Controller[%d]"
			       ), currentCaps[i][j].caps, i, j);
			UE_LOG(LogSkyWorthVRController, Log,
			       TEXT("FSkyWorthVRController::PollController  position:(x:%f,y:%f,z:%f)"),
			       currentState[i][j].position.x, currentState[i][j].position.y, currentState[i][j].position.z);

			UE_LOG(LogSkyWorthVRController, Log,
			       TEXT("FSkyWorthVRController::PollController  rotation:(x:%f,y:%f,z:%f,w:%f)"),
			       currentState[i][j].rotation.x, currentState[i][j].rotation.y, currentState[i][j].rotation.z,
			       currentState[i][j].rotation.w);
		}
	}
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::ProcessControllerButtons()
{
	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			EControllerHand Hand = static_cast<EControllerHand>(j);
			if (Hand == EControllerHand::AnyHand)
			{
				uint32_t TempState = currentState[static_cast<int32>(ESkyWorthVRControllerHMDButton::Button_Enter)][
					static_cast<int32>(EControllerHand::AnyHand)].buttonState;
				if (previousButtonState[static_cast<int32>(ESkyWorthVRControllerHMDButton::Button_Enter)][static_cast<
						int32>(EControllerHand::AnyHand)] !=
					TempState)
				{
					if (TempState)
					{
						UE_LOG(LogSkyWorthVRController, Log, TEXT("test1"));

						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Button down = %s, HandId = %d, ButtonIndex = %d, mask = %X"),
						       *Buttons[j][ESkyWorthVRControllerHMDButton::Button_Enter].ToString(), j,
						       ESkyWorthVRControllerHMDButton::Button_Enter, 0);
						UE_LOG(LogSkyWorthVRController, Log, TEXT("index j:%d i:%d"), j, i);
						MessageHandler->OnControllerButtonPressed(
							Buttons[j][ESkyWorthVRControllerHMDButton::Button_Enter], 0, false);
					}
					else
					{
						UE_LOG(LogSkyWorthVRController, Log, TEXT("test1"));
						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Button Up = %s, HandId = %d, ButtonIndex = %d, mask = %X"),
						       *Buttons[j][ESkyWorthVRControllerHMDButton::Button_Enter].ToString(), j,
						       ESkyWorthVRControllerHMDButton::Button_Enter, 0);
						UE_LOG(LogSkyWorthVRController, Log, TEXT("index j:%d i:%d"), j, i);
						MessageHandler->OnControllerButtonReleased(
							Buttons[j][ESkyWorthVRControllerHMDButton::Button_Enter], 0, false);
					}
				}
				continue;
			}
			if (IsAvailable(i, Hand))
			{
				for (int32 ButtonIndex = 0; ButtonIndex < static_cast<int32>(ESkyWorthVRControllerButton::TotalCount);
				     ++ButtonIndex)
				{
					ESkyWorthVRControllerButton::Type ButtonType = static_cast<ESkyWorthVRControllerButton::Type>(
						ButtonIndex);

					// Process our known set of buttons
					if (GetButtonDown(i, Hand, ButtonType))
					{
						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Button down = %s, HandId = %d, ButtonIndex = %d, mask = %X"),
						       *Buttons[j][ButtonIndex].ToString(), j, ButtonIndex, (1 << ButtonIndex));
						MessageHandler->OnControllerButtonPressed(Buttons[j][ButtonIndex], 0, false);

						if (Buttons[j][ButtonIndex] == SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Menu)
						{
							ResetStartTimeL = 1.5f;
						}
						else if (Buttons[j][ButtonIndex] ==
							SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Menu)
						{
							ResetStartTimeR = 1.5f;
						}
					}
					else if (GetButtonUp(i, Hand, ButtonType))
					{
						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Button up = %s, HandId = %d, ButtonIndex = %d, mask = %X"),
						       *Buttons[j][ButtonIndex].ToString(), j, ButtonIndex, (1 << ButtonIndex));
						MessageHandler->OnControllerButtonReleased(Buttons[j][ButtonIndex], 0, false);

						if (Buttons[j][ButtonIndex] == SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Menu)
						{
							ResetStartTimeL = -1.f;
						}
						else if (Buttons[j][ButtonIndex] ==
							SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Menu)
						{
							ResetStartTimeR = -1.f;
						}
					}
				}

				for (int32 TouchIndex = 0; TouchIndex < static_cast<int32>(ESkyWorthVRControllerTouch::TotalCount); ++
				     TouchIndex)
				{
					ESkyWorthVRControllerTouch::Type TouchType = static_cast<ESkyWorthVRControllerTouch::Type>(
						TouchIndex);

					// Process our known set of buttons
					if (GetTouchDown(i, Hand, TouchType))
					{
						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Touch down = %s, HandId = %d, TouchIndex = %d, mask = %X"),
						       *Touches[j][TouchIndex].ToString(), j, TouchIndex, (1 << TouchIndex));
						MessageHandler->OnControllerButtonPressed(Touches[j][TouchIndex], 0, false);
					}
					else if (GetTouchUp(i, Hand, TouchType))
					{
						UE_LOG(LogSkyWorthVRController, Log,
						       TEXT("Touch up = %s, HandId = %d, TouchIndex = %d, mask = %X"),
						       *Touches[j][TouchIndex].ToString(), j, TouchIndex, (1 << TouchIndex));
						MessageHandler->OnControllerButtonReleased(Touches[j][TouchIndex], 0, false);
					}
				}


				if (Hand == EControllerHand::Left)
				{
					float Input = GetAxis1D(i, Hand, ESkyWorthVRControllerAxis1D::Trigger);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Trigger_Axis, 0, Input);

					Input = GetAxis1D(i, Hand, ESkyWorthVRControllerAxis1D::Grip);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_Grip_Axis, 0, Input);

					FVector2D Loc = GetAxis2D(i, Hand, ESkyWorthVRControllerAxis2D::JoyStick);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Axis_X, 0, Loc.X);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Axis_Y, 0, Loc.Y);
					UE_LOG(LogSkyWorthVRController, Log, TEXT("JoyStick Left X= %f, Y = %f"), Loc.X, Loc.Y);
				}
				else if (Hand == EControllerHand::Right)
				{
					float Input = GetAxis1D(i, Hand, ESkyWorthVRControllerAxis1D::Trigger);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Trigger_Axis, 0, Input);

					Input = GetAxis1D(i, Hand, ESkyWorthVRControllerAxis1D::Grip);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_Grip_Axis, 0, Input);

					FVector2D Loc = GetAxis2D(i, Hand, ESkyWorthVRControllerAxis2D::JoyStick);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Axis_X, 0, Loc.X);
					MessageHandler->OnControllerAnalog(
						SkyWorthVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Axis_Y, 0, Loc.Y);
					UE_LOG(LogSkyWorthVRController, Log, TEXT("JoyStick Right X= %f, Y = %f"), Loc.X, Loc.Y);
				}
			}
		}
	}
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::ProcessControllerEvents()
{
	// check that we're not going to puke because the array changed size...
	static_assert(kError == 4, "sxrControllerState.connectionState has changed size");

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			sxrControllerConnectionState currentConnectionState = currentState[i][j].connectionState;
			if (previousConnectionState[i][j] != currentConnectionState)
			{
				ESkyWorthVRControllerState NewState;
				switch (currentConnectionState)
				{
				case kDisconnected:
					NewState = ESkyWorthVRControllerState::Disconnected;
				//UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ProcessControllerEvents DISCONNECTED"));
					break;
				case kConnected:
					NewState = ESkyWorthVRControllerState::Connected;
				//UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ProcessControllerEvents CONNECTED"));
					ReadControllerConfigStruct(i, static_cast<EControllerHand>(j));
				// need to wait for Connected before config info is valid
					break;
				case kConnecting:
					NewState = ESkyWorthVRControllerState::Connecting;
				//UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ProcessControllerEvents CONNECTING"));
					break;
				case kError:
					NewState = ESkyWorthVRControllerState::Error;
				//UE_LOG(LogSkyWorthVRController, Log, TEXT("FSkyWorthVRController::ProcessControllerEvents ERROR"));
					break;

				default:
					UE_LOG(LogSkyWorthVRController, Log, TEXT("Connection type not supported: %d"),
					       currentConnectionState);
					NewState = ESkyWorthVRControllerState::Error;
					break;
				}

				USkyWorthVRControllerFunctionLibrary::GetSkyWorthVRControllerEventManager()->
					OnControllerStateChangedDelegate.Broadcast(i, static_cast<EControllerHand>(j), NewState);
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRController::IsAvailable(const int ControllerIndex, const EControllerHand DeviceHand) const
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
	if ((ControllerIndex >= 0 || ControllerIndex < CONTROLLER_PAIRS_MAX) && (int)DeviceHand < CONTROLLERS_PER_PLAYER)
	{
		if (ControllerAndHandIndexToDeviceIdMap[ControllerIndex][ (int32)DeviceHand] >= 0 /*&& 
			SC::GSXR_nativeIsControllerConnect(uint32(DeviceHand))*/)
		{
			return true;
		}
		UE_LOG(LogSkyWorthVRController, Error, TEXT("IsAvailable return False! connectionState = %d"), currentState[ControllerIndex][(int32)DeviceHand].connectionState);
	}
	UE_LOG(LogSkyWorthVRController, Error, TEXT("IsAvailable return False! ControllerIndex = %d, CONTROLLER_PAIRS_MAX = %d, DeviceHand = %d, CONTROLLERS_PER_PLAYER = %d"), ControllerIndex, CONTROLLER_PAIRS_MAX, (int)DeviceHand, CONTROLLERS_PER_PLAYER);
#endif
	return false;
}

//-----------------------------------------------------------------------------
int32_t FSkyWorthVRController::GetBatteryLevel(const int ControllerIndex, const EControllerHand DeviceHand) const
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
		int32_t batteryLevel = -1;
		sxrControllerQuery(controllerHandle, sxrControllerQueryType::kControllerQueryBatteryRemaining, &batteryLevel, sizeof(batteryLevel));
		return batteryLevel;// SC::GSXR_nativeGetControllerBattery((uint32)DeviceHand);
	}

#endif

	return 0;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::ReadControllerConfigStruct(uint32_t ControllerIndex, EControllerHand Hand)
{
	// only need to do this once in app lifetime since I'm assuming that
	// once we see the first controller, any others are the same, and that if
	// app resumes, the hardware that was there first is still there
	static bool IKnowWhatTheControllerHardwareIsNow = false;

	if (IKnowWhatTheControllerHardwareIsNow == true)
	{
		return;
	}

	uint32_t theHand = static_cast<uint32_t>(Hand);

	if (!IsAvailable(ControllerIndex, Hand) || kConnected != (currentState[ControllerIndex][theHand].connectionState))
	{
		return;
	}

	// fill struct
	sxrControllerCaps* caps = &(currentCaps[ControllerIndex][theHand]);
	if (true != GetControllerCaps(ControllerIndex, Hand, *caps))
	{
		UE_LOG(LogSkyWorthVRController, Log,
		       TEXT("FSkyWorthVRController::ReadControllerConfigStruct - failed to get Controller Caps"));
		return;
	}

	// ok do hardware specific stuff

	// 0th bit = Provides Rotation
	// 1st bit = Provides Position;
	// assume that Just Postion == 6DOF e.g. Pos and Rot
	has6DOF[ControllerIndex][theHand] = caps->caps == 0b11 || caps->caps == 0b10;

	// ghetto char to Wchar to make UE4 logging happy
	wchar_t wString1[64] = {0};
	wchar_t wString2[64] = {0};
	for (int k = 0; k < 64 && 0 != caps->deviceManufacturer[k]; ++k)
	{
		wString1[k] = static_cast<wchar_t>((unsigned int)(caps->deviceManufacturer[k]));
	}
	for (int k = 0; k < 64 && 0 != caps->deviceIdentifier[k]; ++k)
	{
		wString2[k] = static_cast<wchar_t>((unsigned int)(caps->deviceIdentifier[k]));
	}

	UE_LOG(LogSkyWorthVRController, Log, TEXT("Controller found - brand:%s, Model:%s, is %s"), wString1, wString2,
	       has6DOF[ControllerIndex][theHand] ? TEXT("6-DOF") : TEXT("3-DOF"));

	IKnowWhatTheControllerHardwareIsNow = true;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRController::GetControllerCaps(const int ControllerIndex, const EControllerHand DeviceHand,
                                                sxrControllerCaps& controllerCaps)
{
	int bytesWritten = 0;
	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][static_cast<int32>(DeviceHand)];
		memset(&controllerCaps, 0, sizeof(controllerCaps));

#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

		// returns bytes written, if 0 then error
		bytesWritten = sxrControllerQuery(controllerHandle, sxrControllerQueryType::kControllerQueryControllerCaps, &controllerCaps, sizeof(controllerCaps));
#endif
	}

	return bytesWritten == 0 ? false : true;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRController::Recenter(const int ControllerIndex, const EControllerHand DeviceHand) const
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
		sxrControllerSendMessage(controllerHandle, sxrControllerMessageType::kControllerMessageRecenter, 0, 0);
		USkyWorthVRControllerFunctionLibrary::GetSkyWorthVRControllerEventManager()->OnControllerRecenteredDelegate.Broadcast(ControllerIndex, DeviceHand);
		return true;
	}

#endif

	return false;
}

//-----------------------------------------------------------------------------
float FSkyWorthVRController::GetWorldToMetersScale() const
{
	float worldScale = 100;

	if (IsInGameThread() && GWorld != nullptr)
	{
		worldScale = GWorld->GetWorldSettings()->WorldToMeters;
	}

	return worldScale;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::Tick(float DeltaTime)
{
	if (ResetStartTimeL > 0.f)
	{
		ResetStartTimeL -= DeltaTime;

		if (ResetStartTimeL <= 0.f)
		{
			UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
			ResetStartTimeL = -1.f;
		}
	}
	if (ResetStartTimeR > 0.f)
	{
		ResetStartTimeR -= DeltaTime;

		if (ResetStartTimeR <= 0.f)
		{
			UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
			ResetStartTimeL = -1.f;
		}
	}
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::SendControllerEvents()
{
	PollController();
	ProcessControllerButtons();
	ProcessControllerEvents();
	//UE_LOG(LogSkyWorthVRController, Log, TEXT("SendControllerEvents"));
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::SetMessageHandler(const TSharedRef<FGenericApplicationMessageHandler>& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRController::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

	// Skip unless this is the left or right large channel, which we consider to be the only SteamVRController feedback channel
	if (ChannelType != FForceFeedbackChannelType::LEFT_LARGE && ChannelType != FForceFeedbackChannelType::RIGHT_LARGE)
	{
		return;
	}
	
	const EControllerHand Hand = (ChannelType == FForceFeedbackChannelType::LEFT_LARGE) ? EControllerHand::Left : EControllerHand::Right;
	if (IsAvailable(ControllerId, Hand))
	{	
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		sxrControllerSendMessage(controllerHandle, sxrControllerMessageType::kControllerMessageVibration, Value, 1.0f);
	}

#endif // SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
void FSkyWorthVRController::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

	EControllerHand Hand = EControllerHand::Left;
	if (IsAvailable(ControllerId, Hand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		sxrControllerSendMessage(controllerHandle, sxrControllerMessageType::kControllerMessageVibration, Values.LeftLarge, 1.0f);
	}

	Hand = EControllerHand::Right;
	if (IsAvailable(ControllerId, Hand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		sxrControllerSendMessage(controllerHandle, sxrControllerMessageType::kControllerMessageVibration, Values.RightLarge, 1.0f);
	}

#endif // SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
bool FSkyWorthVRController::GetControllerOrientationAndPosition(const int32 ControllerIndex,
                                                                  const EControllerHand DeviceHand,
                                                                  FRotator& OutOrientation, FVector& OutPosition,
                                                                  float WorldToMetersScale) const
{
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		OutPosition = GetPosition(ControllerIndex, DeviceHand) * WorldToMetersScale;
		//OutOrientation = UKismetMathLibrary::ComposeRotators(FRotator(0.f, -90.f, 45.f),GetOrientation(ControllerIndex, DeviceHand).Rotator());
		OutOrientation = GetOrientation(ControllerIndex, DeviceHand).Rotator();
		FVector euler = OutOrientation.Euler();
		UE_LOG(LogSkyWorthVRController, Log, TEXT("Controller IDX=%d, Hand=%s Raw O&P=[%f,%f,%f] [%f,%f,%f,]"), ControllerIndex,
			DeviceHand == EControllerHand::Left ? TEXT("Left") :
				DeviceHand == EControllerHand::Right ? TEXT("Right") : TEXT("Unknown"),
				euler.X, euler.Y, euler.Z, OutPosition.X, OutPosition.Y, OutPosition.Z);

		return true;
	}

#endif

	UE_LOG(LogSkyWorthVRController, Log, TEXT("Controller IDX=%d Not Found"), ControllerIndex);
	return false;
}

//-----------------------------------------------------------------------------
ETrackingStatus FSkyWorthVRController::GetControllerTrackingStatus(const int32 ControllerIndex,
                                                                     EControllerHand DeviceHand) const
{
	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		return ETrackingStatus::Tracked;
	}

	return ETrackingStatus::NotTracked;
}

bool FSkyWorthVRController::SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude,
                                                   float Time)
{
	if (IsAvailable(0, Hand))
	{
		const uint32_t frequency1 = UKismetMathLibrary::FCeil(UKismetMathLibrary::FClamp(Frequency, 0.f, 1.f) * 15.f);
		const auto amplitude1 = UKismetMathLibrary::FCeil(UKismetMathLibrary::FClamp(Amplitude, 0.f, 1.f) * 15.f);
		const auto time1 = static_cast<uint32_t>(UKismetMathLibrary::FClamp(Time, 0.f, 3.f) * 5.f + .5f);

		uint32_t data;
		data = (Hand == EControllerHand::Left ? 0 : 1);
		data |= Open << 2;
		data |= time1 << 3;
		data |= frequency1 << 8;
		data |= amplitude1 << 12;
#if SkyWorthVRCONTROLLER_SUPPORTED_PLATFORMS
		sxrSetControllerVibrate(data);
#endif
		UE_LOG(LogSkyWorthVRController, Log,
		       TEXT("ControllerVibrate lr = %d, Open = %d, Frequency = %f, Amplitude = %f, Time = %f"),
		       (Hand == EControllerHand::Left ? 0 : 1), Open, Frequency, Amplitude, Time);
		return true;
	}
	return false;
}

#undef LOCTEXT_NAMESPACE
