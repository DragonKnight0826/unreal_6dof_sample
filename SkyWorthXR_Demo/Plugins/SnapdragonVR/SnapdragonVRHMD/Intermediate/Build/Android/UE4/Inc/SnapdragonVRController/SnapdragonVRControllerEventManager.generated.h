// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
enum class EControllerHand : uint8;
enum class ESnapdragonVRControllerState : uint8;
#ifdef SNAPDRAGONVRCONTROLLER_SnapdragonVRControllerEventManager_generated_h
#error "SnapdragonVRControllerEventManager.generated.h already included, missing '#pragma once' in SnapdragonVRControllerEventManager.h"
#endif
#define SNAPDRAGONVRCONTROLLER_SnapdragonVRControllerEventManager_generated_h

#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_26_DELEGATE \
struct _Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms \
{ \
	int32 ControllerIndex; \
	EControllerHand hand; \
	ESnapdragonVRControllerState NewControllerState; \
}; \
static inline void FSnapdragonVRControllerStateChangeDelegate_DelegateWrapper(const FMulticastScriptDelegate& SnapdragonVRControllerStateChangeDelegate, int32 ControllerIndex, EControllerHand hand, ESnapdragonVRControllerState NewControllerState) \
{ \
	_Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms Parms; \
	Parms.ControllerIndex=ControllerIndex; \
	Parms.hand=hand; \
	Parms.NewControllerState=NewControllerState; \
	SnapdragonVRControllerStateChangeDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_25_DELEGATE \
struct _Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms \
{ \
	int32 ControllerIndex; \
	EControllerHand Hand; \
}; \
static inline void FSnapdragonVRControllerRecenterDelegate_DelegateWrapper(const FMulticastScriptDelegate& SnapdragonVRControllerRecenterDelegate, int32 ControllerIndex, EControllerHand Hand) \
{ \
	_Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms Parms; \
	Parms.ControllerIndex=ControllerIndex; \
	Parms.Hand=Hand; \
	SnapdragonVRControllerRecenterDelegate.ProcessMulticastDelegate<UObject>(&Parms); \
}


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_SPARSE_DATA
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_RPC_WRAPPERS
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_RPC_WRAPPERS_NO_PURE_DECLS
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSnapdragonVRControllerEventManager(); \
	friend struct Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRControllerEventManager, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRController"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRControllerEventManager)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_INCLASS \
private: \
	static void StaticRegisterNativesUSnapdragonVRControllerEventManager(); \
	friend struct Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRControllerEventManager, UObject, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRController"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRControllerEventManager)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRControllerEventManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRControllerEventManager) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRControllerEventManager); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRControllerEventManager); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRControllerEventManager(USnapdragonVRControllerEventManager&&); \
	NO_API USnapdragonVRControllerEventManager(const USnapdragonVRControllerEventManager&); \
public:


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRControllerEventManager(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRControllerEventManager(USnapdragonVRControllerEventManager&&); \
	NO_API USnapdragonVRControllerEventManager(const USnapdragonVRControllerEventManager&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRControllerEventManager); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRControllerEventManager); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRControllerEventManager)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_PRIVATE_PROPERTY_OFFSET
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_28_PROLOG
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_RPC_WRAPPERS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_INCLASS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_INCLASS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h_31_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SnapdragonVRControllerEventManager."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SNAPDRAGONVRCONTROLLER_API UClass* StaticClass<class USnapdragonVRControllerEventManager>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerEventManager_h


#define FOREACH_ENUM_ESNAPDRAGONVRCONTROLLERSTATE(op) \
	op(ESnapdragonVRControllerState::Disconnected) \
	op(ESnapdragonVRControllerState::Connected) \
	op(ESnapdragonVRControllerState::Connecting) \
	op(ESnapdragonVRControllerState::Error) 

enum class ESnapdragonVRControllerState : uint8;
template<> SNAPDRAGONVRCONTROLLER_API UEnum* StaticEnum<ESnapdragonVRControllerState>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
