//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "IInputDevice.h"
#include "InputCoreTypes.h"
#include "XRMotionControllerBase.h"
//#include "IMotionController.h"
#include "CoreMinimal.h"

//DECLARE_LOG_CATEGORY_EXTERN(LogSnapdragonVRController, Log, All);
DEFINE_LOG_CATEGORY_STATIC(LogSnapdragonVRController, Log, All);


#include "ISnapdragonVRControllerPlugin.h"
#include "SCGSXRApi.h" // modified build cs file so that the SVR stuff will also get compiled fir Windows build (catch 


/** Total number of controllers in a set */
#define CONTROLLERS_PER_PLAYER	2

//** Total number of controller pairs */
#define CONTROLLER_PAIRS_MAX 1

/** Controller axis mappings. */
#define DOT_45DEG		0.7071f

struct FSkyWorthXRKeyNames
{
	typedef FName Type;

	static const FName Invalid;

	static const FName Button_Left_Menu;
	static const FName Button_X;
	static const FName Button_Y;
	static const FName Button_Left_JoyStick;
	static const FName Button_Left_Grip;
	static const FName Button_Left_Trigger;

	static const FName Button_Right_Menu;
	static const FName Button_B;
	static const FName Button_A;
	static const FName Button_Right_JoyStick;
	static const FName Button_Right_Grip;
	static const FName Button_Right_Trigger;
};

namespace AndroidControllerKeyNames
{
	const FGamepadKeyNames::Type AndroidMenu("Android_Menu");
	const FGamepadKeyNames::Type AndroidBack("Android_Back");
	const FGamepadKeyNames::Type AndroidVolumeUp("Android_Volume_Up");
	const FGamepadKeyNames::Type AndroidVolumeDown("Android_Volume_Down");
}

namespace SnapdragonVRControllerKeys
{
	// Touch
	const FKey SkyWorthXRController_Left_Menu_Touch("SkyWorthXRController_Left_Menu_Touch");
	const FKey SkyWorthXRController_Button_X_Touch("SkyWorthXRController_Button_X_Touch");
	const FKey SkyWorthXRController_Button_Y_Touch("SkyWorthXRController_Button_Y_Touch");
	const FKey SkyWorthXRController_Left_JoyStick_Touch("SkyWorthXRController_Left_JoyStick_Touch");
	const FKey SkyWorthXRController_Left_Grip_Touch("SkyWorthXRController_Left_Grip_Touch");
	const FKey SkyWorthXRController_Left_Trigger_Touch("SkyWorthXRController_Left_Trigger_Touch");

	const FKey SkyWorthXRController_Right_Menu_Touch("SkyWorthXRController_Right_Menu_Touch");
	const FKey SkyWorthXRController_Button_A_Touch("SkyWorthXRController_Button_A_Touch");
	const FKey SkyWorthXRController_Button_B_Touch("SkyWorthXRController_Button_B_Touch");
	const FKey SkyWorthXRController_Right_JoyStick_Touch("SkyWorthXRController_Right_JoyStick_Touch");
	const FKey SkyWorthXRController_Right_Grip_Touch("SkyWorthXRController_Right_Grip_Touch");
	const FKey SkyWorthXRController_Right_Trigger_Touch("SkyWorthXRController_Right_Trigger_Touch");

	// Axis 1D
	const FKey SkyWorthXRController_Left_Trigger_Axis("SkyWorthXRController_Left_Trigger_Axis");
	const FKey SkyWorthXRController_Left_Grip_Axis("SkyWorthXRController_Left_Grip_Axis");
	const FKey SkyWorthXRController_Right_Trigger_Axis("SkyWorthXRController_Right_Trigger_Axis");
	const FKey SkyWorthXRController_Right_Grip_Axis("SkyWorthXRController_Right_Grip_Axis");

	// Axis 2D
	const FKey SkyWorthXRController_Left_JoyStick_Axis_X("SkyWorthXRController_Left_JoyStick_Axis_X");
	const FKey SkyWorthXRController_Left_JoyStick_Axis_Y("SkyWorthXRController_Left_JoyStick_Axis_Y");
	const FKey SkyWorthXRController_Right_JoyStick_Axis_X("SkyWorthXRController_Right_JoyStick_Axis_X");
	const FKey SkyWorthXRController_Right_JoyStick_Axis_Y("SkyWorthXRController_Right_JoyStick_Axis_Y");

