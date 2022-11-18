//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//==============================================================================

#pragma once

#include "CoreMinimal.h"
#include "Delegates/Delegate.h"
#include "UObject/Object.h"
#include "UObject/WeakObjectPtr.h"
#include "SkyWorthVRControllerEventManager.generated.h"

UENUM(BlueprintType)
enum class ESkyWorthVRControllerState : uint8
{
	Disconnected = 1,
	Connected = 2,
	Connecting = 3,
	Error = 0
};

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FSkyWorthVRControllerRecenterDelegate, int32, ControllerIndex, EControllerHand, Hand);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FSkyWorthVRControllerStateChangeDelegate, int32, ControllerIndex, EControllerHand, hand, ESkyWorthVRControllerState, NewControllerState);

UCLASS()
class USkyWorthVRControllerEventManager : public UObject
{
	GENERATED_UCLASS_BODY()

public:

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRControllerRecenterDelegate OnControllerRecenteredDelegate;

	UPROPERTY(BlueprintAssignable)
		FSkyWorthVRControllerStateChangeDelegate OnControllerStateChangedDelegate;

public:
	static USkyWorthVRControllerEventManager* GetInstance();
};