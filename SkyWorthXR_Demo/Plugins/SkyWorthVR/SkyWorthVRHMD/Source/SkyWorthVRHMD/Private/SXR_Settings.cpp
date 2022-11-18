// Fill out your copyright notice in the Description page of Project Settings.


#include "SXR_Settings.h"
#include "Engine/RendererSettings.h"

USXR_Settings::USXR_Settings(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer),
	bEnableFoveation(false),
	FoveationLevel(EFoveationLevel::Low)
{
}

void USXR_Settings::PostInitProperties()
{
	Super::PostInitProperties();
}

#if WITH_EDITOR
void USXR_Settings::PostEditChangeProperty(FPropertyChangedEvent& PropertyChangedEvent)
{
	Super::PostEditChangeProperty(PropertyChangedEvent);
	Super::UpdateDefaultConfigFile();
}
#endif