	//Buttons
	const FKey SkyWorthXRController_Left_Menu("SkyWorthXRController_Left_Menu");
	const FKey SkyWorthXRController_Button_X("SkyWorthXRController_Button_X");
	const FKey SkyWorthXRController_Button_Y("SkyWorthXRController_Button_Y");
	const FKey SkyWorthXRController_Left_JoyStick("SkyWorthXRController_Left_JoyStick");
	const FKey SkyWorthXRController_Left_Grip("SkyWorthXRController_Left_Grip");
	const FKey SkyWorthXRController_Left_Trigger("SkyWorthXRController_Left_Trigger");
	const FKey SkyWorthXRController_Right_Menu("SkyWorthXRController_Right_Menu");
	const FKey SkyWorthXRController_Button_A("SkyWorthXRController_Button_A");
	const FKey SkyWorthXRController_Button_B("SkyWorthXRController_Button_B");
	const FKey SkyWorthXRController_Right_JoyStick("SkyWorthXRController_Right_JoyStick");
	const FKey SkyWorthXRController_Right_Grip("SkyWorthXRController_Right_Grip");
	const FKey SkyWorthXRController_Right_Trigger("SkyWorthXRController_Right_Trigger");
}

namespace SnapdragonVRControllerKeyNames
{
	// Touch
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Menu_Touch("SkyWorthXRController_Left_Menu_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_X_Touch("SkyWorthXRController_Button_X_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_Y_Touch("SkyWorthXRController_Button_Y_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_JoyStick_Touch("SkyWorthXRController_Left_JoyStick_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Grip_Touch("SkyWorthXRController_Left_Grip_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Trigger_Touch("SkyWorthXRController_Left_Trigger_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Menu_Touch("SkyWorthXRController_Right_Menu_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_A_Touch("SkyWorthXRController_Button_A_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_B_Touch("SkyWorthXRController_Button_B_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_JoyStick_Touch("SkyWorthXRController_Right_JoyStick_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Grip_Touch("SkyWorthXRController_Right_Grip_Touch");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Trigger_Touch("SkyWorthXRController_Right_Trigger_Touch");

	// Axis 1D
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Trigger_Axis("SkyWorthXRController_Left_Trigger_Axis");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Grip_Axis("SkyWorthXRController_Left_Grip_Axis");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Trigger_Axis("SkyWorthXRController_Right_Trigger_Axis");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Grip_Axis("SkyWorthXRController_Right_Grip_Axis");

	// Axis 2D
	const FGamepadKeyNames::Type SkyWorthXRController_Left_JoyStick_Axis_X("SkyWorthXRController_Left_JoyStick_Axis_X");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_JoyStick_Axis_Y("SkyWorthXRController_Left_JoyStick_Axis_Y");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_JoyStick_Axis_X("SkyWorthXRController_Right_JoyStick_Axis_X");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_JoyStick_Axis_Y("SkyWorthXRController_Right_JoyStick_Axis_Y");

	//Buttons
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Menu("SkyWorthXRController_Left_Menu");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_X("SkyWorthXRController_Button_X");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_Y("SkyWorthXRController_Button_Y");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_JoyStick("SkyWorthXRController_Left_JoyStick");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Grip("SkyWorthXRController_Left_Grip");
	const FGamepadKeyNames::Type SkyWorthXRController_Left_Trigger("SkyWorthXRController_Left_Trigger");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Menu("SkyWorthXRController_Right_Menu");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_A("SkyWorthXRController_Button_A");
	const FGamepadKeyNames::Type SkyWorthXRController_Button_B("SkyWorthXRController_Button_B");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_JoyStick("SkyWorthXRController_Right_JoyStick");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Grip("SkyWorthXRController_Right_Grip");
	const FGamepadKeyNames::Type SkyWorthXRController_Right_Trigger("SkyWorthXRController_Right_Trigger");

}
struct EGSXRControllerKeyCode
{
	enum Type
	{
		Button_Left_Menu = 300,
		Button_X = 301,
		Button_Y = 302,
		Button_Left_JoyStick = 303,
		Button_Left_Grip = 304,
		Button_Left_Trigger = 501,
		Button_Right_Menu = 305,
		Button_B = 306,
		Button_A = 307,
		Button_Right_JoyStick = 308,
		Button_Right_Grip = 309,
		Button_Right_Trigger = 601,

		TotalCount
	};
};
struct ESnapdragonVRControllerTouch
{
	enum Type
	{
		Button_Left_Menu,
		Button_X,
		Button_Y,
		Button_Left_JoyStick,
		Button_Left_Grip,
		Button_Left_Trigger,
		Button_Right_Menu,
		Button_B,
		Button_A,
		Button_Right_JoyStick,
		Button_Right_Grip,
		Button_Right_Trigger,

		TotalCount
	};
};

// Must match the enum order of ESnapdragonVRControllerTouch
const uint32 KTouchMask[30] =
{
	0x00000001,
	0x00000002,
	0x00000004,
	0x00000008,
	0x00000010,
	0x00000020,
	0x00000040,
	0x00000080,
	0x00000100,
	0x00000200,
	0x00001000,
	0x00002000,
	0x00004000,
	0x00008000,
	0x00010000,
	0x00020000,
	0x00040000,
	0x00080000,
	0x00100000,
	0x00200000,
	0x00400000,
	0x00800000,
	0x01000000,
	0x02000000,
	0x04000000,
	0x08000000,
	0x10000000,
	0x20000000,
	0x40000000,
	0x80000000
};

struct ESnapdragonVRControllerAxis1D
{
	enum Type
	{
		Trigger,
		Grip,

		TotalCount
	};
};

struct ESnapdragonVRControllerAxis2D
{
	enum Type
	{
		JoyStick,

		TotalCount
	};
};

struct ESnapdragonVRControllerButton
{
	enum Type
	{
		Button_Left_Menu,
		Button_X,
		Button_Y,
		Button_Left_JoyStick,
		Button_Left_Grip,
		Button_Left_Trigger,
		Button_Right_Menu,
		Button_B,
		Button_A,
		Button_Right_JoyStick,
		Button_Right_Grip,
		Button_Right_Trigger,

		/** Max number of controller buttons.  Must be < 256 */
		TotalCount
	};
};

// Must match the enum order of ESnapdragonVRControllerButton
const uint32 KButtonMask[30] =
{
	0x00000001,
	0x00000002,
	0x00000004,
	0x00000008,
	0x00000010,
	0x00000020,
	0x00000040,
	0x00000080,
	0x00000100,
	0x00000200,
	0x00001000,
	0x00002000,
	0x00004000,
	0x00008000,
	0x00010000,
	0x00020000,
	0x00040000,
	0x00080000,
	0x00100000,
	0x00200000,
	0x00400000,
	0x00800000,
	0x01000000,
	0x02000000,
	0x04000000,
	0x08000000,
	0x10000000,
	0x20000000,
	0x40000000,
	0x80000000
};

class FSnapdragonVRController : public IInputDevice, public FXRMotionControllerBase
{
public:

	/** Constructor that takes an initial message handler that will receive motion controller events */
	FSnapdragonVRController(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);

	virtual ~FSnapdragonVRController();

	static void PreInit();

public: // Helper Functions

	/** Called before application enters background */
	void ApplicationStopDelegate();

	/** Called after application resumes */
	void ApplicationStartDelegate();

	/** Polls the controller state */
	void PollController();

	/** Processes the controller buttons */
	void ProcessControllerButtons();

	/** Processes the controller events */
	void ProcessControllerEvents();

	/** Checks if the controller is available */
	bool IsAvailable(int ControllerIndex, const EControllerHand DeviceHand) const;

	/** Query the controller battery level. */
	int32_t GetBatteryLevel(const int ControllerIndex, const EControllerHand DeviceHand) const;

	/** Query the controller capabilities. */
//#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
	bool GetControllerCaps(const int ControllerIndex, const EControllerHand DeviceHand, GSXRControllerCaps& controllerCaps);

	inline bool is6DOF(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{
		return has6DOF[ControllerIndex][static_cast<int32>(DeviceHand)];
	}
//#endif

	/** Recenters the controller */
	bool Recenter(const int ControllerIndex, const EControllerHand DeviceHand) const;

	float GetWorldToMetersScale() const;

private:

	inline bool GetButton(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerButton::Type button) const 
	{
		uint32 mask = KButtonMask[static_cast<uint32>(button)];
		return ((currentState[ControllerIndex][(int32)DeviceHand].buttonState & mask) != 0);
	}

	inline bool GetButtonUp(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerButton::Type button) const 
	{
		uint32 mask = KButtonMask[static_cast<uint32>(button)];
		return ((previousButtonState[ControllerIndex][(int32)DeviceHand] & mask) != 0) && ((currentState[ControllerIndex][(int32)DeviceHand].buttonState & mask) == 0);
	}

	inline bool GetButtonDown(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerButton::Type button) const
	{
		uint32 mask = KButtonMask[static_cast<uint32>(button)];
		return ((previousButtonState[ControllerIndex][(int32)DeviceHand] & mask) == 0) && ((currentState[ControllerIndex][(int32)DeviceHand].buttonState & mask) != 0);
	}

	inline FQuat GetOrientation(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{		
		GSXRQuaternion rotation = currentState[ControllerIndex][(int32)DeviceHand].rotation;
		return FQuat(-rotation.z, rotation.x, rotation.y, rotation.w);
	}

	inline FVector GetPosition(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{		
		GSXRVector3 position = currentState[ControllerIndex][(int32)DeviceHand].position;
		return FVector(position.z, -position.x, -position.y);
	}

	inline long GetTimestamp(const int32 ControllerIndex, const EControllerHand DeviceHand) const
	{
		return currentState[ControllerIndex][(int32)DeviceHand].timestamp;
	}

	inline FVector2D GetAxis2D(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerAxis2D::Type axi2D) const
	{
		GSXRVector2 axis = currentState[ControllerIndex][(int32)DeviceHand].analog2D[(int)axi2D];
		return FVector2D(axis.x, axis.y);
	}

	inline float GetAxis1D(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerAxis1D::Type axis1D) const
	{
		return currentState[ControllerIndex][(int32)DeviceHand].analog1D[(int)axis1D];
	}

	inline bool GetTouch(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerTouch::Type touch) const
	{
		uint32 mask = KTouchMask[static_cast<uint32>(touch)];
		return ((currentState[ControllerIndex][(int32)DeviceHand].isTouching & mask) != 0);
	}

	inline bool GetTouchDown(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerTouch::Type touch) const
	{
		uint32 mask = KTouchMask[static_cast<uint32>(touch)];
		return ((previousTouchState[ControllerIndex][(int32)DeviceHand] & mask) == 0) && ((currentState[ControllerIndex][(int32)DeviceHand].isTouching & mask) != 0);
	}

	inline bool GetTouchUp(const int32 ControllerIndex, const EControllerHand DeviceHand, ESnapdragonVRControllerTouch::Type touch) const
	{
		uint32 mask = KTouchMask[static_cast<uint32>(touch)];
		return ((previousTouchState[ControllerIndex][(int32)DeviceHand] & mask) != 0) && ((currentState[ControllerIndex][(int32)DeviceHand].isTouching & mask) == 0);
	}
	
	void StartTracking();
	void StopTracking();

public:	// IInputDevice

	/** Tick the interface (e.g. check for new controllers) */
	virtual void Tick(float DeltaTime);

	/** Poll for controller state and send events if needed */
	virtual void SendControllerEvents();

	/** Set which MessageHandler will get the events from SendControllerEvents. */
	virtual void SetMessageHandler(const TSharedRef< FGenericApplicationMessageHandler >& InMessageHandler);

	/** Exec handler to allow console commands to be passed through for debugging */
	virtual bool Exec(UWorld* InWorld, const TCHAR* Cmd, FOutputDevice& Ar);

	/** IForceFeedbackSystem */
	virtual void SetChannelValue(int32 ControllerId, FForceFeedbackChannelType ChannelType, float Value);
	virtual void SetChannelValues(int32 ControllerId, const FForceFeedbackValues &values);

public: // IMotionController

	static FName DeviceTypeName;

	/**
	* Returns the device name
	*
	* @return					Returns the device name
	*/
	virtual FName GetMotionControllerDeviceTypeName() const override
	{
		return DeviceTypeName;
	}

	/**
	* Returns the calibration-space orientation of the requested controller's hand.
	*
	* @param ControllerIndex	The Unreal controller (player) index of the contoller set
	* @param DeviceHand		Which hand, within the controller set for the player, to get the orientation and position for
	* @param OutOrientation	(out) If tracked, the orientation (in calibrated-space) of the controller in the specified hand
	* @param OutPosition		(out) If tracked, the position (in calibrated-space) of the controller in the specified hand
	* @return					True if the device requested is valid and tracked, false otherwise
	*/
	virtual bool GetControllerOrientationAndPosition(const int32 ControllerIndex, const EControllerHand DeviceHand, FRotator& OutOrientation, FVector& OutPosition, float WorldToMetersScale) const;

	/**
	* Returns the tracking status (e.g. not tracked, inertial-only, fully tracked) of the specified controller
	*
	* @return	Tracking status of the specified controller, or ETrackingStatus::NotTracked if the device is not found
	*/
	virtual ETrackingStatus GetControllerTrackingStatus(const int32 ControllerIndex, EControllerHand DeviceHand) const;

	/**
	* Called to request the motion sources that this IMotionController provides
	*
	* @param Sources	A motion source enumerator object that IMotionControllers can add source names to
	*/
//	virtual void EnumerateSources(TArray<FMotionControllerSource>& SourcesOut) const override;


	bool SetControllerVibrate(EControllerHand Hand, bool Open, float Frequency, float Amplitude, float Time);
	/**
	* Returns a custom names parameter value
	*
	* @param MotionSource		The name of the motion source we want parameters for
	* @param ParameterName		The specific value we are looking for
	* @param bOutValueFound	(out) Whether the parameter could be found
	*
	* @return			The value of the parameter
	*/
//	virtual float GetCustomParameterValue(const FName MotionSource, FName ParameterName, bool& bOutValueFound) const override { bOutValueFound = false;  return 0.f; }

	/** Cached controller info */
//#if SNAPDRAGONVRCONTROLLER_SUPPORTED_PLATFORMS
//	sxrControllerState currentState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
//	GSXRControllerCaps currentCaps[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
//	sxrControllerConnectionState previousConnectionState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
//#endif
	GSXRControllerState currentState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	GSXRControllerCaps currentCaps[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	GSXRControllerConnectionState previousConnectionState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	uint32_t previousButtonState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	uint32_t previousTouchState[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	bool has6DOF[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];
	/*friend void OnControllerTriggerCallback(int hall_x, int hall_y, const int lr);
	friend void OnControllerKeyEventCallback(int keycode, int action, const int lr);
	friend void OnControllerRockerCallback(int touch_x, int touch_y, const int lr);
	friend void OnControllerKeyTouchEventCallback(int keycode, bool touch, const int lr);*/
private:

	bool bIsTracking;

	void ReadControllerConfigStruct(uint32_t i, EControllerHand Hand);


	/** Mapping of controller buttons and touch */
	FSkyWorthXRKeyNames::Type Buttons[CONTROLLERS_PER_PLAYER][ESnapdragonVRControllerButton::TotalCount];
	FSkyWorthXRKeyNames::Type Touches[CONTROLLERS_PER_PLAYER][ESnapdragonVRControllerTouch::TotalCount];

	int32_t ControllerAndHandIndexToDeviceIdMap[CONTROLLER_PAIRS_MAX][CONTROLLERS_PER_PLAYER];

	/** handler to send all messages to */
	TSharedRef<FGenericApplicationMessageHandler> MessageHandler;

	/** Last Orientation used */
	mutable FRotator LastOrientation;
};
