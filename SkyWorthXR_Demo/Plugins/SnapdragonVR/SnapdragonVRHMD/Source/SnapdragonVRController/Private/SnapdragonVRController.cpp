//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#include "SnapdragonVRController.h"
#include "Kismet/KismetMathLibrary.h"
#include "SnapdragonVRControllerFunctionLibrary.h"
#include "Core/Public/Misc/CoreDelegates.h"
#include "IHeadMountedDisplay.h"

#include "Misc/Paths.h"
#include "HAL/FileManager.h"
#include "GameFramework/WorldSettings.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "HAL/UnrealMemory.h"
#include "SCGSXRApi.h"

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


//DEFINE_LOG_CATEGORY_STATIC(LogSnapdragonVRController, Log, All);
#define LOCTEXT_NAMESPACE "SnapdragonVRController"
//-----------------------------------------------------------------------------

uint32_t TempButtonState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER] = {};

uint32_t TempiousTouchState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER] = {};

GSXRVector2 TempAxis2D[CONTROLLERS_PER_PLAYER][4] = {};

float TempAxis[CONTROLLERS_PER_PLAYER][8] = {};

class FSnapdragonVRControllerPlugin : public ISnapdragonVRControllerPlugin
{
	virtual void StartupModule() override
	{
		UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRControllerPlugin::StartupModule"));
		IInputDeviceModule::StartupModule();
		FSnapdragonVRController::PreInit();
	}

	virtual TSharedPtr< class IInputDevice > CreateInputDevice(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler) override
	{
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS || (PLATFORM_WINDOWS && WINVER > 0x0502)
		UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRControllerPlugin::CreateInputDevice"));

		return TSharedPtr< class IInputDevice >(new FSnapdragonVRController(InMessageHandler));
#else
		return nullptr;
#endif

	}
};

FSnapdragonVRController* GetSnapdragonVRController2()
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

//手柄扳机及侧键回调函数，X和Y取值范围为0-10，lr 0为左 1为右
void OnControllerTriggerCallback(int hall_x, int hall_y, const int lr)
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("TriggerCallback:hall_x = %d, hall_y = %d, lr = %d"), hall_x, hall_y, lr);

	TempAxis[lr][ESnapdragonVRControllerAxis1D::Type::Trigger] = hall_x;
	TempAxis[lr][ESnapdragonVRControllerAxis1D::Type::Grip] = hall_y;

	if (!lr)
	{
		uint32 Mask = KButtonMask[ESnapdragonVRControllerButton::Button_Left_Trigger];
		TempButtonState[0][lr] = TempButtonState[0][lr] & ~Mask;
		TempButtonState[0][lr] = TempButtonState[0][lr] | (hall_y ? 0 : Mask);
		Mask = KButtonMask[ESnapdragonVRControllerButton::Button_Left_Grip];
		TempButtonState[0][lr] = TempButtonState[0][lr] & ~Mask;
		TempButtonState[0][lr] = TempButtonState[0][lr] | (hall_x ? 0 : Mask);
	}
	else
	{
		uint32 Mask = KButtonMask[ESnapdragonVRControllerButton::Button_Right_Trigger];
		TempButtonState[0][lr] = TempButtonState[0][lr] & ~Mask;
		TempButtonState[0][lr] = TempButtonState[0][lr] | (hall_y ? 0 : Mask);

		Mask = KButtonMask[ESnapdragonVRControllerButton::Button_Right_Grip];
		TempButtonState[0][lr] = TempButtonState[0][lr] & ~Mask;
		TempButtonState[0][lr] = TempButtonState[0][lr] | (hall_x ? 0 : Mask);
	}
}

//手柄按键事件回调
void OnControllerKeyEventCallback(int keycode, int action, const int lr)
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("KeyEventCallback:keycode = %d, action = %d, lr = %d"), keycode, action, lr);

	switch (static_cast<EGSXRControllerKeyCode::Type>(keycode))
	{
	case EGSXRControllerKeyCode::Button_Left_Menu:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Menu)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Menu)];
		break;
	case EGSXRControllerKeyCode::Button_Left_JoyStick:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_JoyStick)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_JoyStick)];
		break;
	case EGSXRControllerKeyCode::Button_A:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_A)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_A)];
		break;
	case EGSXRControllerKeyCode::Button_B:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_B)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_B)];
		break;
	case EGSXRControllerKeyCode::Button_Right_Menu:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Menu)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Menu)];
		break;
	case EGSXRControllerKeyCode::Button_Right_JoyStick:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_JoyStick)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_JoyStick)];
		break;
	case EGSXRControllerKeyCode::Button_Right_Trigger:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Trigger)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Trigger)];
		break;
	case EGSXRControllerKeyCode::Button_Right_Grip:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Grip)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Grip)];
		break;
	case EGSXRControllerKeyCode::Button_X:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_X)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_X)];
		break;
	case EGSXRControllerKeyCode::Button_Y:
		TempButtonState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Y)];
		TempButtonState[0][lr] |= action ? 0 : KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Y)];
		break;
	}
}

