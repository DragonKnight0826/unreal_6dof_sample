//=============================================================================
// FILE: SnapdragonVRHMD_CustomPresent_OpenGLES.cpp
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================
#include "SnapdragonVRHMD_CustomPresent.h"
#include "SnapdragonVRHMD_TextureSetProxy.h"
#include "SnapdragonXR_CVars.h"

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_OPENGLES

#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"

#define dbgprint(format, ...) //FPlatformMisc::LowLevelOutputDebugStringf(*FString::Printf(TEXT("%s line %d: %s"), TEXT(__FILE__), __LINE__, *FString::Printf(TEXT(format), ##__VA_ARGS__)))

extern void GetExtensionsString(FString& ExtensionsString);

// GL_QCOM_texture_foveated
PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC glTextureFoveationParametersQCOM = NULL;

class FOpenGLESCustomPresentSVR : public FSnapdragonVRHMDCustomPresent
{
public:
	FOpenGLESCustomPresentSVR(FSnapdragonVRHMD* pHMD)
		: FSnapdragonVRHMDCustomPresent(pHMD)
		, mTextureSet(nullptr)
		, mMaxFoveatedFocalPoints(0)
		, ReticleTexture(nullptr)
	{
		// Create texture for reticle layer
		FOpenGLDynamicRHI* OpenGLRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);
		FRHIResourceCreateInfo CreateInfo(TEXT("SnapdragonVRHMD_CustomPresent_OpenGLES"));
		ReticleTexture = OpenGLRHI->RHICreateTexture2D(
			5, 
			5, 
			PF_B8G8R8A8, 
			1, 
			1, 
			TexCreate_ShaderResource, 

#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION == 5
			ERHIAccess::Unknown,
#endif
			CreateInfo);

		if (IsInRenderingThread() || IsInRHIThread())
		{
			uint32 Stride;
			uint8* ReticleTextureBuffer = (uint8*)OpenGLRHI->RHILockTexture2D(ReticleTexture, 0, RLM_WriteOnly, Stride, false);
			static uint32 ReticleData[] =
			{
				0xffff00ff, 0xffff00ff, 0xffff00ff, 0xffff00ff, 0xffff00ff,
				0xffff00ff, 0x00000000, 0x00000000, 0x00000000, 0xffff00ff,
				0xffff00ff, 0x00000000, 0x00000000, 0x00000000, 0xffff00ff,
				0xffff00ff, 0x00000000, 0x00000000, 0x00000000, 0xffff00ff,
				0xffff00ff, 0xffff00ff, 0xffff00ff, 0xffff00ff, 0xffff00ff,
			};

			uint32 Index = 0;
			for (int32 y = 0; y < 5; ++y)
			{
				for (int32 x = 0; x < 5; ++x)
				{
					uint32* TexelData = (uint32*)(ReticleTextureBuffer + x * sizeof(uint32) + y * Stride);
					*TexelData = ReticleData[Index++];
				}
			}
			OpenGLRHI->RHIUnlockTexture2D(ReticleTexture, 0, false);
		}

		CachedFocusPoints[0] = CachedFocusPoints[1] = FVector2D::ZeroVector;
	}

	~FOpenGLESCustomPresentSVR()
	{
		// DARCHARD: TODO -fixme
		//delete mTextureSet;
	}

	void SubmitFrame(const FPoseStateFrame& PoseState) override;

	virtual bool AllocateRenderTargetTexture(
		uint32 SizeX,
		uint32 SizeY,
		uint8 Format,
		uint32 NumMips, 
		ETextureCreateFlags Flags,
		ETextureCreateFlags TargetableTextureFlags,
		FTexture2DRHIRef& OutTargetableTexture,
		FTexture2DRHIRef& OutShaderResourceTexture,
		uint32 NumSamples) override;

