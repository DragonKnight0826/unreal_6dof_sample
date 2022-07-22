// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
class USnapdragonVRControllerEventManager;
enum class EControllerHand : uint8;
enum class ETrackingStatus : uint8;
struct FRotator;
struct FVector;
#ifdef SNAPDRAGONVRCONTROLLER_SnapdragonVRControllerFunctionLibrary_generated_h
#error "SnapdragonVRControllerFunctionLibrary.generated.h already included, missing '#pragma once' in SnapdragonVRControllerFunctionLibrary.h"
#endif
#define SNAPDRAGONVRCONTROLLER_SnapdragonVRControllerFunctionLibrary_generated_h

#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_SPARSE_DATA
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execGetSnapdragonVRControllerEventManager); \
	DECLARE_FUNCTION(execGetControllerTrackingStatus); \
	DECLARE_FUNCTION(execRecenter); \
	DECLARE_FUNCTION(execSetControllerVibrate); \
	DECLARE_FUNCTION(execGetControllerBatteryLevel); \
	DECLARE_FUNCTION(execGetControllerOrientationAndPosition);


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execGetSnapdragonVRControllerEventManager); \
	DECLARE_FUNCTION(execGetControllerTrackingStatus); \
	DECLARE_FUNCTION(execRecenter); \
	DECLARE_FUNCTION(execSetControllerVibrate); \
	DECLARE_FUNCTION(execGetControllerBatteryLevel); \
	DECLARE_FUNCTION(execGetControllerOrientationAndPosition);


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSnapdragonVRControllerFunctionLibrary(); \
	friend struct Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRControllerFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRController"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRControllerFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_INCLASS \
private: \
	static void StaticRegisterNativesUSnapdragonVRControllerFunctionLibrary(); \
	friend struct Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRControllerFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRController"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRControllerFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRControllerFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRControllerFunctionLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRControllerFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRControllerFunctionLibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRControllerFunctionLibrary(USnapdragonVRControllerFunctionLibrary&&); \
	NO_API USnapdragonVRControllerFunctionLibrary(const USnapdragonVRControllerFunctionLibrary&); \
public:


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRControllerFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRControllerFunctionLibrary(USnapdragonVRControllerFunctionLibrary&&); \
	NO_API USnapdragonVRControllerFunctionLibrary(const USnapdragonVRControllerFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRControllerFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRControllerFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRControllerFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_PRIVATE_PROPERTY_OFFSET
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_20_PROLOG
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_RPC_WRAPPERS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_INCLASS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_RPC_WRAPPERS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_INCLASS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h_23_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SnapdragonVRControllerFunctionLibrary."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SNAPDRAGONVRCONTROLLER_API UClass* StaticClass<class USnapdragonVRControllerFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRController_Classes_SnapdragonVRControllerFunctionLibrary_h


PRAGMA_ENABLE_DEPRECATION_WARNINGS