//手柄摇杆事件回调函数
void OnControllerRockerCallback(int touch_x, int touch_y, const int lr)
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("RockerCallback:touch_x = %d, touch_y = %d, lr = %d"), touch_x, touch_y, lr);

	FSnapdragonVRController* p = GetSnapdragonVRController2();

	if (!p)
	{
		UE_LOG(LogSnapdragonVRController, Warning, TEXT("RockerCallback GetSnapdragonVRController Is NONE!"));
		return;
	}

	TempAxis2D[lr][ESnapdragonVRControllerAxis2D::JoyStick].x = touch_x;
	TempAxis2D[lr][ESnapdragonVRControllerAxis2D::JoyStick].x = touch_y;
}


//手柄按键触摸事件回调
void OnControllerKeyTouchEventCallback(int keycode, bool touch, const int lr)
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("KeyTouchCallback:keycode = %d, touch = %d, lr = %d"), keycode, touch, lr);

	switch (keycode)
	{
	case EGSXRControllerKeyCode::Button_Left_Menu:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Menu)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Menu)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Left_JoyStick:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_JoyStick)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_JoyStick)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Left_Trigger:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Trigger)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Trigger)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Left_Grip:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Grip)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Left_Grip)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_A:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_A)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_A)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_B:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_B)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_B)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Right_Menu:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Menu)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Menu)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Right_JoyStick:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_JoyStick)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_JoyStick)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Right_Trigger:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Trigger)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Trigger)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Right_Grip:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Grip)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Right_Grip)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_X:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_X)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_X)] : 0;
		break;
	case EGSXRControllerKeyCode::Button_Y:
		TempiousTouchState[0][lr] &= ~KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Y)];
		TempiousTouchState[0][lr] |= touch ? KButtonMask[static_cast<uint32>(ESnapdragonVRControllerButton::Button_Y)] : 0;
		break;
	}
}

IMPLEMENT_MODULE(FSnapdragonVRControllerPlugin, SnapdragonVRController)

//-----------------------------------------------------------------------------
FName FSnapdragonVRController::DeviceTypeName(TEXT("SnapdragonVRController"));

//-----------------------------------------------------------------------------
FSnapdragonVRController::FSnapdragonVRController(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
	: MessageHandler(InMessageHandler),
	bIsTracking(false)
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("SnapdragonVRController Created"));
	
	// Register the motion controller! This is the secret sauce of how to get Motion Controllers working natively.
	// You must MANUALLY call IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this) in your implementation!
	// This allows motion controllers to be both piggy-backed off HMD devices which support them, as well as standing alone.
	IModularFeatures::Get().RegisterModularFeature(GetModularFeatureName(), this);

	StartTracking();
	for (int i = 0; i < CONTROLLERS_PER_PLAYER; ++i)
	{
		for (int j = 0; j < ESnapdragonVRControllerButton::TotalCount; ++j)
		{
			Touches[i][j] = Buttons[i][j] = FGamepadKeyNames::Type("None");
		}
	}
	// Setup input mappings
	//Touch
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_Left_Menu] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Menu_Touch;
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_X] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_X_Touch;
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_Y] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_Y_Touch;
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_Left_JoyStick] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Touch;
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_Left_Grip] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Grip_Touch;
	Touches[(int32)EControllerHand::Left][ESnapdragonVRControllerTouch::Button_Left_Trigger] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Trigger_Touch;

	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_Right_Menu] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Menu_Touch;
	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_A] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_A_Touch;
	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_B] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_B_Touch;
	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_Right_JoyStick] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Touch;
	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_Right_Grip] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Grip_Touch;
	Touches[(int32)EControllerHand::Right][ESnapdragonVRControllerTouch::Button_Right_Trigger] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Trigger_Touch;

	//Buttons
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_Left_Menu] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Menu;
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_X] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_X;
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_Y] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_Y;
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_Left_JoyStick] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_JoyStick;
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_Left_Grip] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Grip;
	Buttons[(int32)EControllerHand::Left][ESnapdragonVRControllerButton::Button_Left_Trigger] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Trigger;

	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_Right_Menu] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Menu;
	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_A] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_A;
	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_B] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Button_B;
	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_Right_JoyStick] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_JoyStick;
	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_Right_Grip] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Grip;
	Buttons[(int32)EControllerHand::Right][ESnapdragonVRControllerButton::Button_Right_Trigger] = SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Trigger;



	// Register life cycle delegates
	// See "Multi-Window Lifecycle" https://developer.android.com/guide/topics/ui/multi-window 
	// Instead of handling APP_CMD_RESUME and APP_CMD_PAUSE, handle APP_CMD_START and APP_CMD_STOP
	FCoreDelegates::ApplicationWillEnterBackgroundDelegate.AddRaw(this, &FSnapdragonVRController::ApplicationStopDelegate);
	FCoreDelegates::ApplicationHasEnteredForegroundDelegate.AddRaw(this, &FSnapdragonVRController::ApplicationStartDelegate);
	//FCoreDelegates::ApplicationWillDeactivateDelegate.AddRaw(this, &FSnapdragonVRController::ApplicationStopDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationWillEnterBackgroundDelegate(), but cover the possibility that only this delegate is called
	//FCoreDelegates::ApplicationHasReactivatedDelegate.AddRaw(this, &FSnapdragonVRController::ApplicationStartDelegate);//calls to this delegate are often (always?) paired with a call to ApplicationHasEnteredForegroundDelegate(), but cover the possibility that only this delegate is called

	// RBF VALIDATE THIS
	// Go ahead and resume to be safe
	//ApplicationResumeDelegate();

	//绑定回调事件