private:
	class FSnapdragonVRTextureSet_OpenGL* mTextureSet;

	uint32 mMaxFoveatedFocalPoints;

	FTexture2DRHIRef ReticleTexture;

	// If eye-tracking is enabled and no valid data is available for either eye this frame, fall back to the last known gaze points
	FVector2D CachedFocusPoints[2];
};

FSnapdragonVRHMDCustomPresent* CreateCustomPresent_OpenGLES(class FSnapdragonVRHMD* pHMD)
{
	return new FOpenGLESCustomPresentSVR(pHMD);
}

bool FOpenGLESCustomPresentSVR::AllocateRenderTargetTexture(
	uint32 SizeX,
	uint32 SizeY,
	uint8 Format,
	uint32 NumMips, 
	ETextureCreateFlags Flags,
	ETextureCreateFlags TargetableTextureFlags,
	FTexture2DRHIRef& OutTargetableTexture,
	FTexture2DRHIRef& OutShaderResourceTexture,
	uint32 NumSamples)
{
	FOpenGLDynamicRHI* OpenGLRHI = static_cast<FOpenGLDynamicRHI*>(GDynamicRHI);

	//FDebug::DumpStackTraceToLog(); // RBF
	dbgprint("FOpenGLESCustomPresentSVR::AllocateRenderTargetTexture called");

	FString ExtensionsString;
	GetExtensionsString(ExtensionsString);

	bool bSupportsTextureBasedFoveation = ExtensionsString.Contains(TEXT("GL_QCOM_texture_foveated"));
	if (bSupportsTextureBasedFoveation)
	{
		dbgprint("GL_QCOM_texture_foveated: Extension string is exposed");

		// CTORNE: This is global state, it doesn't matter what texture target we pass in and whether an actual texture is bound.
		// The extension should be changed to rely on the global glGetIntegerv query instead.
		GLint QueryResult = 0;
		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_FOVEATED_FEATURE_QUERY_QCOM, &QueryResult);

		checkf((QueryResult & GL_FOVEATION_ENABLE_BIT_QCOM) == GL_FOVEATION_ENABLE_BIT_QCOM &&
			   (QueryResult & GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM) == GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM,
			   TEXT("GL_QCOM_texture_foveated: The implementation does not support the scaled bin foveation method despite exposing the extension!"));
		
		dbgprint("GL_QCOM_texture_foveated: Scaled bin foveation method supported");

		float DefaultMinFoveatedPixelDensity = -1.f;
		glGetTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM, &DefaultMinFoveatedPixelDensity);

		checkf(DefaultMinFoveatedPixelDensity >= 0.f && DefaultMinFoveatedPixelDensity <= 1.f,
			   TEXT("GL_QCOM_texture_foveated: The minimum pixel density supported (%f), should be [0.f;1.f]!"), DefaultMinFoveatedPixelDensity);

		glGetTexParameteriv(GL_TEXTURE_2D, GL_TEXTURE_FOVEATED_NUM_FOCAL_POINTS_QUERY_QCOM, &QueryResult);

		checkf(QueryResult > 0,
			   TEXT("GL_QCOM_texture_foveated: The Maximum number of focal points per texture layer (%d), should be >= 1!"), mMaxFoveatedFocalPoints);
		
		mMaxFoveatedFocalPoints = static_cast<GLuint>(QueryResult);
		dbgprint("GL_QCOM_texture_foveated: Maximum number of focal points per texture layer = %u", mMaxFoveatedFocalPoints);

		glTextureFoveationParametersQCOM = (PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC)((void*)eglGetProcAddress("glTextureFoveationParametersQCOM"));

		checkf(glTextureFoveationParametersQCOM != nullptr,
			   TEXT("Couldn't find entry point glTextureFoveationParametersQCOM!"));
	}
	else
	{
		CVars::TextureFoveationEnabled = 0;
	}

	static const auto MobileMultiViewCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView"));
	static const bool bIsUsingMobileMultiView = GSupportsMobileMultiView && MobileMultiViewCVar && MobileMultiViewCVar->GetValueOnAnyThread() != 0;

	static const auto MobileMultiViewDirectCVar = IConsoleManager::Get().FindTConsoleVariableDataInt(TEXT("vr.MobileMultiView.Direct"));
	static const bool bIsUsingMobileMultiViewDirect = bIsUsingMobileMultiView && MobileMultiViewDirectCVar && MobileMultiViewDirectCVar->GetValueOnAnyThread() != 0;

	GLenum Target = GL_TEXTURE_2D;
	uint32 Layers = 1;
	if (bIsUsingMobileMultiViewDirect)
	{
		Target = GL_TEXTURE_2D_ARRAY;
		Layers = 2;
		bUsesArrayTexture = true;
	}

	uint32 Width = SizeX / Layers;

	mTextureSet = CreateTextureSetProxy_OpenGLES(
		OpenGLRHI,
		Target,
		Width,
		SizeY,
		Layers,
		Format,
		Flags,
		TargetableTextureFlags,
		OutTargetableTexture,
		OutShaderResourceTexture);

	return true;
}

