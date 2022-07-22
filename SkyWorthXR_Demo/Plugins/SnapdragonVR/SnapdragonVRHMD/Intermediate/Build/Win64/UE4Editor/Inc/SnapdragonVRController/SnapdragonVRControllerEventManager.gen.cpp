// Copyright Epic Games, Inc. All Rights Reserved.
/*===========================================================================
	Generated code exported from UnrealHeaderTool.
	DO NOT modify this manually! Edit the corresponding .h files instead!
===========================================================================*/

#include "UObject/GeneratedCppIncludes.h"
#include "SnapdragonVRController/Classes/SnapdragonVRControllerEventManager.h"
#ifdef _MSC_VER
#pragma warning (push)
#pragma warning (disable : 4883)
#endif
PRAGMA_DISABLE_DEPRECATION_WARNINGS
void EmptyLinkFunctionForGeneratedCodeSnapdragonVRControllerEventManager() {}
// Cross Module References
	SNAPDRAGONVRCONTROLLER_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature();
	UPackage* Z_Construct_UPackage__Script_SnapdragonVRController();
	INPUTCORE_API UEnum* Z_Construct_UEnum_InputCore_EControllerHand();
	SNAPDRAGONVRCONTROLLER_API UEnum* Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState();
	SNAPDRAGONVRCONTROLLER_API UFunction* Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature();
	SNAPDRAGONVRCONTROLLER_API UClass* Z_Construct_UClass_USnapdragonVRControllerEventManager_NoRegister();
	SNAPDRAGONVRCONTROLLER_API UClass* Z_Construct_UClass_USnapdragonVRControllerEventManager();
	COREUOBJECT_API UClass* Z_Construct_UClass_UObject();
