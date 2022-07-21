// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SnapdragonVRHMD/Classes/SnapdragonVRHMDEventManager.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSnapdragonVRHMDEventManager() {}
// Cross Module References
	SNAPDRAGONVRHMD_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_SnapdragonVRHMD();
	SNAPDRAGONVRHMD_API UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType();
	SNAPDRAGONVRHMD_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature();
	SNAPDRAGONVRHMD_API UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType();
	SNAPDRAGONVRHMD_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature();
	SNAPDRAGONVRHMD_API UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone();
	SNAPDRAGONVRHMD_API UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel();
	SNAPDRAGONVRHMD_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature();
	SNAPDRAGONVRHMD_API UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType();
	SNAPDRAGONVRHMD_API UClass* Z_Construct_UClass_USnapdragonVRHMDEventManager_NoRegister();
	SNAPDRAGONVRHMD_API UClass* Z_Construct_UClass_USnapdragonVRHMDEventManager();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRHMD_eventSnapdragonVRSensorDelegate_Parms
		{
			ESnapdragonVRSensorEventType Type;
		};
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Type_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Type;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::NewProp_Type_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRHMD_eventSnapdragonVRSensorDelegate_Parms, Type), Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::NewProp_Type_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::NewProp_Type,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD, nullptr, "SnapdragonVRSensorDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRHMD_eventSnapdragonVRSensorDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRHMD_eventSnapdragonVRModeDelegate_Parms
		{
			ESnapdragonVRModeEventType Type;
		};
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Type_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Type;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::NewProp_Type_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRHMD_eventSnapdragonVRModeDelegate_Parms, Type), Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::NewProp_Type_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::NewProp_Type,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD, nullptr, "SnapdragonVRModeDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRHMD_eventSnapdragonVRModeDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRHMD_eventSnapdragonVRThermalDelegate_Parms
		{
			ESnapdragonVRThermalZone Zone;
			ESnapdragonVRThermalLevel Level;
		};
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Zone_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Zone;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Level_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Level;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Zone_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Zone = { "Zone", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRHMD_eventSnapdragonVRThermalDelegate_Parms, Zone), Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Level_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Level = { "Level", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRHMD_eventSnapdragonVRThermalDelegate_Parms, Level), Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Zone_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Zone,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Level_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::NewProp_Level,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD, nullptr, "SnapdragonVRThermalDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRHMD_eventSnapdragonVRThermalDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRHMD_eventSnapdragonVRSdkServiceDelegate_Parms
		{
			ESnapdragonVRSdkServiceEventType Type;
		};
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Type_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Type;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::NewProp_Type_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::NewProp_Type = { "Type", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRHMD_eventSnapdragonVRSdkServiceDelegate_Parms, Type), Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::NewProp_Type_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::NewProp_Type,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD, nullptr, "SnapdragonVRSdkServiceDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRHMD_eventSnapdragonVRSdkServiceDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	static UEnum* ESnapdragonVRThermalZone_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone, Z_Construct_UPackage__Script_SnapdragonVRHMD(), TEXT("ESnapdragonVRThermalZone"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRThermalZone>()
	{
		return ESnapdragonVRThermalZone_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRThermalZone(ESnapdragonVRThermalZone_StaticEnum, TEXT("/Script/SnapdragonVRHMD"), TEXT("ESnapdragonVRThermalZone"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone_Hash() { return 1151388468U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRHMD();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRThermalZone"), 0, Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalZone_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRThermalZone::Cpu", (int64)ESnapdragonVRThermalZone::Cpu },
				{ "ESnapdragonVRThermalZone::Gpu", (int64)ESnapdragonVRThermalZone::Gpu },
				{ "ESnapdragonVRThermalZone::Skin", (int64)ESnapdragonVRThermalZone::Skin },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "Cpu.Name", "ESnapdragonVRThermalZone::Cpu" },
				{ "Gpu.Name", "ESnapdragonVRThermalZone::Gpu" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
				{ "Skin.Name", "ESnapdragonVRThermalZone::Skin" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
				nullptr,
				"ESnapdragonVRThermalZone",
				"ESnapdragonVRThermalZone",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* ESnapdragonVRThermalLevel_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel, Z_Construct_UPackage__Script_SnapdragonVRHMD(), TEXT("ESnapdragonVRThermalLevel"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRThermalLevel>()
	{
		return ESnapdragonVRThermalLevel_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRThermalLevel(ESnapdragonVRThermalLevel_StaticEnum, TEXT("/Script/SnapdragonVRHMD"), TEXT("ESnapdragonVRThermalLevel"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel_Hash() { return 1370336740U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRHMD();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRThermalLevel"), 0, Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRThermalLevel_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRThermalLevel::Safe", (int64)ESnapdragonVRThermalLevel::Safe },
				{ "ESnapdragonVRThermalLevel::Level1", (int64)ESnapdragonVRThermalLevel::Level1 },
				{ "ESnapdragonVRThermalLevel::Level2", (int64)ESnapdragonVRThermalLevel::Level2 },
				{ "ESnapdragonVRThermalLevel::Level3", (int64)ESnapdragonVRThermalLevel::Level3 },
				{ "ESnapdragonVRThermalLevel::Critical", (int64)ESnapdragonVRThermalLevel::Critical },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "Critical.Name", "ESnapdragonVRThermalLevel::Critical" },
				{ "Level1.Name", "ESnapdragonVRThermalLevel::Level1" },
				{ "Level2.Name", "ESnapdragonVRThermalLevel::Level2" },
				{ "Level3.Name", "ESnapdragonVRThermalLevel::Level3" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
				{ "Safe.Name", "ESnapdragonVRThermalLevel::Safe" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
				nullptr,
				"ESnapdragonVRThermalLevel",
				"ESnapdragonVRThermalLevel",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* ESnapdragonVRSensorEventType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType, Z_Construct_UPackage__Script_SnapdragonVRHMD(), TEXT("ESnapdragonVRSensorEventType"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRSensorEventType>()
	{
		return ESnapdragonVRSensorEventType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRSensorEventType(ESnapdragonVRSensorEventType_StaticEnum, TEXT("/Script/SnapdragonVRHMD"), TEXT("ESnapdragonVRSensorEventType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType_Hash() { return 228971798U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRHMD();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRSensorEventType"), 0, Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSensorEventType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRSensorEventType::EventSensorError", (int64)ESnapdragonVRSensorEventType::EventSensorError },
				{ "ESnapdragonVRSensorEventType::EventMagnometerUncalibrated", (int64)ESnapdragonVRSensorEventType::EventMagnometerUncalibrated },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EventMagnometerUncalibrated.Name", "ESnapdragonVRSensorEventType::EventMagnometerUncalibrated" },
				{ "EventSensorError.Name", "ESnapdragonVRSensorEventType::EventSensorError" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
				nullptr,
				"ESnapdragonVRSensorEventType",
				"ESnapdragonVRSensorEventType",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* ESnapdragonVRModeEventType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType, Z_Construct_UPackage__Script_SnapdragonVRHMD(), TEXT("ESnapdragonVRModeEventType"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRModeEventType>()
	{
		return ESnapdragonVRModeEventType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRModeEventType(ESnapdragonVRModeEventType_StaticEnum, TEXT("/Script/SnapdragonVRHMD"), TEXT("ESnapdragonVRModeEventType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType_Hash() { return 3695904719U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRHMD();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRModeEventType"), 0, Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRModeEventType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRModeEventType::EventVrModeStarted", (int64)ESnapdragonVRModeEventType::EventVrModeStarted },
				{ "ESnapdragonVRModeEventType::EventVrModeStopping", (int64)ESnapdragonVRModeEventType::EventVrModeStopping },
				{ "ESnapdragonVRModeEventType::EventVrModeStopped", (int64)ESnapdragonVRModeEventType::EventVrModeStopped },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EventVrModeStarted.Name", "ESnapdragonVRModeEventType::EventVrModeStarted" },
				{ "EventVrModeStopped.Name", "ESnapdragonVRModeEventType::EventVrModeStopped" },
				{ "EventVrModeStopping.Name", "ESnapdragonVRModeEventType::EventVrModeStopping" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
				nullptr,
				"ESnapdragonVRModeEventType",
				"ESnapdragonVRModeEventType",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	static UEnum* ESnapdragonVRSdkServiceEventType_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType, Z_Construct_UPackage__Script_SnapdragonVRHMD(), TEXT("ESnapdragonVRSdkServiceEventType"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRHMD_API UEnum* StaticEnum<ESnapdragonVRSdkServiceEventType>()
	{
		return ESnapdragonVRSdkServiceEventType_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRSdkServiceEventType(ESnapdragonVRSdkServiceEventType_StaticEnum, TEXT("/Script/SnapdragonVRHMD"), TEXT("ESnapdragonVRSdkServiceEventType"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType_Hash() { return 91068524U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRHMD();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRSdkServiceEventType"), 0, Get_Z_Construct_UEnum_SnapdragonVRHMD_ESnapdragonVRSdkServiceEventType_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRSdkServiceEventType::EventSdkServiceStarting", (int64)ESnapdragonVRSdkServiceEventType::EventSdkServiceStarting },
				{ "ESnapdragonVRSdkServiceEventType::EventSdkServiceStarted", (int64)ESnapdragonVRSdkServiceEventType::EventSdkServiceStarted },
				{ "ESnapdragonVRSdkServiceEventType::EventSdkServiceStopped", (int64)ESnapdragonVRSdkServiceEventType::EventSdkServiceStopped },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "EventSdkServiceStarted.Name", "ESnapdragonVRSdkServiceEventType::EventSdkServiceStarted" },
				{ "EventSdkServiceStarting.Name", "ESnapdragonVRSdkServiceEventType::EventSdkServiceStarting" },
				{ "EventSdkServiceStopped.Name", "ESnapdragonVRSdkServiceEventType::EventSdkServiceStopped" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
				nullptr,
				"ESnapdragonVRSdkServiceEventType",
				"ESnapdragonVRSdkServiceEventType",
				Enumerators,
				UE_ARRAY_COUNT(Enumerators),
				RF_Public|RF_Transient|RF_MarkAsNative,
				EEnumFlags::None,
				UE4CodeGen_Private::EDynamicType::NotDynamic,
				(uint8)UEnum::ECppForm::EnumClass,
				METADATA_PARAMS(Enum_MetaDataParams, UE_ARRAY_COUNT(Enum_MetaDataParams))
			};
			UE4CodeGen_Private::ConstructUEnum(ReturnEnum, EnumParams);
		}
		return ReturnEnum;
	}
	void USnapdragonVRHMDEventManager::StaticRegisterNativesUSnapdragonVRHMDEventManager()
	{
	}
	UClass* Z_Construct_UClass_USnapdragonVRHMDEventManager_NoRegister()
	{
		return USnapdragonVRHMDEventManager::StaticClass();
	}
	struct Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnSnapdragonVRSdkServiceDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSnapdragonVRSdkServiceDelegate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnSnapdragonVRThermalDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSnapdragonVRThermalDelegate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnSnapdragonVRModeDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSnapdragonVRModeDelegate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnSnapdragonVRSensorDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnSnapdragonVRSensorDelegate;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_SnapdragonVRHMD,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::Class_MetaDataParams[] = {
		{ "Comment", "/**\n* SnapdragonVRHMD Extensions Function Library\n*/" },
		{ "IncludePath", "SnapdragonVRHMDEventManager.h" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
		{ "ToolTip", "SnapdragonVRHMD Extensions Function Library" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSdkServiceDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSdkServiceDelegate = { "OnSnapdragonVRSdkServiceDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRHMDEventManager, OnSnapdragonVRSdkServiceDelegate), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSdkServiceDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSdkServiceDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSdkServiceDelegate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRThermalDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRThermalDelegate = { "OnSnapdragonVRThermalDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRHMDEventManager, OnSnapdragonVRThermalDelegate), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRThermalDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRThermalDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRThermalDelegate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRModeDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRModeDelegate = { "OnSnapdragonVRModeDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRHMDEventManager, OnSnapdragonVRModeDelegate), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRModeDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRModeDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRModeDelegate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSensorDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRHMDEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSensorDelegate = { "OnSnapdragonVRSensorDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRHMDEventManager, OnSnapdragonVRSensorDelegate), Z_Construct_UDelegateFunction_SnapdragonVRHMD_SnapdragonVRSensorDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSensorDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSensorDelegate_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSdkServiceDelegate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRThermalDelegate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRModeDelegate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::NewProp_OnSnapdragonVRSensorDelegate,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USnapdragonVRHMDEventManager>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::ClassParams = {
		&USnapdragonVRHMDEventManager::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::PropPointers),
		0,
		0x008000A0u,
		METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USnapdragonVRHMDEventManager()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_USnapdragonVRHMDEventManager_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(USnapdragonVRHMDEventManager, 2072201787);
	template<> SNAPDRAGONVRHMD_API UClass* StaticClass<USnapdragonVRHMDEventManager>()
	{
		return USnapdragonVRHMDEventManager::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_USnapdragonVRHMDEventManager(Z_Construct_UClass_USnapdragonVRHMDEventManager, &USnapdragonVRHMDEventManager::StaticClass, TEXT("/Script/SnapdragonVRHMD"), TEXT("USnapdragonVRHMDEventManager"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USnapdragonVRHMDEventManager);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
