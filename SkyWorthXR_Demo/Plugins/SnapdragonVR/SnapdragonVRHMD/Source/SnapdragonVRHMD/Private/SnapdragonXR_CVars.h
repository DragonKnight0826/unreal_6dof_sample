//=============================================================================
//
//                  Copyright (c) 2019 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

// DO NOT uncomment this - Unreal will not see the #define in the Cvars.cpp file, so you get undefined vars
//#pragma once


//-----------------------------------------------------------------------------
#include "Core/Public/Misc/CoreMisc.h"
#include "Math/Vector.h"
#include "Math/Quat.h"

// make a macro that will define for headers and declare in a CPP file
// based upon if DEFINE_CVAR is defined  - see SnapdragonXR_Cvars.cpp

#ifndef  DEFINE_CVAR // For header files
#define CVR_EXTRN(TYPE,NAME,INIT_VAL) extern TYPE NAME
//#define CVR_EXTRN(TYPE,NAME, ... ) extern TYPE NAME
#else                // for a single cpp file
	#define CVR_EXTRN(TYPE,NAME,INIT_VAL)  TYPE NAME = INIT_VAL
//	#define CVR_EXTRN(TYPE,NAME, ...)  TYPE NAME = TYPE( __VA_ARGS__ )
#endif

// When the compiler gets C++17 compatibility we can try to clean this up (not having to have one declarations and the rest definitions)
// note that the variadic macros would be fine but it's Unreals header generator that's not exactly c++ compliant (see above)

// unmacroable things go here

// #ifndef  DEFINE_CVAR // For header files
// 	extern FQuat SXRFrustumRotationQuat;
// 	extern FVector SXRFrustumPosition;
// #else // C++ files
// 	FQuat SXRFrustumRotationQuat(0, 0, 0, 1.f);
// 	FVector SXRFrustumPosition = FVector(0, 0, 0);
// #endif 

namespace CVars
{
	// misc useful functions and stuff
	uint32 ParseIntoFloats(FString& TheString, float* loc, const uint32 maxNumber);

#ifndef  DEFINE_CVAR // For header files
#ifndef INCLUDE_CVARS
#define INCLUDE_CVARS
	enum AverageGazeMode
	{
		kNoAverage = 0,
		kUseCombinedGazeData,
		kComputeAverageGaze
		};
#endif
#endif

	// the variables

	CVR_EXTRN(int32, TextureFoveationEnabled, 0);
	CVR_EXTRN(float, TextureFoveationArea, 0.0f);
	CVR_EXTRN(float, TextureFoveationGainX, 0.0f);
	CVR_EXTRN(float, TextureFoveationGainY, 0.0f);
	CVR_EXTRN(float, TextureFoveationMinPixelDensity, 0.0f);
	CVR_EXTRN(int32, TemporalFoveationEnabled, 0);

	CVR_EXTRN(int32, TextureFoveationFocusAmplitude, 0);
	CVR_EXTRN(int32, TextureFoveationFocusEnabled, 0);
	CVR_EXTRN(float, TextureFoveationFocusFrequency, 0.0f);
	CVR_EXTRN(float, TextureFoveationFocusFrequencyRho, 0.0f);



	CVR_EXTRN(int32, EyeTrackingEnabled, 1);
	CVR_EXTRN(int32, AverageGazeDirectionX, 0);
	CVR_EXTRN(int32, AverageGazeDirectionY, 0);
	CVR_EXTRN(int32, EyeMarkerEnabled, 0);
	CVR_EXTRN(float, EyeMarkerRadius, 0.0f);
	CVR_EXTRN(int32, LogEyePoseData, 0);

	CVR_EXTRN(int32, GNumSwapchainImages, 3);

	//CVR_EXTRN(FString, SXRViewFrustumLeft_string, "0.1 0.2 0.3 0.4 0.5 0.6 0.7"); // position (3 floats) then quaternion (4 floats)
}


#undef CVR_EXTRN



