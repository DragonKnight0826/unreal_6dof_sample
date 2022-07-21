// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SnapdragonVRController/Classes/SnapdragonVRControllerFunctionLibrary.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSnapdragonVRControllerFunctionLibrary() {}
// Cross Module References
	SNAPDRAGONVRCONTROLLER_API UClass* Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_NoRegister();
	SNAPDRAGONVRCONTROLLER_API UClass* Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary();
	ENGINE_API UClass* Z_Construct_UClass_UBlueprintFunctionLibrary();
	UPackage* Z_Construct_UPackage__Script_SnapdragonVRController();
	INPUTCORE_API UEnum* Z_Construct_UEnum_InputCore_EControllerHand();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FRotator();
	COREUOBJECT_API UScriptStruct* Z_Construct_UScriptStruct_FVector();
	HEADMOUNTEDDISPLAY_API UEnum* Z_Construct_UEnum_HeadMountedDisplay_ETrackingStatus();
	SNAPDRAGONVRCONTROLLER_API UClass* Z_Construct_UClass_USnapdragonVRControllerEventManager_NoRegister();
// End Cross Module References
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execGetSnapdragonVRControllerEventManager)
	{
		P_FINISH;
		P_NATIVE_BEGIN;
		*(USnapdragonVRControllerEventManager**)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::GetSnapdragonVRControllerEventManager();
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execGetControllerTrackingStatus)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_ControllerIndex);
		P_GET_ENUM(EControllerHand,Z_Param_DeviceHand);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(ETrackingStatus*)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::GetControllerTrackingStatus(Z_Param_ControllerIndex,EControllerHand(Z_Param_DeviceHand));
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execRecenter)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_ControllerIndex);
		P_GET_ENUM(EControllerHand,Z_Param_Hand);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::Recenter(Z_Param_ControllerIndex,EControllerHand(Z_Param_Hand));
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execSetControllerVibrate)
	{
		P_GET_ENUM(EControllerHand,Z_Param_Hand);
		P_GET_UBOOL(Z_Param_Open);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Frequency);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Amplitude);
		P_GET_PROPERTY(FFloatProperty,Z_Param_Time);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::SetControllerVibrate(EControllerHand(Z_Param_Hand),Z_Param_Open,Z_Param_Frequency,Z_Param_Amplitude,Z_Param_Time);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execGetControllerBatteryLevel)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_ControllerIndex);
		P_GET_ENUM(EControllerHand,Z_Param_Hand);
		P_GET_PROPERTY_REF(FIntProperty,Z_Param_Out_OutBatteryLevel);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::GetControllerBatteryLevel(Z_Param_ControllerIndex,EControllerHand(Z_Param_Hand),Z_Param_Out_OutBatteryLevel);
		P_NATIVE_END;
	}
	DEFINE_FUNCTION(USnapdragonVRControllerFunctionLibrary::execGetControllerOrientationAndPosition)
	{
		P_GET_PROPERTY(FIntProperty,Z_Param_ControllerIndex);
		P_GET_ENUM(EControllerHand,Z_Param_Hand);
		P_GET_STRUCT_REF(FRotator,Z_Param_Out_OutOrientation);
		P_GET_STRUCT_REF(FVector,Z_Param_Out_OutPosition);
		P_FINISH;
		P_NATIVE_BEGIN;
		*(bool*)Z_Param__Result=USnapdragonVRControllerFunctionLibrary::GetControllerOrientationAndPosition(Z_Param_ControllerIndex,EControllerHand(Z_Param_Hand),Z_Param_Out_OutOrientation,Z_Param_Out_OutPosition);
		P_NATIVE_END;
	}
	void USnapdragonVRControllerFunctionLibrary::StaticRegisterNativesUSnapdragonVRControllerFunctionLibrary()
	{
		UClass* Class = USnapdragonVRControllerFunctionLibrary::StaticClass();
		static const FNameNativePtrPair Funcs[] = {
			{ "GetControllerBatteryLevel", &USnapdragonVRControllerFunctionLibrary::execGetControllerBatteryLevel },
			{ "GetControllerOrientationAndPosition", &USnapdragonVRControllerFunctionLibrary::execGetControllerOrientationAndPosition },
			{ "GetControllerTrackingStatus", &USnapdragonVRControllerFunctionLibrary::execGetControllerTrackingStatus },
			{ "GetSnapdragonVRControllerEventManager", &USnapdragonVRControllerFunctionLibrary::execGetSnapdragonVRControllerEventManager },
			{ "Recenter", &USnapdragonVRControllerFunctionLibrary::execRecenter },
			{ "SetControllerVibrate", &USnapdragonVRControllerFunctionLibrary::execSetControllerVibrate },
		};
		FNativeFunctionRegistrar::RegisterFunctions(Class, Funcs, UE_ARRAY_COUNT(Funcs));
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms
		{
			int32 ControllerIndex;
			EControllerHand Hand;
			int32 OutBatteryLevel;
			bool ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hand;
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_OutBatteryLevel;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms, ControllerIndex), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_Hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_Hand = { "Hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms, Hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_OutBatteryLevel = { "OutBatteryLevel", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms, OutBatteryLevel), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms), &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_Hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_Hand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_OutBatteryLevel,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "GetControllerBatteryLevel", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventGetControllerBatteryLevel_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14422401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms
		{
			int32 ControllerIndex;
			EControllerHand Hand;
			FRotator OutOrientation;
			FVector OutPosition;
			bool ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hand;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_OutOrientation;
		static const UE4CodeGen_Private::FStructPropertyParams NewProp_OutPosition;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms, ControllerIndex), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_Hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_Hand = { "Hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms, Hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_OutOrientation = { "OutOrientation", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms, OutOrientation), Z_Construct_UScriptStruct_FRotator, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FStructPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_OutPosition = { "OutPosition", nullptr, (EPropertyFlags)0x0010000000000180, UE4CodeGen_Private::EPropertyGenFlags::Struct, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms, OutPosition), Z_Construct_UScriptStruct_FVector, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms), &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_Hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_Hand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_OutOrientation,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_OutPosition,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "GetControllerOrientationAndPosition", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventGetControllerOrientationAndPosition_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x14C22401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventGetControllerTrackingStatus_Parms
		{
			int32 ControllerIndex;
			EControllerHand DeviceHand;
			ETrackingStatus ReturnValue;
		};
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_ControllerIndex_MetaData[];
#endif
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_DeviceHand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_DeviceHand;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_ReturnValue_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ControllerIndex_MetaData[] = {
		{ "NativeConst", "" },
	};
#endif
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000082, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerTrackingStatus_Parms, ControllerIndex), METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ControllerIndex_MetaData, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ControllerIndex_MetaData)) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_DeviceHand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_DeviceHand = { "DeviceHand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerTrackingStatus_Parms, DeviceHand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ReturnValue_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetControllerTrackingStatus_Parms, ReturnValue), Z_Construct_UEnum_HeadMountedDisplay_ETrackingStatus, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_DeviceHand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_DeviceHand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ReturnValue_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "GetControllerTrackingStatus", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventGetControllerTrackingStatus_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventGetSnapdragonVRControllerEventManager_Parms
		{
			USnapdragonVRControllerEventManager* ReturnValue;
		};
		static const UE4CodeGen_Private::FObjectPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FObjectPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Object, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventGetSnapdragonVRControllerEventManager_Parms, ReturnValue), Z_Construct_UClass_USnapdragonVRControllerEventManager_NoRegister, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "Keywords", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "GetSnapdragonVRControllerEventManager", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventGetSnapdragonVRControllerEventManager_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms
		{
			int32 ControllerIndex;
			EControllerHand Hand;
			bool ReturnValue;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hand;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms, ControllerIndex), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_Hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_Hand = { "Hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms, Hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms), &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_Hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_Hand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "Recenter", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventRecenter_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics
	{
		struct SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms
		{
			EControllerHand Hand;
			bool Open;
			float Frequency;
			float Amplitude;
			float Time;
			bool ReturnValue;
		};
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hand;
		static void NewProp_Open_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_Open;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Frequency;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Amplitude;
		static const UE4CodeGen_Private::FFloatPropertyParams NewProp_Time;
		static void NewProp_ReturnValue_SetBit(void* Obj);
		static const UE4CodeGen_Private::FBoolPropertyParams NewProp_ReturnValue;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Hand = { "Hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms, Hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Open_SetBit(void* Obj)
	{
		((SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms*)Obj)->Open = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Open = { "Open", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms), &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Open_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Frequency = { "Frequency", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms, Frequency), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Amplitude = { "Amplitude", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms, Amplitude), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FFloatPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Time = { "Time", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Float, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms, Time), METADATA_PARAMS(nullptr, 0) };
	void Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_ReturnValue_SetBit(void* Obj)
	{
		((SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms*)Obj)->ReturnValue = 1;
	}
	const UE4CodeGen_Private::FBoolPropertyParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_ReturnValue = { "ReturnValue", nullptr, (EPropertyFlags)0x0010000000000580, UE4CodeGen_Private::EPropertyGenFlags::Bool | UE4CodeGen_Private::EPropertyGenFlags::NativeBool, RF_Public|RF_Transient|RF_MarkAsNative, 1, sizeof(bool), sizeof(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms), &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_ReturnValue_SetBit, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Hand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Open,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Frequency,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Amplitude,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_Time,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::NewProp_ReturnValue,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::Function_MetaDataParams[] = {
		{ "Category", "SnapdragonVRController" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::FuncParams = { (UObject*(*)())Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, nullptr, "SetControllerVibrate", nullptr, nullptr, sizeof(SnapdragonVRControllerFunctionLibrary_eventSetControllerVibrate_Parms), Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x04022401, 0, 0, METADATA_PARAMS(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	UClass* Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_NoRegister()
	{
		return USnapdragonVRControllerFunctionLibrary::StaticClass();
	}
	struct Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics
	{
		static UObject* (*const DependentSingletons[])();
		static const FClassFunctionLinkInfo FuncInfo[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UBlueprintFunctionLibrary,
		(UObject* (*)())Z_Construct_UPackage__Script_SnapdragonVRController,
	};
	const FClassFunctionLinkInfo Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::FuncInfo[] = {
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerBatteryLevel, "GetControllerBatteryLevel" }, // 1807760431
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerOrientationAndPosition, "GetControllerOrientationAndPosition" }, // 2511397107
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetControllerTrackingStatus, "GetControllerTrackingStatus" }, // 1016249914
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_GetSnapdragonVRControllerEventManager, "GetSnapdragonVRControllerEventManager" }, // 3573972080
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_Recenter, "Recenter" }, // 1198085170
		{ &Z_Construct_UFunction_USnapdragonVRControllerFunctionLibrary_SetControllerVibrate, "SetControllerVibrate" }, // 2332022172
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n* Snapdragon VR Controller Extensions Function Library\n*/" },
		{ "IncludePath", "SnapdragonVRControllerFunctionLibrary.h" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerFunctionLibrary.h" },
		{ "ToolTip", "Snapdragon VR Controller Extensions Function Library" },
	};
#endif
	const FCppClassTypeInfoStatic Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USnapdragonVRControllerFunctionLibrary>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::ClassParams = {
		&USnapdragonVRControllerFunctionLibrary::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		FuncInfo,
		nullptr,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		UE_ARRAY_COUNT(FuncInfo),
		0,
		0,
		0x000000A0u,
		METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(USnapdragonVRControllerFunctionLibrary, 1811429735);
	template<> SNAPDRAGONVRCONTROLLER_API UClass* StaticClass<USnapdragonVRControllerFunctionLibrary>()
	{
		return USnapdragonVRControllerFunctionLibrary::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_USnapdragonVRControllerFunctionLibrary(Z_Construct_UClass_USnapdragonVRControllerFunctionLibrary, &USnapdragonVRControllerFunctionLibrary::StaticClass, TEXT("/Script/SnapdragonVRController"), TEXT("USnapdragonVRControllerFunctionLibrary"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USnapdragonVRControllerFunctionLibrary);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
