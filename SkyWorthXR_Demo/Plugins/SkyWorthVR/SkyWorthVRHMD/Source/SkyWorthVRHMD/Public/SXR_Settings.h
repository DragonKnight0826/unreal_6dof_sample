// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/SoftObjectPath.h"
#include "SXR_Settings.generated.h"

UENUM()
namespace EFoveationLevel
{
	enum Type
	{
		Low,
		Med,
		High,
		TopHigh
	};
}

/**
 * 
 */
UCLASS(config = Engine, defaultconfig)
class SKYWORTHVRHMD_API USXR_Settings : public UObject
{
	GENERATED_UCLASS_BODY()

public:
	UPROPERTY(Config, EditAnywhere, Category = Feature, Meta = (DisplayName = "Enable Foveation Rendering"))
		bool bEnableFoveation;

	UPROPERTY(Config, EditAnywhere, Category = Feature, Meta = (EditCondition = "bEnableFoveation", DisplayName = "Foveation Level"))
		TEnumAsByte<EFoveationLevel::Type> FoveationLevel;

	virtual void PostInitProperties() override;

#if WITH_EDITOR
	virtual void PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent) override;
#endif 
};