#if PLATFORM_ANDROID

	SC::GSXR_nativeSetControllerTriggerCallback(OnControllerTriggerCallback);
	SC::GSXR_nativeSetControllerRockerCallback(OnControllerRockerCallback);
	SC::GSXR_nativeSetControllerKeyEventCallback(OnControllerKeyEventCallback);
	SC::GSXR_nativeSetControllerKeyTouchEventCallback(OnControllerKeyTouchEventCallback);

#endif

}

//-----------------------------------------------------------------------------
FSnapdragonVRController::~FSnapdragonVRController()
{
	IModularFeatures::Get().UnregisterModularFeature(GetModularFeatureName(), this);

	StopTracking();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::PreInit()
{
	//Add custom keys

	// Warning - any user name longer than 32 chars will not be that visible as it will get cut off...

	// Touch
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Menu_Touch, LOCTEXT("SkyWorthXRController_Left_Menu_Touch", "SkyWorthXR Controller (L) Touch Menu"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Trigger_Touch, LOCTEXT("SkyWorthXRController_Left_Trigger_Touch", "SkyWorthXR Controller (L) Touch Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Grip_Touch, LOCTEXT("SkyWorthXRController_Left_Grip_Touch", "SkyWorthXR Controller (L) Touch Grip"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_JoyStick_Touch, LOCTEXT("SkyWorthXRController_Left_JoyStick_Touch", "SkyWorthXR Controller (L) Touch JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_X_Touch, LOCTEXT("SkyWorthXRController_Button_X_Touch", "SkyWorthXR Controller (L) Touch Button X"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_Y_Touch, LOCTEXT("SkyWorthXRController_Button_Y_Touch", "SkyWorthXR Controller (L) Touch Button Y"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Menu_Touch, LOCTEXT("SkyWorthXRController_Right_Menu_Touch", "SkyWorthXR Controller (R) Touch Menu"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Trigger_Touch, LOCTEXT("SkyWorthXRController_Right_Trigger_Touch", "SkyWorthXR Controller (R) Touch Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Grip_Touch, LOCTEXT("SkyWorthXRController_Right_Grip_Touch", "SkyWorthXR Controller (R) Touch Grip"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_JoyStick_Touch, LOCTEXT("SkyWorthXRController_Right_JoyStick_Touch", "SkyWorthXR Controller (R) Touch JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_A_Touch, LOCTEXT("SkyWorthXRController_Button_A_Touch", "SkyWorthXR Controller (R) Touch Button A"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_B_Touch, LOCTEXT("SkyWorthXRController_Button_B_Touch", "SkyWorthXR Controller (R) Touch Button B"), FKeyDetails::GamepadKey));

	// Axis 1D
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Trigger_Axis, LOCTEXT("SkyWorthXRController_Left_Trigger_Axis", "SkyWorthXR Controller (L) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Grip_Axis, LOCTEXT("SkyWorthXRController_Left_Grip_Axis", "SkyWorthXR Controller (L) Grip Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Trigger_Axis, LOCTEXT("SkyWorthXRController_Right_Trigger_Axis", "SkyWorthXR Controller (R) Trigger Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Grip_Axis, LOCTEXT("SkyWorthXRController_Right_Grip_Axis", "SkyWorthXR Controller (R) Grip_Axis"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));

	//Axis 2D
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_X, LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_X", "SkyWorthXR Controller (L) JoyStick Axis X"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_JoyStick_Axis_Y, LOCTEXT("SkyWorthXRController_Left_JoyStick_Axis_Y", "SkyWorthXR Controller (L) JoyStick Axis Y"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_X, LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_X", "SkyWorthXR Controller (R) JoyStick Axis X"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_JoyStick_Axis_Y, LOCTEXT("SkyWorthXRController_Right_JoyStick_Axis_Y", "SkyWorthXR Controller (R) JoyStick Axis Y"), FKeyDetails::GamepadKey | FKeyDetails::FloatAxis));

	//Buttons
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Menu, LOCTEXT("SkyWorthXRController_Left_Menu", "SkyWorthXR Controller (L) Menu"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Trigger, LOCTEXT("SkyWorthXRController_Left_Trigger", "SkyWorthXR Controller (L) Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_Grip, LOCTEXT("SkyWorthXRController_Left_Grip", "SkyWorthXR Controller (L) Grip"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Left_JoyStick, LOCTEXT("SkyWorthXRController_Left_JoyStick", "SkyWorthXR Controller (L) JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_X, LOCTEXT("SkyWorthXRController_Button_X", "SkyWorthXR Controller (L) Button X"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_Y, LOCTEXT("SkyWorthXRController_Button_Y", "SkyWorthXR Controller (L) Button Y"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Menu, LOCTEXT("SkyWorthXRController_Right_Menu", "SkyWorthXR Controller (R) Menu"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Trigger, LOCTEXT("SkyWorthXRController_Right_Trigger", "SkyWorthXR Controller (R) Trigger"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_Grip, LOCTEXT("SkyWorthXRController_Right_Grip", "SkyWorthXR Controller (R) Grip"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Right_JoyStick, LOCTEXT("SkyWorthXRController_Right_JoyStick", "SkyWorthXR Controller (R) JoyStick"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_A, LOCTEXT("SkyWorthXRController_Button_A", "SkyWorthXR Controller (R) Button_A"), FKeyDetails::GamepadKey));
	EKeys::AddKey(FKeyDetails(SnapdragonVRControllerKeys::SkyWorthXRController_Button_B, LOCTEXT("SkyWorthXRController_Button_B", "SkyWorthXR Controller (R) Button_B"), FKeyDetails::GamepadKey));

}
//-----------------------------------------------------------------------------
void FSnapdragonVRController::ApplicationStopDelegate()
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ApplicationStopDelegate()"));
	StopTracking();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::ApplicationStartDelegate()
{
	UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ApplicationStartDelegate()"));

	StartTracking();
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::StartTracking()
{
	if (bIsTracking)
	{
		return;
	}

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
			ControllerAndHandIndexToDeviceIdMap[i][j] = SC::GSXR_nativeControllerStartTracking(NULL);
			#endif

			if (ControllerAndHandIndexToDeviceIdMap[i][j] > -1)
			{
				bIsTracking = true; // one or more == isTracking
			}

			FMemory::Memset(&currentState[i][j], 0, sizeof(GSXRControllerState));
			FMemory::Memset(&currentCaps[i][j], 0, sizeof(GSXRControllerCaps));
			previousButtonState[i][j] = 0;
			previousTouchState[i][j] = 0;
			previousConnectionState[i][j] = GSXRControllerConnectionState::kGSXRNotInitialized;
		}
	}

	if (bIsTracking)
	{
		UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::StartTracking"));
	}


}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::StopTracking()
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
				#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
				SC::GSXR_nativeControllerStopTracking(ControllerAndHandIndexToDeviceIdMap[i][j]);
				#endif
				ControllerAndHandIndexToDeviceIdMap[i][j] = -1;
			}
		}
	}

	bIsTracking = false;

	UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::StopTracking"));

}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::PollController()
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

		#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

			if (currentState[i][j].connectionState == GSXRControllerConnectionState::kGSXRConnected)
			{
				int bytesWritten = SC::GSXR_nativeControllerQuery(ControllerAndHandIndexToDeviceIdMap[i][j], GSXRControllerQueryType::kGSXRControllerQueryControllerCaps, &currentCaps[i][j], sizeof(GSXRControllerCaps));

				if ( bytesWritten == 0 )
				{
					UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::PollController call for controller caps failed to get any information = zero bytes written"));
				}
				UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::PollController CONNECTED controller %d %d"), i, j);

			}
			else
			{
				//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::PollController NOT CONNECTED"));
			}
			
			currentState[i][j] = SC::GSXR_nativeControllerGetState(ControllerAndHandIndexToDeviceIdMap[i][j], 0 );  // For new service in SVR space, 2nd param is 0

			EControllerHand Hand = static_cast<EControllerHand>(j);
			if (!IsAvailable(i, Hand))
				return;

			float tempbuff[7];
			SC::GSXR_nativeGetControllerPosture(tempbuff, j);
			currentState[i][j].position.x = tempbuff[0];
			currentState[i][j].position.y = tempbuff[1];
			currentState[i][j].position.z = tempbuff[2];
			currentState[i][j].rotation.x = tempbuff[3];
			currentState[i][j].rotation.y = tempbuff[4];
			currentState[i][j].rotation.z = tempbuff[5];
			currentState[i][j].rotation.w = tempbuff[6];

			currentState[i][j].isTouching = TempiousTouchState[i][j];
			currentState[i][j].buttonState = TempButtonState[i][j];
			//currentState[i][j].connectionState = IsConnect[i][j] ? GSXRControllerConnectionState::kGSXRConnected : GSXRControllerConnectionState::kGSXRDisconnected;
			for (int AxisIndex = 0; AxisIndex < (int32)ESnapdragonVRControllerAxis1D::TotalCount; ++AxisIndex)
			{
				currentState[i][j].analog1D[AxisIndex] = TempAxis[j][AxisIndex];
			}

			for (int AxisIndex = 0; AxisIndex < (int32)ESnapdragonVRControllerAxis2D::TotalCount; ++AxisIndex)
			{
				currentState[i][j].analog2D[AxisIndex] = TempAxis2D[j][AxisIndex];
			}
			


		#endif
			//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::PollController -  SC::GSXR_nativeControllerGetState(caps=%d) for Player[%d], Controller[%d]"), currentCaps[i][j].caps, i, j);
		}
	}

}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::ProcessControllerButtons()
{

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			EControllerHand Hand = static_cast<EControllerHand>(j);

			if (IsAvailable(i, Hand))
			{
				for (int32 ButtonIndex = 0; ButtonIndex < (int32)ESnapdragonVRControllerButton::TotalCount; ++ButtonIndex)
				{
					ESnapdragonVRControllerButton::Type ButtonType = static_cast<ESnapdragonVRControllerButton::Type>(ButtonIndex);

					// Process our known set of buttons
					if (GetButtonDown(i, Hand, ButtonType))
					{
						UE_LOG(LogSnapdragonVRController, Log, TEXT("Button down = %s, HandId = %d, ButtonIndex = %d, mask = %X"), *Buttons[j][ButtonIndex].ToString(), j, ButtonIndex, (1 << ButtonIndex));
						MessageHandler->OnControllerButtonPressed(Buttons[j][ButtonIndex], 0, false);
					}
					else if (GetButtonUp(i, Hand, ButtonType))
					{
						UE_LOG(LogSnapdragonVRController, Log, TEXT("Button up = %s, HandId = %d, ButtonIndex = %d, mask = %X"), *Buttons[j][ButtonIndex].ToString(), j, ButtonIndex, (1 << ButtonIndex));
						MessageHandler->OnControllerButtonReleased(Buttons[j][ButtonIndex], 0, false);
					}
				}

				for (int32 TouchIndex = 0; TouchIndex < (int32)ESnapdragonVRControllerTouch::TotalCount; ++TouchIndex)
				{
					ESnapdragonVRControllerTouch::Type TouchType = static_cast<ESnapdragonVRControllerTouch::Type>(TouchIndex);

					// Process our known set of buttons
					if (GetTouchDown(i, Hand, TouchType))
					{
						UE_LOG(LogSnapdragonVRController, Log, TEXT("Touch down = %s, HandId = %d, TouchIndex = %d, mask = %X"), *Touches[j][TouchIndex].ToString(), j, TouchIndex, (1 << TouchIndex));
						MessageHandler->OnControllerButtonPressed(Touches[j][TouchIndex], 0, false);

					}
					else if (GetTouchUp(i, Hand, TouchType))
					{
						UE_LOG(LogSnapdragonVRController, Log, TEXT("Touch up = %s, HandId = %d, TouchIndex = %d, mask = %X"), *Touches[j][TouchIndex].ToString(), j, TouchIndex, (1 << TouchIndex));
						MessageHandler->OnControllerButtonReleased(Touches[j][TouchIndex], 0, false);
					}
				}


				if (Hand == EControllerHand::Left)
				{	
					float Input = GetAxis1D(i, Hand, ESnapdragonVRControllerAxis1D::Trigger);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Trigger_Axis, 0, Input);
					
					Input = GetAxis1D(i, Hand, ESnapdragonVRControllerAxis1D::Grip);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_Grip_Axis, 0, Input);

					FVector2D Loc = GetAxis2D(i, Hand, ESnapdragonVRControllerAxis2D::JoyStick);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Axis_X, 0, Loc.X);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Left_JoyStick_Axis_Y, 0, Loc.Y);

				}
				else if (Hand == EControllerHand::Right)
				{	
					float Input = GetAxis1D(i, Hand, ESnapdragonVRControllerAxis1D::Trigger);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Trigger_Axis, 0, Input);

					Input = GetAxis1D(i, Hand, ESnapdragonVRControllerAxis1D::Grip);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_Grip_Axis, 0, Input);

					FVector2D Loc = GetAxis2D(i, Hand, ESnapdragonVRControllerAxis2D::JoyStick);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Axis_X, 0, Loc.X);
					MessageHandler->OnControllerAnalog(SnapdragonVRControllerKeyNames::SkyWorthXRController_Right_JoyStick_Axis_Y, 0, Loc.Y);
				}
			}
		}
	}

}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::ProcessControllerEvents()
{
// check that we're not going to puke because the array changed size...
static_assert(GSXRControllerConnectionState::kGSXRError == 4, "sxrControllerState.connectionState has changed size");

	for (int i = 0; i < CONTROLLER_PAIRS_MAX; i++)
	{
		for (int j = 0; j < CONTROLLERS_PER_PLAYER; j++)
		{
			GSXRControllerConnectionState currentConnectionState = currentState[i][j].connectionState;
			if (previousConnectionState[i][j] != currentConnectionState)
			{
				ESnapdragonVRControllerState NewState;
				switch (currentConnectionState)
				{
				case GSXRControllerConnectionState::kGSXRDisconnected:
					NewState = ESnapdragonVRControllerState::Disconnected;
					//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ProcessControllerEvents DISCONNECTED"));
					break;
				case GSXRControllerConnectionState::kGSXRConnected:
					NewState = ESnapdragonVRControllerState::Connected;
					//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ProcessControllerEvents CONNECTED"));
					ReadControllerConfigStruct( i, static_cast<EControllerHand>(j) ); // need to wait for Connected before config info is valid
					break;
				case GSXRControllerConnectionState::kGSXRConnecting:
					NewState = ESnapdragonVRControllerState::Connecting;
					//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ProcessControllerEvents CONNECTING"));
					break;
				case GSXRControllerConnectionState::kGSXRError:
					NewState = ESnapdragonVRControllerState::Error;
					//UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ProcessControllerEvents ERROR"));
					break;

				default:
					UE_LOG(LogSnapdragonVRController, Log, TEXT("Connection type not supported: %d"), currentConnectionState);
					NewState = ESnapdragonVRControllerState::Error;
					break;
				}

				USnapdragonVRControllerFunctionLibrary::GetSnapdragonVRControllerEventManager()->OnControllerStateChangedDelegate.Broadcast(i, static_cast<EControllerHand>(j), NewState);
			}
		}
	}
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRController::IsAvailable(const int ControllerIndex, const EControllerHand DeviceHand) const
{
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
	if ((ControllerIndex >= 0 || ControllerIndex < CONTROLLER_PAIRS_MAX) && (int)DeviceHand < CONTROLLERS_PER_PLAYER)
	{
		if (ControllerAndHandIndexToDeviceIdMap[ControllerIndex][ (int32)DeviceHand] >= 0 /*&& 
			SC::GSXR_nativeIsControllerConnect(uint32(DeviceHand))*/)
		{
			return true;
		}
		UE_LOG(LogSnapdragonVRController, Error, TEXT("IsAvailable return False! connectionState = %d"), currentState[ControllerIndex][(int32)DeviceHand].connectionState);
	}
	UE_LOG(LogSnapdragonVRController, Error, TEXT("IsAvailable return False! ControllerIndex = %d, CONTROLLER_PAIRS_MAX = %d, DeviceHand = %d, CONTROLLERS_PER_PLAYER = %d"), ControllerIndex, CONTROLLER_PAIRS_MAX, (int)DeviceHand, CONTROLLERS_PER_PLAYER);
#endif
	return false;
}

//-----------------------------------------------------------------------------
int32_t FSnapdragonVRController::GetBatteryLevel(const int ControllerIndex, const EControllerHand DeviceHand) const
{

#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
		int32_t batteryLevel = -1;
		return SC::GSXR_nativeGetControllerBattery((uint32)DeviceHand);
	}

#endif

	return 0;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::ReadControllerConfigStruct(uint32_t ControllerIndex, EControllerHand Hand) 
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

	if ( !IsAvailable(ControllerIndex, Hand) || GSXRControllerConnectionState::kGSXRConnected != (currentState[ControllerIndex][theHand].connectionState) )
	{
		return;
	}

	// fill struct
	GSXRControllerCaps*  caps = &(currentCaps[ControllerIndex][theHand]);
	if (true != GetControllerCaps(ControllerIndex, Hand, *caps))
	{
		UE_LOG(LogSnapdragonVRController, Log, TEXT("FSnapdragonVRController::ReadControllerConfigStruct - failed to get Controller Caps"));
		return;
	}

	// ok do hardware specific stuff

	// 0th bit = Provides Rotation
	// 1st bit = Provides Position;
	// assume that Just Postion == 6DOF e.g. Pos and Rot
	has6DOF[ControllerIndex][theHand] = caps->caps == 0b11 || caps->caps == 0b10;

	// ghetto char to Wchar to make UE4 logging happy
	wchar_t  wString1[64] = { 0 };
	wchar_t  wString2[64] = { 0 };
	for (int k = 0; k < 64 && 0 != caps->deviceManufacturer[k]; ++k) wString1[k] = (wchar_t)(unsigned int)(caps->deviceManufacturer[k]);
	for (int k = 0; k < 64 && 0 != caps->deviceIdentifier[k];   ++k) wString2[k] = (wchar_t)(unsigned int)(caps->deviceIdentifier[k]);

	UE_LOG(LogSnapdragonVRController, Log, TEXT("Controller found - brand:%s, Model:%s, is %s"), wString1, wString2, has6DOF[ControllerIndex][theHand] ? TEXT("6-DOF") : TEXT("3-DOF"));

	IKnowWhatTheControllerHardwareIsNow = true;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRController::GetControllerCaps(const int ControllerIndex, const EControllerHand DeviceHand, GSXRControllerCaps& controllerCaps)
{
	int bytesWritten = 0;
	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
		memset(&controllerCaps, 0, sizeof(controllerCaps));

#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

		// returns bytes written, if 0 then error
		bytesWritten = SC::GSXR_nativeControllerQuery(controllerHandle, GSXRControllerQueryType::kGSXRControllerQueryControllerCaps, &controllerCaps, sizeof(controllerCaps));
#endif

	}

	return bytesWritten == 0 ? false : true;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRController::Recenter(const int ControllerIndex, const EControllerHand DeviceHand) const
{
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerIndex][(int32)DeviceHand];
		SC::GSXR_nativeControllerSendMessage(controllerHandle, GSXRControllerMessageType::kGSXRControllerMessageRecenter, 0, 0);
		USnapdragonVRControllerFunctionLibrary::GetSnapdragonVRControllerEventManager()->OnControllerRecenteredDelegate.Broadcast(ControllerIndex, DeviceHand);
		return true;
	}

