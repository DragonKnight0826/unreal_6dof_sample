// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/ObjectMacros.h"
#include "UObject/ScriptMacros.h"

PRAGMA_DISABLE_DEPRECATION_WARNINGS
struct FVector;
struct FQuat;
struct FRotator;
enum class ESnapdragonVRHMD_Eye : uint8;
class USnapdragonVRHMDEventManager;
#ifdef SNAPDRAGONVRHMD_SnapdragonVRHMDFunctionLibrary_generated_h
#error "SnapdragonVRHMDFunctionLibrary.generated.h already included, missing '#pragma once' in SnapdragonVRHMDFunctionLibrary.h"
#endif
#define SNAPDRAGONVRHMD_SnapdragonVRHMDFunctionLibrary_generated_h

#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_SPARSE_DATA
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_RPC_WRAPPERS \
 \
	DECLARE_FUNCTION(execisEyetrackingEnabled); \
	DECLARE_FUNCTION(execenableEyetrackingMarker); \
	DECLARE_FUNCTION(execisEyetrackingMarkerEnabled); \
	DECLARE_FUNCTION(execsetEyetrackingMarkerRadius); \
	DECLARE_FUNCTION(execgetEyetrackingMarkerRadius); \
	DECLARE_FUNCTION(execgetAverageGazeDirection); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusFrequencyRho); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusFrequencyRho); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusFrequency); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusFrequency); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusAmplitude); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusAmplitude); \
	DECLARE_FUNCTION(execsetTextureMinimumPixelDensity); \
	DECLARE_FUNCTION(execgetTextureMinimumPixelDensity); \
	DECLARE_FUNCTION(execsetTextureFoveationArea); \
	DECLARE_FUNCTION(execgetTextureFoveationArea); \
	DECLARE_FUNCTION(execsetTextureFoveationGain); \
	DECLARE_FUNCTION(execgetTextureFoveationGain); \
	DECLARE_FUNCTION(execenableTextureFoveationFocus); \
	DECLARE_FUNCTION(execenableTextureFoveation); \
	DECLARE_FUNCTION(execenableTemporalFoveation); \
	DECLARE_FUNCTION(execisTextureFoveationFocusEnabled); \
	DECLARE_FUNCTION(execisTextureFoveationEnabled); \
	DECLARE_FUNCTION(execisTemporalFoveationEnabled); \
	DECLARE_FUNCTION(execRecenterPosition); \
	DECLARE_FUNCTION(execRecenterOrientation); \
	DECLARE_FUNCTION(execGetHeadOrientationAndPosition); \
	DECLARE_FUNCTION(execGetPupilDialation); \
	DECLARE_FUNCTION(execGetEyeOpenness); \
	DECLARE_FUNCTION(execGetEyeOrientationAndPosition); \
	DECLARE_FUNCTION(execGetEyeGazeDirection); \
	DECLARE_FUNCTION(execGetSnapdragonVRHMDEventManager); \
	DECLARE_FUNCTION(execSetCpuAndGpuLevelsSVR);


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
 \
	DECLARE_FUNCTION(execisEyetrackingEnabled); \
	DECLARE_FUNCTION(execenableEyetrackingMarker); \
	DECLARE_FUNCTION(execisEyetrackingMarkerEnabled); \
	DECLARE_FUNCTION(execsetEyetrackingMarkerRadius); \
	DECLARE_FUNCTION(execgetEyetrackingMarkerRadius); \
	DECLARE_FUNCTION(execgetAverageGazeDirection); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusFrequencyRho); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusFrequencyRho); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusFrequency); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusFrequency); \
	DECLARE_FUNCTION(execsetTextureFoveationFocusAmplitude); \
	DECLARE_FUNCTION(execgetTextureFoveationFocusAmplitude); \
	DECLARE_FUNCTION(execsetTextureMinimumPixelDensity); \
	DECLARE_FUNCTION(execgetTextureMinimumPixelDensity); \
	DECLARE_FUNCTION(execsetTextureFoveationArea); \
	DECLARE_FUNCTION(execgetTextureFoveationArea); \
	DECLARE_FUNCTION(execsetTextureFoveationGain); \
	DECLARE_FUNCTION(execgetTextureFoveationGain); \
	DECLARE_FUNCTION(execenableTextureFoveationFocus); \
	DECLARE_FUNCTION(execenableTextureFoveation); \
	DECLARE_FUNCTION(execenableTemporalFoveation); \
	DECLARE_FUNCTION(execisTextureFoveationFocusEnabled); \
	DECLARE_FUNCTION(execisTextureFoveationEnabled); \
	DECLARE_FUNCTION(execisTemporalFoveationEnabled); \
	DECLARE_FUNCTION(execRecenterPosition); \
	DECLARE_FUNCTION(execRecenterOrientation); \
	DECLARE_FUNCTION(execGetHeadOrientationAndPosition); \
	DECLARE_FUNCTION(execGetPupilDialation); \
	DECLARE_FUNCTION(execGetEyeOpenness); \
	DECLARE_FUNCTION(execGetEyeOrientationAndPosition); \
	DECLARE_FUNCTION(execGetEyeGazeDirection); \
	DECLARE_FUNCTION(execGetSnapdragonVRHMDEventManager); \
	DECLARE_FUNCTION(execSetCpuAndGpuLevelsSVR);


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_INCLASS_NO_PURE_DECLS \
private: \
	static void StaticRegisterNativesUSnapdragonVRHMDFunctionLibrary(); \
	friend struct Z_Construct_UClass_USnapdragonVRHMDFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRHMDFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRHMD"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRHMDFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_INCLASS \
private: \
	static void StaticRegisterNativesUSnapdragonVRHMDFunctionLibrary(); \
	friend struct Z_Construct_UClass_USnapdragonVRHMDFunctionLibrary_Statics; \
public: \
	DECLARE_CLASS(USnapdragonVRHMDFunctionLibrary, UBlueprintFunctionLibrary, COMPILED_IN_FLAGS(0), CASTCLASS_None, TEXT("/Script/SnapdragonVRHMD"), NO_API) \
	DECLARE_SERIALIZER(USnapdragonVRHMDFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_STANDARD_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRHMDFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRHMDFunctionLibrary) \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRHMDFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRHMDFunctionLibrary); \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRHMDFunctionLibrary(USnapdragonVRHMDFunctionLibrary&&); \
	NO_API USnapdragonVRHMDFunctionLibrary(const USnapdragonVRHMDFunctionLibrary&); \
public:


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_ENHANCED_CONSTRUCTORS \
	/** Standard constructor, called after all reflected properties have been initialized */ \
	NO_API USnapdragonVRHMDFunctionLibrary(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get()) : Super(ObjectInitializer) { }; \