extern GLuint TextureSetProxy_OpenGLES_GetNativeResource(class FSnapdragonVRTextureSet_OpenGL* TextureSetProxy);
extern void TextureSetProxy_SwitchToNextElement(FSnapdragonVRTextureSet_OpenGL* TextureSetProxy);

void FOpenGLESCustomPresentSVR::SubmitFrame(const FPoseStateFrame& PoseState)
{
	GSXRFrameParams FrameParams;
	memset(&FrameParams, 0, sizeof(FrameParams));
	FrameParams.frameIndex = (int32)PoseState.FrameNumber;
	GLuint TextureHandle = TextureSetProxy_OpenGLES_GetNativeResource(mTextureSet);
	// UE_LOG(LogSVR, Log, TEXT(" FOpenGLESCustomPresentSVR::SubmitFrame - pose is Position = x:%5.2f, y:%5.2f, z:%5.2f    Rotation=[%5.3f,%5.3f,%5.3f,%5.3f]"),
	// 	PoseState.Pose.pose.position.x, PoseState.Pose.pose.position.y, PoseState.Pose.pose.position.z,
	// 	PoseState.Pose.pose.rotation.x, PoseState.Pose.pose.rotation.y, PoseState.Pose.pose.rotation.z, PoseState.Pose.pose.rotation.w
	// );
		/// RBF TODO
	if (UsesArrayTexture())
	{
		// Left Eye
		FrameParams.renderLayers[0].imageType = kGSXRTypeTextureArray;
		FrameParams.renderLayers[0].imageHandle = TextureHandle;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].eyeMask = kGSXREyeMaskLeft;
		FrameParams.renderLayers[0].layerFlags |= kGSXRLayerFlagOpaque;

		// Right Eye
		FrameParams.renderLayers[1].imageType = kGSXRTypeTextureArray;
		FrameParams.renderLayers[1].imageHandle = TextureHandle;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].eyeMask = kGSXREyeMaskRight;
		FrameParams.renderLayers[1].layerFlags |= kGSXRLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - rendering texture ID %d - from a texture array"), TextureHandle);

	}
	else
	{
		FrameParams.renderLayers[0].imageHandle = TextureHandle;
		FrameParams.renderLayers[0].imageType = kGSXRTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[0].imageCoords);
		FrameParams.renderLayers[0].imageCoords.LowerUVs[2] = 0.5f;
		FrameParams.renderLayers[0].imageCoords.UpperUVs[2] = 0.5f;
		FrameParams.renderLayers[0].eyeMask = kGSXREyeMaskLeft;
		FrameParams.renderLayers[0].layerFlags |= kGSXRLayerFlagOpaque;

		FrameParams.renderLayers[1].imageHandle = TextureHandle;
		FrameParams.renderLayers[1].imageType = kGSXRTypeTexture;
		CreateLayout(0.0f, 0.0f, 1.0f, 1.0f, &FrameParams.renderLayers[1].imageCoords);
		FrameParams.renderLayers[1].imageCoords.LowerUVs[0] = 0.5f;
		FrameParams.renderLayers[1].imageCoords.UpperUVs[0] = 0.5f;
		FrameParams.renderLayers[1].eyeMask = kGSXREyeMaskRight;
		FrameParams.renderLayers[1].layerFlags |= kGSXRLayerFlagOpaque;
		// UE_LOG(LogSVR, Log, TEXT("FOpenGLESCustomPresentSVR::SubmitFrame - not using array"));
	}

	// Focal point is provided in normalized device coordinates, (-1,-1) is the lower left and (+1,+1) is the upper right corner of our render target
	FVector2D FocusPoints[2] = { CachedFocusPoints[0], CachedFocusPoints[1] };

	// Only update the focus point if required (foveation is enabled and/or we're drawing the eye marker)
	if (CVars::TextureFoveationEnabled != 0 || CVars::EyeMarkerEnabled != 0)
	{
		if (CVars::EyeTrackingEnabled != 0)
		{
			GSXREyePoseState EyePose;
			memset(&EyePose, 0, sizeof(EyePose));

			if ( SC::GSXR_nativeGetEyePose(&EyePose) != GSXR_ERROR_NONE )
			{
				// Yes yes, but if you take the HMD off it crashes the service and you need to reboot the device
				// so, F that
				goto skipFoveationAndEyeTracking;
			}

			dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - eye pose");


			if (CVars::LogEyePoseData)
			{
#define AppendBitFieldName(BitField, Name) \
				if ((status & BitField) == BitField)\
				{\
					status &= ~BitField;\
					if (String.Len() > 0)\
					{\
						String += TEXT("|");\
					}\
					String += Name;\
				}

				auto GetEyeStatusString = [](int32_t status)->FString
				{
					if (status == 0)
					{
						return TEXT("NO VALID DATA");
					}
					FString String;
					AppendBitFieldName(kGSXRGazePointValid, TEXT("GazePointValid"));
					AppendBitFieldName(kGSXRGazeVectorValid, TEXT("GazeVectorValid"));
					AppendBitFieldName(kGSXREyeOpennessValid, TEXT("EyeOpennessValid"));
					AppendBitFieldName(kGSXREyePupilDilationValid, TEXT("EyePupilDilationValid"));
					AppendBitFieldName(kGSXREyePositionGuideValid, TEXT("EyePositionGuideValid"));
					if (status != 0)
					{
						FString Unknown = FString::Printf(TEXT("Unknown (%d)"), status);
						AppendBitFieldName(status, Unknown);
					}
					return String;
				};

#undef AppendBitFieldName

				dbgprint(
					"Frame %llu, EyePose:\n"
					"	leftEyePoseStatus = %d (%s), rightEyePoseStatus = %d (%s), combinedEyePoseStatus = %d (%s)\n"
					"	leftEyeGazeVector = (%f, %f, %f), rightEyeGazeVector = (%f, %f, %f), combinedEyeGazeVector = (%f, %f, %f)\n",
					GFrameNumber,
					EyePose.leftEyePoseStatus, *GetEyeStatusString(EyePose.leftEyePoseStatus),
					EyePose.rightEyePoseStatus, *GetEyeStatusString(EyePose.rightEyePoseStatus),
					EyePose.combinedEyePoseStatus, *GetEyeStatusString(EyePose.combinedEyePoseStatus),
					EyePose.leftEyeGazeVector[0], EyePose.leftEyeGazeVector[1], EyePose.leftEyeGazeVector[2],
					EyePose.rightEyeGazeVector[0], EyePose.rightEyeGazeVector[1], EyePose.rightEyeGazeVector[2],
					EyePose.combinedEyeGazeVector[0], EyePose.combinedEyeGazeVector[1], EyePose.combinedEyeGazeVector[2]
				);
			}

			FVector EyeDirections[3] =
			{
				FVector(EyePose.leftEyeGazeVector[0], EyePose.leftEyeGazeVector[1], -EyePose.leftEyeGazeVector[2]),
				FVector(EyePose.rightEyeGazeVector[0], EyePose.rightEyeGazeVector[1], -EyePose.rightEyeGazeVector[2]),
				FVector(EyePose.combinedEyeGazeVector[0], EyePose.combinedEyeGazeVector[1], -EyePose.combinedEyeGazeVector[2])
			};

			bool bUsesCombinedGazeDirection = (CVars::AverageGazeDirectionX == CVars::kUseCombinedGazeData || CVars::AverageGazeDirectionY == CVars::kUseCombinedGazeData);
			bool bUsesIndividualGazeDirections = !(CVars::AverageGazeDirectionX == CVars::kUseCombinedGazeData && CVars::AverageGazeDirectionY == CVars::kUseCombinedGazeData);

			bool bGazeVectorValid[3] =
			{
				(EyePose.leftEyePoseStatus & kGSXRGazeVectorValid) != 0,
				(EyePose.rightEyePoseStatus & kGSXRGazeVectorValid) != 0,
				(EyePose.combinedEyePoseStatus & kGSXRGazeVectorValid) != 0
			};

			bool bUseCachedFocalPoints = false;

			if (bUsesCombinedGazeDirection && !bGazeVectorValid[2])
			{
				if (CVars::LogEyePoseData)
				{
					UE_LOG(LogSVR, Warning, TEXT("Combined eye gaze vector data is invalid."));
				}
				bUseCachedFocalPoints = true;
				dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - bUsesCombinedGazeDirection");
			}

			if (bUsesIndividualGazeDirections)
			{
				dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 1");
				if (!bGazeVectorValid[0])
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 1a");
					if (CVars::LogEyePoseData)
					{
						UE_LOG(LogSVR, Warning, TEXT("Left eye gaze vector data is invalid."));
					}
					bUseCachedFocalPoints = true;
				}

				if (!bGazeVectorValid[1])
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 2");
					if (CVars::LogEyePoseData)
					{
						UE_LOG(LogSVR, Warning, TEXT("Right eye gaze vector data is invalid."));
					}
					bUseCachedFocalPoints = true;
				}
			}			

			if (!bUseCachedFocalPoints)
			{
				dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 3");
				if (CVars::AverageGazeDirectionX == CVars::kUseCombinedGazeData)
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 3a");
					EyeDirections[0].X = EyeDirections[1].X = EyeDirections[2].X;
				}
				else if (CVars::AverageGazeDirectionX == CVars::kComputeAverageGaze)
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 3b");
					EyeDirections[0].X = EyeDirections[1].X = 0.5f * (EyeDirections[0].X + EyeDirections[1].X);
				}

				if (CVars::AverageGazeDirectionY == CVars::kUseCombinedGazeData)
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 3c");
					EyeDirections[0].Y = EyeDirections[1].Y = EyeDirections[2].Y;
				}
				else if (CVars::AverageGazeDirectionY == CVars::kComputeAverageGaze)
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 3d");
					EyeDirections[0].Y = EyeDirections[1].Y = 0.5f * (EyeDirections[0].Y + EyeDirections[1].Y);
				}

				GSXRDeviceInfo DeviceInfo = SC::GSXR_nativeGetDeviceInfo();

				float NearPlanesDistances[2] = { DeviceInfo.leftEyeFrustum.near, DeviceInfo.rightEyeFrustum.near };

				FVector4 NearPlaneExtents[2] =
				{
					FVector4(DeviceInfo.leftEyeFrustum.left, DeviceInfo.leftEyeFrustum.right, DeviceInfo.leftEyeFrustum.bottom, DeviceInfo.leftEyeFrustum.top),
					FVector4(DeviceInfo.rightEyeFrustum.left, DeviceInfo.rightEyeFrustum.right, DeviceInfo.rightEyeFrustum.bottom, DeviceInfo.rightEyeFrustum.top)
				};

				for (uint32 EyeIndex = 0; EyeIndex < 2; EyeIndex++)
				{
					dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 4");
					if (!FMath::IsNearlyZero(EyeDirections[EyeIndex].Z) && bGazeVectorValid[EyeIndex])
					{
						dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 4a");

						// Determine the intersection of the gaze direction with the near plane using similar triangles:
						//
						//   GazeDir.Z   GazeDir.X   GazeDir.Y
						//   --------- = --------- = ---------
						//      Near         X           Y    
						//
						//   Where Near is the distance to the near plane, i.e. its Z coordinate, and (X,Y) is the intersection point on the near plane
						FVector2D Intersection;
						Intersection.X = (NearPlanesDistances[EyeIndex] * EyeDirections[EyeIndex].X) / EyeDirections[EyeIndex].Z;
						Intersection.Y = (NearPlanesDistances[EyeIndex] * EyeDirections[EyeIndex].Y) / EyeDirections[EyeIndex].Z;

						// Now we need to remap the intersection X coordinate from [Left;Right] and the Y coordinate from [Bottom;Top] to [-1;+1]
						// Note that our eye frusta can be asymmetrical, so left != (-right) necessarily (same for bottom and top).

						// X in [L,R] -> x in [-1,1] => x == -1 + (X-L)*(1-(-1))/(R-L)
						check(NearPlaneExtents[EyeIndex].X != NearPlaneExtents[EyeIndex].Y);
						FocusPoints[EyeIndex].X = -1.f + 2.f*(Intersection.X - NearPlaneExtents[EyeIndex].X) / (NearPlaneExtents[EyeIndex].Y - NearPlaneExtents[EyeIndex].X);

						// Y in [B,T] -> y in [-1,1] => y == -1 + (Y-B)*(1-(-1))/(T-B)
						check(NearPlaneExtents[EyeIndex].Z != NearPlaneExtents[EyeIndex].W);
						FocusPoints[EyeIndex].Y = -1.f + 2.f*(Intersection.Y - NearPlaneExtents[EyeIndex].Z) / (NearPlaneExtents[EyeIndex].W - NearPlaneExtents[EyeIndex].Z);

						CachedFocusPoints[EyeIndex] = FocusPoints[EyeIndex];
					}
				}
			}
		} // 		if (CVars::EyeTrackingEnabled != 0)
		else if (CVars::TextureFoveationFocusEnabled != 0)
		{
			dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 5");
			float Time = GRenderingRealtimeClock.GetCurrentTime();
			float Radius = CVars::TextureFoveationFocusAmplitude * FMath::Abs(FMath::Cos(Time * CVars::TextureFoveationFocusFrequencyRho));

			FocusPoints[0].X = Radius * FMath::Cos(Time * CVars::TextureFoveationFocusFrequency);
			FocusPoints[0].Y = Radius * FMath::Sin(Time * CVars::TextureFoveationFocusFrequency);

			// They're all the same point
			CachedFocusPoints[0] = CachedFocusPoints[1] = FocusPoints[1] = FocusPoints[0];
		}
	} // 	if (TextureFoveationEnabled != 0 || CVars::EyeMarkerEnabled != 0)

	if (CVars::TextureFoveationEnabled != 0)
	{
		if (UsesArrayTexture())
		{
			dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 6");
			glTextureFoveationParametersQCOM(TextureHandle,         // texture
											 0,						// layer
											 0,						// focalPoint
											 FocusPoints[0].X,		// focalX
											 FocusPoints[0].Y,		// focalY
											 CVars::TextureFoveationGainX, // gainX
											 CVars::TextureFoveationGainY,	// gainY
											 CVars::TextureFoveationArea	// foveaArea
			);

			glTextureFoveationParametersQCOM(TextureHandle,         // texture
											 1,						// layer
											 0,						// focalPoint
											 FocusPoints[1].X,		// focalX
											 FocusPoints[1].Y,		// focalY
										CVars::TextureFoveationGainX, // gainX
										CVars::TextureFoveationGainY,	// gainY
										CVars::TextureFoveationArea	// foveaArea
			);
		}
		else
		{
			dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 6a");
			// Make sure we support specifying 2 focal points
			checkf(mMaxFoveatedFocalPoints >= 2, TEXT("GL_QCOM_texture_foveated: Need 2 focal points for double-wide render target!"));
			
			// In our case, the focal points will be (-0.5, 0.0) and (+0.5, 0.0), respectively. X-axis gain should twice Y-axis gain to compensate for the width
			glTextureFoveationParametersQCOM(TextureHandle,						// texture
											 0,									// layer
											 0,									// focalPoint
											 -0.5f + FocusPoints[0].X * 0.5f,	// focalX
											 FocusPoints[0].Y,					// focalY
											 CVars::TextureFoveationGainX * 2.f,		// gainX
											 CVars::TextureFoveationGainY,				// gainY
											 CVars::TextureFoveationArea		// foveaArea
			);

			glTextureFoveationParametersQCOM(TextureHandle,						// texture
											 0,									// layer
											 1,									// focalPoint
											 0.5f + FocusPoints[1].X * 0.5f,	// focalX
											 FocusPoints[1].Y,					// focalY
											 CVars::TextureFoveationGainX * 2.f,		// gainX
											 CVars::TextureFoveationGainY,				// gainY
											 CVars::TextureFoveationArea		// foveaArea
			);
		}
	}

	if (CVars::EyeMarkerEnabled != 0)
	{
		dbgprint("FOpenGLESCustomPresentSVR::SubmitFrame - 7");
		FrameParams.renderLayers[2].imageHandle = *(GLuint*)ReticleTexture->GetNativeResource();
		FrameParams.renderLayers[2].imageType = kGSXRTypeTexture;
		CreateLayout(FocusPoints[0].X, FocusPoints[0].Y, CVars::EyeMarkerRadius, CVars::EyeMarkerRadius, &FrameParams.renderLayers[2].imageCoords);
		FrameParams.renderLayers[2].eyeMask = kGSXREyeMaskLeft;
		FrameParams.renderLayers[2].layerFlags |= kGSXRLayerFlagHeadLocked;

		FrameParams.renderLayers[3].imageHandle = *(GLuint*)ReticleTexture->GetNativeResource();
		FrameParams.renderLayers[3].imageType = kGSXRTypeTexture;
		CreateLayout(FocusPoints[1].X, FocusPoints[1].Y, CVars::EyeMarkerRadius, CVars::EyeMarkerRadius, &FrameParams.renderLayers[3].imageCoords);
		FrameParams.renderLayers[3].eyeMask = kGSXREyeMaskRight;
		FrameParams.renderLayers[3].layerFlags |= kGSXRLayerFlagHeadLocked;
	}

	skipFoveationAndEyeTracking:

	FrameParams.headPoseState = PoseState.Pose;
	FrameParams.minVsyncs = 1;

	// sxrprint("(%s) (Frame %d) SubmitFrame(mRenderPose) => Calling SC::GSXR_nativeSubmitDataFrame(Frame %d)", IsInRenderingThread() ? TEXT("Render") : TEXT("Game"), GFrameNumber, FrameParams.frameIndex);

	SC::GSXR_nativeSubmitDataFrame(&FrameParams);
	TextureSetProxy_SwitchToNextElement(mTextureSet);
}

#endif //SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_OPENGLES