// End Cross Module References
	struct Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms
		{
			int32 ControllerIndex;
			EControllerHand hand;
			ESnapdragonVRControllerState NewControllerState;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_hand;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_NewControllerState_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_NewControllerState;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms, ControllerIndex), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_hand = { "hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms, hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_NewControllerState_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_NewControllerState = { "NewControllerState", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms, NewControllerState), Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_hand,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_NewControllerState_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::NewProp_NewControllerState,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRController, nullptr, "SnapdragonVRControllerStateChangeDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRController_eventSnapdragonVRControllerStateChangeDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	struct Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics
	{
		struct _Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms
		{
			int32 ControllerIndex;
			EControllerHand Hand;
		};
		static const UE4CodeGen_Private::FIntPropertyParams NewProp_ControllerIndex;
		static const UE4CodeGen_Private::FBytePropertyParams NewProp_Hand_Underlying;
		static const UE4CodeGen_Private::FEnumPropertyParams NewProp_Hand;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Function_MetaDataParams[];
#endif
		static const UE4CodeGen_Private::FFunctionParams FuncParams;
	};
	const UE4CodeGen_Private::FIntPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_ControllerIndex = { "ControllerIndex", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Int, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms, ControllerIndex), METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FBytePropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_Hand_Underlying = { "UnderlyingType", nullptr, (EPropertyFlags)0x0000000000000000, UE4CodeGen_Private::EPropertyGenFlags::Byte, RF_Public|RF_Transient|RF_MarkAsNative, 1, 0, nullptr, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FEnumPropertyParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_Hand = { "Hand", nullptr, (EPropertyFlags)0x0010000000000080, UE4CodeGen_Private::EPropertyGenFlags::Enum, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(_Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms, Hand), Z_Construct_UEnum_InputCore_EControllerHand, METADATA_PARAMS(nullptr, 0) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_ControllerIndex,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_Hand_Underlying,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::NewProp_Hand,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::Function_MetaDataParams[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FFunctionParams Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::FuncParams = { (UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRController, nullptr, "SnapdragonVRControllerRecenterDelegate__DelegateSignature", nullptr, nullptr, sizeof(_Script_SnapdragonVRController_eventSnapdragonVRControllerRecenterDelegate_Parms), Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::PropPointers, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::PropPointers), RF_Public|RF_Transient|RF_MarkAsNative, (EFunctionFlags)0x00130000, 0, 0, METADATA_PARAMS(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::Function_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::Function_MetaDataParams)) };
	UFunction* Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature()
	{
		static UFunction* ReturnFunction = nullptr;
		if (!ReturnFunction)
		{
			UE4CodeGen_Private::ConstructUFunction(ReturnFunction, Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature_Statics::FuncParams);
		}
		return ReturnFunction;
	}
	static UEnum* ESnapdragonVRControllerState_StaticEnum()
	{
		static UEnum* Singleton = nullptr;
		if (!Singleton)
		{
			Singleton = GetStaticEnum(Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState, Z_Construct_UPackage__Script_SnapdragonVRController(), TEXT("ESnapdragonVRControllerState"));
		}
		return Singleton;
	}
	template<> SNAPDRAGONVRCONTROLLER_API UEnum* StaticEnum<ESnapdragonVRControllerState>()
	{
		return ESnapdragonVRControllerState_StaticEnum();
	}
	static FCompiledInDeferEnum Z_CompiledInDeferEnum_UEnum_ESnapdragonVRControllerState(ESnapdragonVRControllerState_StaticEnum, TEXT("/Script/SnapdragonVRController"), TEXT("ESnapdragonVRControllerState"), false, nullptr, nullptr);
	uint32 Get_Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState_Hash() { return 535879702U; }
	UEnum* Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState()
	{
#if WITH_HOT_RELOAD
		UPackage* Outer = Z_Construct_UPackage__Script_SnapdragonVRController();
		static UEnum* ReturnEnum = FindExistingEnumIfHotReloadOrDynamic(Outer, TEXT("ESnapdragonVRControllerState"), 0, Get_Z_Construct_UEnum_SnapdragonVRController_ESnapdragonVRControllerState_Hash(), false);
#else
		static UEnum* ReturnEnum = nullptr;
#endif // WITH_HOT_RELOAD
		if (!ReturnEnum)
		{
			static const UE4CodeGen_Private::FEnumeratorParam Enumerators[] = {
				{ "ESnapdragonVRControllerState::Disconnected", (int64)ESnapdragonVRControllerState::Disconnected },
				{ "ESnapdragonVRControllerState::Connected", (int64)ESnapdragonVRControllerState::Connected },
				{ "ESnapdragonVRControllerState::Connecting", (int64)ESnapdragonVRControllerState::Connecting },
				{ "ESnapdragonVRControllerState::Error", (int64)ESnapdragonVRControllerState::Error },
			};
#if WITH_METADATA
			const UE4CodeGen_Private::FMetaDataPairParam Enum_MetaDataParams[] = {
				{ "BlueprintType", "true" },
				{ "Connected.Name", "ESnapdragonVRControllerState::Connected" },
				{ "Connecting.Name", "ESnapdragonVRControllerState::Connecting" },
				{ "Disconnected.Name", "ESnapdragonVRControllerState::Disconnected" },
				{ "Error.Name", "ESnapdragonVRControllerState::Error" },
				{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
			};
#endif
			static const UE4CodeGen_Private::FEnumParams EnumParams = {
				(UObject*(*)())Z_Construct_UPackage__Script_SnapdragonVRController,
				nullptr,
				"ESnapdragonVRControllerState",
				"ESnapdragonVRControllerState",
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
	void USnapdragonVRControllerEventManager::StaticRegisterNativesUSnapdragonVRControllerEventManager()
	{
	}
	UClass* Z_Construct_UClass_USnapdragonVRControllerEventManager_NoRegister()
	{
		return USnapdragonVRControllerEventManager::StaticClass();
	}
	struct Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics
	{
		static UObject* (*const DependentSingletons[])();
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam Class_MetaDataParams[];
#endif
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnControllerRecenteredDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnControllerRecenteredDelegate;
#if WITH_METADATA
		static const UE4CodeGen_Private::FMetaDataPairParam NewProp_OnControllerStateChangedDelegate_MetaData[];
#endif
		static const UE4CodeGen_Private::FMulticastDelegatePropertyParams NewProp_OnControllerStateChangedDelegate;
		static const UE4CodeGen_Private::FPropertyParamsBase* const PropPointers[];
		static const FCppClassTypeInfoStatic StaticCppClassTypeInfo;
		static const UE4CodeGen_Private::FClassParams ClassParams;
	};
	UObject* (*const Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::DependentSingletons[])() = {
		(UObject* (*)())Z_Construct_UClass_UObject,
		(UObject* (*)())Z_Construct_UPackage__Script_SnapdragonVRController,
	};
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::Class_MetaDataParams[] = {
		{ "IncludePath", "SnapdragonVRControllerEventManager.h" },
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
	};
#endif
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerRecenteredDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerRecenteredDelegate = { "OnControllerRecenteredDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRControllerEventManager, OnControllerRecenteredDelegate), Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerRecenterDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerRecenteredDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerRecenteredDelegate_MetaData)) };
#if WITH_METADATA
	const UE4CodeGen_Private::FMetaDataPairParam Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerStateChangedDelegate_MetaData[] = {
		{ "ModuleRelativePath", "Classes/SnapdragonVRControllerEventManager.h" },
	};
#endif
	const UE4CodeGen_Private::FMulticastDelegatePropertyParams Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerStateChangedDelegate = { "OnControllerStateChangedDelegate", nullptr, (EPropertyFlags)0x0010000010080000, UE4CodeGen_Private::EPropertyGenFlags::InlineMulticastDelegate, RF_Public|RF_Transient|RF_MarkAsNative, 1, STRUCT_OFFSET(USnapdragonVRControllerEventManager, OnControllerStateChangedDelegate), Z_Construct_UDelegateFunction_SnapdragonVRController_SnapdragonVRControllerStateChangeDelegate__DelegateSignature, METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerStateChangedDelegate_MetaData, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerStateChangedDelegate_MetaData)) };
	const UE4CodeGen_Private::FPropertyParamsBase* const Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::PropPointers[] = {
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerRecenteredDelegate,
		(const UE4CodeGen_Private::FPropertyParamsBase*)&Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::NewProp_OnControllerStateChangedDelegate,
	};
	const FCppClassTypeInfoStatic Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::StaticCppClassTypeInfo = {
		TCppClassTypeTraits<USnapdragonVRControllerEventManager>::IsAbstract,
	};
	const UE4CodeGen_Private::FClassParams Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::ClassParams = {
		&USnapdragonVRControllerEventManager::StaticClass,
		nullptr,
		&StaticCppClassTypeInfo,
		DependentSingletons,
		nullptr,
		Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::PropPointers,
		nullptr,
		UE_ARRAY_COUNT(DependentSingletons),
		0,
		UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::PropPointers),
		0,
		0x008000A0u,
		METADATA_PARAMS(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::Class_MetaDataParams, UE_ARRAY_COUNT(Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::Class_MetaDataParams))
	};
	UClass* Z_Construct_UClass_USnapdragonVRControllerEventManager()
	{
		static UClass* OuterClass = nullptr;
		if (!OuterClass)
		{
			UE4CodeGen_Private::ConstructUClass(OuterClass, Z_Construct_UClass_USnapdragonVRControllerEventManager_Statics::ClassParams);
		}
		return OuterClass;
	}
	IMPLEMENT_CLASS(USnapdragonVRControllerEventManager, 1849326646);
	template<> SNAPDRAGONVRCONTROLLER_API UClass* StaticClass<USnapdragonVRControllerEventManager>()
	{
		return USnapdragonVRControllerEventManager::StaticClass();
	}
	static FCompiledInDefer Z_CompiledInDefer_UClass_USnapdragonVRControllerEventManager(Z_Construct_UClass_USnapdragonVRControllerEventManager, &USnapdragonVRControllerEventManager::StaticClass, TEXT("/Script/SnapdragonVRController"), TEXT("USnapdragonVRControllerEventManager"), false, nullptr, nullptr, nullptr);
	DEFINE_VTABLE_PTR_HELPER_CTOR(USnapdragonVRControllerEventManager);
PRAGMA_ENABLE_DEPRECATION_WARNINGS
#ifdef _MSC_VER
#pragma warning (pop)
#endif