private: \
	/** Private move- and copy-constructors, should never be used */ \
	NO_API USnapdragonVRHMDFunctionLibrary(USnapdragonVRHMDFunctionLibrary&&); \
	NO_API USnapdragonVRHMDFunctionLibrary(const USnapdragonVRHMDFunctionLibrary&); \
public: \
	DECLARE_VTABLE_PTR_HELPER_CTOR(NO_API, USnapdragonVRHMDFunctionLibrary); \
DEFINE_VTABLE_PTR_HELPER_CTOR_CALLER(USnapdragonVRHMDFunctionLibrary); \
	DEFINE_DEFAULT_OBJECT_INITIALIZER_CONSTRUCTOR_CALL(USnapdragonVRHMDFunctionLibrary)


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_PRIVATE_PROPERTY_OFFSET
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_28_PROLOG
#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_GENERATED_BODY_LEGACY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_RPC_WRAPPERS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_INCLASS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_STANDARD_CONSTRUCTORS \
public: \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


#define SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_GENERATED_BODY \
PRAGMA_DISABLE_DEPRECATION_WARNINGS \
public: \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_PRIVATE_PROPERTY_OFFSET \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_SPARSE_DATA \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_RPC_WRAPPERS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_INCLASS_NO_PURE_DECLS \
	SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h_31_ENHANCED_CONSTRUCTORS \
static_assert(false, "Unknown access specifier for GENERATED_BODY() macro in class SnapdragonVRHMDFunctionLibrary."); \
PRAGMA_ENABLE_DEPRECATION_WARNINGS


template<> SNAPDRAGONVRHMD_API UClass* StaticClass<class USnapdragonVRHMDFunctionLibrary>();

#undef CURRENT_FILE_ID
#define CURRENT_FILE_ID SkyWorthXR_Demo_Plugins_SnapdragonVR_SnapdragonVRHMD_Source_SnapdragonVRHMD_Public_SnapdragonVRHMDFunctionLibrary_h


#define FOREACH_ENUM_ESNAPDRAGONVRHMD_EYE(op) \
	op(ESnapdragonVRHMD_Eye::LeftEye) \
	op(ESnapdragonVRHMD_Eye::RightEye) \
	op(ESnapdragonVRHMD_Eye::AverageEye) 

enum class ESnapdragonVRHMD_Eye : uint8;
template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRHMD_Eye>();

PRAGMA_ENABLE_DEPRECATION_WARNINGS