#endif

	return false;
}

//-----------------------------------------------------------------------------
float FSnapdragonVRController::GetWorldToMetersScale() const
{
	float worldScale = 100;

	if (IsInGameThread() && GWorld != nullptr)
	{
		worldScale = GWorld->GetWorldSettings()->WorldToMeters;
	}

	return worldScale;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::Tick(float DeltaTime)
{
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::SendControllerEvents()
{
	PollController();
	ProcessControllerButtons();
	ProcessControllerEvents();
	//UE_LOG(LogSnapdragonVRController, Log, TEXT("SendControllerEvents"));
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler)
{
	MessageHandler = InMessageHandler;
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRController::Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar)
{
	return false;
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value)
{
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

	// Skip unless this is the left or right large channel, which we consider to be the only SteamVRController feedback channel
	if (ChannelType != FForceFeedbackChannelType::LEFT_LARGE && ChannelType != FForceFeedbackChannelType::RIGHT_LARGE)
	{
		return;
	}
	
	const EControllerHand Hand = (ChannelType == FForceFeedbackChannelType::LEFT_LARGE) ? EControllerHand::Left : EControllerHand::Right;
	if (IsAvailable(ControllerId, Hand))
	{	
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		SC::GSXR_nativeControllerSendMessage(controllerHandle, GSXRControllerMessageType::kGSXRControllerMessageVibration, Value, 1.0f);
	}

#endif // SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
void FSnapdragonVRController::SetChannelValues(int32 ControllerId, const FForceFeedbackValues& Values)
{
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS	

	EControllerHand Hand = EControllerHand::Left;
	if (IsAvailable(ControllerId, Hand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		SC::GSXR_nativeControllerSendMessage(controllerHandle, GSXRControllerMessageType::kGSXRControllerMessageVibration, Values.LeftLarge, 1.0f);
	}

	Hand = EControllerHand::Right;
	if (IsAvailable(ControllerId, Hand))
	{
		int32_t controllerHandle = ControllerAndHandIndexToDeviceIdMap[ControllerId][(int32)Hand];
		SC::GSXR_nativeControllerSendMessage(controllerHandle, GSXRControllerMessageType::kGSXRControllerMessageVibration, Values.RightLarge, 1.0f);
	}

#endif // SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
}

//-----------------------------------------------------------------------------
bool FSnapdragonVRController::GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const
{
	
#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS

	if (IsAvailable(ControllerIndex, DeviceHand) && SC::GSXR_nativeIsControllerConnect(uint32(DeviceHand)))
	{
		OutPosition = GetPosition(ControllerIndex, DeviceHand) * WorldToMetersScale;
		OutOrientation = UKismetMathLibrary::ComposeRotators(FRotator(0.f, -90.f, 45.f),GetOrientation(ControllerIndex, DeviceHand).Rotator());
		FVector euler = OutOrientation.Euler();
		UE_LOG(LogSnapdragonVRController, Log, TEXT("Controller IDX=%d, Hand=%s Raw O&P=[%f,%f,%f] [%f,%f,%f,]"), ControllerIndex,
			DeviceHand == EControllerHand::Left ? TEXT("Left") :
				DeviceHand == EControllerHand::Right ? TEXT("Right") : TEXT("Unknown"),
				euler.X, euler.Y, euler.Z, OutPosition.X, OutPosition.Y, OutPosition.Z);

		return true;
	}

#endif
	
	UE_LOG(LogSnapdragonVRController, Log, TEXT("Controller IDX=%d Not Found"), ControllerIndex);
	return false;
}

//-----------------------------------------------------------------------------
ETrackingStatus FSnapdragonVRController::GetControllerTrackingStatus(const int32 ControllerIndex, EControllerHand DeviceHand) const
{
	if (IsAvailable(ControllerIndex, DeviceHand))
	{
		return ETrackingStatus::Tracked;
	}

	return ETrackingStatus::NotTracked;
}

bool FSnapdragonVRController::SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude, float Time)
{

	if (IsAvailable(0, Hand))
	{
		uint32_t frequency1 = (uint32_t)(Frequency * 16);//(uint32_t)std::ceil(frequency*16);
		auto amplitude1 = (uint32_t)(Amplitude * 16);//(uint32_t)std::ceil(amplitude*16);
		auto time1 = (uint32_t)(Time * 10);//(uint32_t)std::round((float)(time / 1e9) / 3.2f * 10);

		uint32_t data;
		data = (Hand == EControllerHand::Left ? 0 : 1);
		data |= Open << 2;
		data |= time1 << 3;
		data |= frequency1 << 8;
		data |= amplitude1 << 12;
		SC::GSXR_nativeSetControllerVibrate(data);
		UE_LOG(LogSnapdragonVRController, Log, TEXT("ControllerVibrate lr = %d, Open = %d, Frequency = %f, Amplitude = %f, Time = %f"), (Hand == EControllerHand::Left ? 0 : 1), Open, Frequency, Amplitude, Time);
		return true;
	}
	return false;
}
//void FSnapdragonVRController::EnumerateSources(TArray<FMotionControllerSource>& SourcesOut) const
//{
//	///SourcesOut.Add(ARCoreMotionSourceId);
//}

#undef LOCTEXT_NAMESPACE
