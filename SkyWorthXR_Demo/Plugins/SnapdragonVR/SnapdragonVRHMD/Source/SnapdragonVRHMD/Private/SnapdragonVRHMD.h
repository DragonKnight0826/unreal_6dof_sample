//=============================================================================
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================

#pragma once


#include "XRRenderTargetManager.h"
#include "IStereoLayers.h"
#include "ISnapdragonVRHMDModule.h"
#include "XRRenderBridge.h"
#include "FGSXRRenderBridge.h"
#include "FGSXRStereoLayer.h"

//#if WITH_TELEMETRY
//#include "ThirdParty/Telemetry/3.0.1.903/include/rad_tmgpu.h"
//#endif

#include "SCGSXRApi.h"
#include "SceneViewExtension.h"
#include "SnapdragonVRHMD_CustomPresent.h"

#define TextureQueueLength 3

struct FFrameSensorState
{
	float PredictedDisplayTime;
	FVector Position;
	FQuat Orientation;
	GSXRHeadPoseState headPoseState;
};

//-----------------------------------------------------------------------------
class FSnapdragonVRHMD : public FHeadMountedDisplayBase, public FXRRenderTargetManager, public FSceneViewExtensionBase, public IStereoLayers
{
public:
	//friend class FSceneViewExtensions;

	FSnapdragonVRHMD(const FAutoRegister&);
	/** Constructor */
//	FSnapdragonVRHMD(ISnapdragonVRHMDPlugin* SnapdragonPlugin);


	virtual ~FSnapdragonVRHMD();


	static FSnapdragonVRHMD* GetSnapdragonHMD();

	//////////////////////////////////////////////////////
	// IXRSystemIdentifier
	//////////////////////////////////////////////////////

	static const FName SnapdragonVRHMDSystemName;

	virtual FName GetSystemName() const override
	{
		return SnapdragonVRHMDSystemName;
	}
#if ENGINE_MINOR_VERSION >25
	virtual int32 GetXRSystemFlags() const override;
#endif
	/** @return	True if the HMD was initialized OK */
	bool IsInitialized() const;

	/** Update current HMD pose */
	void UpdatePoses();

	void UpdateMyPoses(GSXRHeadPoseState& HPS);  // RBF TODO - this is a temp function which (currently) returns the immediate pose

	//////////////////////////////////////////////////////
	// IXRTrackingSystem
	//////////////////////////////////////////////////////

	/**
	* Resets position, assuming current position as a 'zero-point'.
	*/
	virtual void ResetPosition() override;

	void SetCPUAndGPULevels(const int32 InCPULevel, const int32 InGPULevel) const;

	virtual bool HasHiddenAreaMesh() const override { return HiddenAreaMeshes[0].IsValid() && HiddenAreaMeshes[1].IsValid(); }
	virtual void DrawHiddenAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const override;

	virtual bool HasVisibleAreaMesh() const override { return false; }
	virtual void DrawVisibleAreaMesh_RenderThread(FRHICommandList& RHICmdList, EStereoscopicPass StereoPass) const override;

	virtual void DrawDistortionMesh_RenderThread(struct FRenderingCompositePassContext& Context, const FIntPoint& TextureSize) override;

	virtual void UpdateScreenSettings(const FViewport* InViewport) override {}


	void DrawDebug(class UCanvas* Canvas, class APlayerController*);
	void DrawDebugTrackingCameraFrustum(UWorld* World, const FRotator& ViewRotation, const FVector& ViewLocation);


	//////////////////////////////////////////////////////
	// Begin ISceneViewExtension Pure-Virtual Interface //
	//////////////////////////////////////////////////////

	virtual void SetupViewFamily(FSceneViewFamily& InViewFamily) override;
	virtual void SetupView(FSceneViewFamily& InViewFamily, FSceneView& InView) override;
	virtual void BeginRenderViewFamily(FSceneViewFamily& InViewFamily) override;
	// 	// Called on render thread at the start of rendering.
	virtual void PreRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	// Called on render thread at the start of rendering, for each view, after PreRenderViewFamily_RenderThread call.
	virtual void PreRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override {}
	//virtual bool IsActiveThisFrame(class FViewport* InViewport) const override { return false;  }  // RBF questionable
	virtual void PostRenderViewFamily_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily) override;
	virtual void PostRenderView_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneView& InView) override {}
	virtual bool IsActiveThisFrame(class FViewport* InViewport) const override;

	///////////////////////////////////////////////////
	// Begin IStereoRendering Pure-Virtual Interface //
	///////////////////////////////////////////////////

	/**
	* Whether or not stereo rendering is on this frame.
	*/
	virtual bool IsStereoEnabled() const override;

	/**
	* Switches stereo rendering on / off. Returns current state of stereo.
	* @return Returns current state of stereo (true / false).
	*/
	virtual bool EnableStereo(bool stereo = true) override;

	/**
	* Adjusts the viewport rectangle for stereo, based on which eye pass is being rendered.
	*/
	virtual void AdjustViewRect(EStereoscopicPass StereoPass, int32& X, int32& Y, uint32& SizeX, uint32& SizeY) const override;

	/**
	* Gets a projection matrix for the device, given the specified eye setup
	*/
	virtual FMatrix GetStereoProjectionMatrix(const EStereoscopicPass StereoPassType) const override;

	virtual FIntPoint GetIdealRenderTargetSize() const override;
	// virtual void SetFinalViewRect(const enum EStereoscopicPass StereoPass, const FIntRect& FinalViewRect) override;

	//////////////////////////////////////////////
	// Begin IStereoRendering Virtual Interface //
	//////////////////////////////////////////////

	/**
	* Returns eye render params, used from PostProcessHMD, RenderThread.
	*/
	virtual void GetEyeRenderParams_RenderThread(const struct FRenderingCompositePassContext& Context, FVector2D& EyeToSrcUVScaleValue, FVector2D& EyeToSrcUVOffsetValue) const override;

	// Renders texture into a backbuffer. Could be empty if no rendertarget texture is used, or if direct-rendering
	// through RHI bridge is implemented.
	virtual void RenderTexture_RenderThread(class FRHICommandListImmediate& RHICmdList, class FRHITexture2D* BackBuffer, class FRHITexture2D* SrcTexture, FVector2D WindowSize) const override;

	/**
	* Returns currently active custom present.
	*/
	//virtual FRHICustomPresent* GetCustomPresent() override { return pSnapdragonVRBridge;	}

	virtual IStereoRenderTargetManager* GetRenderTargetManager() override { return this; }

	////////////////////////////////////////////
	// Begin FXRRenderTargetManager Interface //
	////////////////////////////////////////////


	// Whether separate render target should be used or not.
	virtual bool ShouldUseSeparateRenderTarget() const override { return IsStereoEnabled(); }
	#if ENGINE_MINOR_VERSION >25
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, ETextureCreateFlags InTexFlags, ETextureCreateFlags InTargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
#else
	virtual bool AllocateRenderTargetTexture(uint32 Index, uint32 SizeX, uint32 SizeY, uint8 Format, uint32 NumMips, uint32 Flags, uint32 TargetableTextureFlags, FTexture2DRHIRef& OutTargetableTexture, FTexture2DRHIRef& OutShaderResourceTexture, uint32 NumSamples = 1) override;
#endif
	virtual bool NeedReAllocateViewportRenderTarget(const FViewport& Viewport) override;
	virtual FXRRenderBridge* GetActiveRenderBridge_GameThread(bool bUseSeparateRenderTarget) override;
	virtual void CalculateRenderTargetSize(const FViewport& Viewport, uint32& InOutSizeX, uint32& InOutSizeY) override;
	/** IStereoRenderTargetManager */
	virtual void OnBeginPlay(FWorldContext& InWorldContext) override;
	virtual void OnEndPlay(FWorldContext& InWorldContext) override;
	

	virtual IStereoLayers* GetStereoLayers() override { return this; }
	virtual uint32 CreateLayer(const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual void DestroyLayer(uint32 LayerId) override;
	virtual void SetLayerDesc(uint32 LayerId, const IStereoLayers::FLayerDesc& InLayerDesc) override;
	virtual bool GetLayerDesc(uint32 LayerId, IStereoLayers::FLayerDesc& OutLayerDesc) override;
	virtual void MarkTextureForUpdate(uint32 LayerId) override;
	virtual void UpdateSplashScreen() override;
	virtual FLayerDesc GetDebugCanvasLayerDesc(FTextureRHIRef Texture) override;
	virtual void GetAllocatedTexture(uint32 LayerId, FTextureRHIRef& Texture, FTextureRHIRef& LeftTexture) override;
	virtual bool ShouldCopyDebugLayersToSpectatorScreen() const override { return true; }

	////////////////////////////////////////////////////
	// Begin IXRTrackingSystem Pure-Virtual Interface //
	////////////////////////////////////////////////////
	
	/**
	* Returns version string.
	*/
	virtual FString GetVersionString() const override;

	/**
	* Whether or not the system supports positional tracking (either via sensor or other means).
	* The default implementation always returns false, indicating that only rotational tracking is supported.
	*/
	virtual bool DoesSupportPositionalTracking() const override;

	/**
	* If the system currently has valid tracking positions. If not supported at all, returns false.
	*/
	virtual bool HasValidTrackingPosition();

	/**
	* Reports all devices currently available to the system, optionally limiting the result to a given class of devices.
	*
	* @param OutDevices The device ids of available devices will be appended to this array.
	* @param Type Optionally limit the list of devices to a certain type.
	*/
	virtual bool EnumerateTrackedDevices(TArray<int32>& OutDevices, EXRTrackedDeviceType Type = EXRTrackedDeviceType::Any) override;

	/**
	* Get the current pose for a device.
	* This method must be callable both on the render thread and the game thread.
	* For devices that don't support positional tracking, OutPosition will be at the base position.
	*
	* @param DeviceId the device to request the pose for.
	* @param OutOrientation The current orientation of the device
	* @param OutPosition The current position of the device
	* @return true if the pose is valid or not.
	*/
	virtual bool GetCurrentPose(int32 DeviceId, FQuat& OutOrientation, FVector& OutPosition) override;

	/**
	* If the device id represents a head mounted display, fetches the relative position of the given eye relative to the eye.
	* If the device is does not represent a stereoscopic tracked camera, orientation and position should be identity and zero and the return value should be false.
	*
	* @param DeviceId the device to request the eye pose for.
	* @param Eye the eye the pose should be requested for, if passing in any other value than eSSP_LEFT_EYE or eSSP_RIGHT_EYE, the method should return a zero offset.
	* @param OutOrientation The orientation of the eye relative to the device orientation.
	* @param OutPosition The position of the eye relative to the tracked device
	* @return true if the pose is valid or not. If the device is not a stereoscopic device, return false.
	*/
	virtual bool GetRelativeEyePose(int32 DeviceId, EStereoscopicPass Eye, FQuat& OutOrientation, FVector& OutPosition) override;

	/**
	* Resets orientation by setting roll and pitch to 0, assuming that current yaw is forward direction and assuming
	* current position as a 'zero-point' (for positional tracking).
	*
	* @param Yaw				(in) the desired yaw to be set after orientation reset.
	*/
	virtual void ResetOrientationAndPosition(float Yaw = 0.f) override;


	///////////////////////////////////////////////
	// Begin IXRTrackingSystem Virtual Interface //
	///////////////////////////////////////////////

	/**
	* Resets orientation by setting roll and pitch to 0, assuming that current yaw is forward direction. Position is not changed.
	*
	* @param Yaw				(in) the desired yaw to be set after orientation reset.
	*/
	virtual void ResetOrientation(float Yaw = 0.f) override;

	/**
	* Sets base orientation by setting yaw, pitch, roll, assuming that this is forward direction.
	* Position is not changed.
	*
	* @param BaseRot			(in) the desired orientation to be treated as a base orientation.
	*/
	virtual void SetBaseRotation(const FRotator& BaseRot) override;

	/**
	* Returns current base orientation of HMD as yaw-pitch-roll combination.
	*/
	virtual FRotator GetBaseRotation() const override;

	/**
	* Sets base orientation, assuming that this is forward direction.
	* Position is not changed.
	*
	* @param BaseOrient		(in) the desired orientation to be treated as a base orientation.
	*/
	virtual void SetBaseOrientation(const FQuat& BaseOrient) override;

	/**
	* Returns current base orientation of HMD as a quaternion.
	*/
	virtual FQuat GetBaseOrientation() const override;

	/**
	* This method is called when new game frame begins (called on a game thread).
	*/
	virtual bool OnStartGameFrame(FWorldContext& WorldContext) override;

	/**
	* This method is called when game frame ends (called on a game thread).
	*/
	virtual bool OnEndGameFrame(FWorldContext& WorldContext) override;

	/**
	* Called on the game thread when view family is about to be rendered.
	*/
	virtual void OnBeginRendering_GameThread() override;

	/**
	* Called on the render thread at the start of rendering.
	*/
	virtual void OnBeginRendering_RenderThread(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) override;
	/**
	* Returns true, if head tracking is allowed. Most common case: it returns true when GEngine->IsStereoscopic3D() is true,
	* but some overrides are possible.
	*/
	// virtual bool IsHeadTrackingAllowed() const override;


	/**
	* To Access HMD rendering - related features.
	*/
	virtual class IHeadMountedDisplay* GetHMDDevice() override { return this; }
	/**
	* Access Stereo rendering device associated with this XR system.
	*/
	virtual class TSharedPtr< class IStereoRendering, ESPMode::ThreadSafe > GetStereoRenderingDevice() override { return SharedThis(this); }

	//////////////////////////////////////////////////////
	// Begin IHeadMountedDisplay Pure-Virtual Interface //
	//////////////////////////////////////////////////////

	/**
	* Called on the game thread when view family is about to be rendered.
	*/

//	virtual void BeginRendering_GameThread() override;

	/**
	* Called on the render thread at the start of rendering.
	*/

//	virtual void BeginRendering_RenderThread(const FTransform& NewRelativeTransform, FRHICommandListImmediate& RHICmdList, FSceneViewFamily& ViewFamily) override;

	/**
	* Returns whether HMDDistortion post processing should be enabled or not.
	*/
	virtual bool GetHMDDistortionEnabled(EShadingPath ShadingPath) const override { return false; }

	/**
	* Returns true if HMD is currently connected.
	*/
	virtual bool IsHMDConnected() override { return true; };

	/**
	* Whether or not switching to stereo is enabled; if it is false, then EnableStereo(true) will do nothing.
	*/
	virtual bool IsHMDEnabled() const override;

	/**
	* Enables or disables switching to stereo.
	*/
	virtual void EnableHMD(bool bEnable = true) override;

	/**
	* Get the name or id of the display to output for this HMD.
	*/
	virtual bool	GetHMDMonitorInfo(MonitorInfo&) override;

	/**
	* Calculates the FOV, based on the screen dimensions of the device. Original FOV is passed as params.
	*/
	virtual void	GetFieldOfView(float& InOutHFOVInDegrees, float& InOutVFOVInDegrees) const override;

	/**
	* Accessors to modify the interpupillary distance (meters)
	*/
	virtual void	SetInterpupillaryDistance(float NewInterpupillaryDistance) override;
	virtual float	GetInterpupillaryDistance() const override;

	/**
	* Returns 'false' if chromatic aberration correction is off.
	*/
	virtual bool IsChromaAbCorrectionEnabled() const override;

	// virtual void CalculateStereoViewOffset(const EStereoscopicPass StereoPassType, FRotator& ViewRotation, const float WorldToMeters, FVector& ViewLocation) override;

	// virtual void InitCanvasFromView(FSceneView* InView, UCanvas* Canvas) override;

	//virtual void UpdateViewport(bool bUseSeparateRenderTarget, const FViewport& Viewport, SViewport*) override;

	// virtual uint32 GetNumberOfBufferedFrames() const override;


	// void ShutdownRendering();
//	void SendMessage(const int32 score) const;  // sachin's webhelper stuff
//	void StartWebHelper();
//	void StopWebHelper();

	// Eye tracking functionality

	/**
	* If the device id represents a head mounted display, fetches the relative position of the given eye relative to the eye.
	*
	* @param DeviceId the device to request the eye pose for.
	* @param OutDirection ref to the direction vector variable to set
	*
	* @return true if the pose direction returned is valid
	*/
	bool GetRelativeEyeDirection(int32 DeviceId, FVector& OutDirection);

	/**
	* If the device id represents a head mounted display, fetches the relative position of the given eye relative to the eye.
	*
	* @param DeviceId the device to request the eye pose for.
	* @param EyePoseState ref to the pose state to set
	*
	* @return true if the pose state returned is valid
	*/
	bool GetEyePoseState(int32 DeviceId, GSXREyePoseState& EyePoseState);

public:

	/** FXRTrackingSystemBase protected interface */
	virtual float GetWorldToMetersScale() const override;

	#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	// Performance helper functions
	static void PerfLevelLog(const TCHAR* const InPrefix, enum GSXRPerfLevel InPerfLevelCpu, enum GSXRPerfLevel InPerfLevelGpu);
	static bool PerfLevelsLastSetByCvarRead( enum GSXRPerfLevel* OutPerfLevelCpuCurrent, enum GSXRPerfLevel* OutPerfLevelGpuCurrent,
											const enum GSXRPerfLevel InPerfLevelCpuDefault,const enum GSXRPerfLevel InPerfLevelGpuDefault);
	static enum GSXRPerfLevel GetCVarSnapdragonVrPerfLevelDefault();
	static void PerfLevelCpuWrite(const enum GSXRPerfLevel InPerfLevel);
	static void PerfLevelGpuWrite(const enum GSXRPerfLevel InPerfLevel);
	#endif //  SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	static enum GSXRPerfLevel PerfLevelCpuLastSet, PerfLevelGpuLastSet;

	// Eye tracking helper functions

	// pass in an GSXREyePoseState eye status member
	bool isEyeGazePointValid(int32_t& Status)      const { return  0 != (Status & GSXREyePoseStatus::kGSXRGazePointValid); }
	bool isEyeGazeDirectionValid(int32_t& Status)  const { return  0 != (Status & GSXREyePoseStatus::kGSXRGazeVectorValid); }
	bool isEyeOpenessValid(int32_t& Status)        const { return  0 != (Status & GSXREyePoseStatus::kGSXREyeOpennessValid); }
	bool isEyePupilDialationValid(int32_t& Status) const { return  0 != (Status & GSXREyePoseStatus::kGSXREyePupilDilationValid); }
	bool isEyePositionGuideValid(int32_t& Status)  const { return  0 != (Status & GSXREyePoseStatus::kGSXREyePositionGuideValid); }

	bool isEyeTrackingEnabled() const;
	void enableEyeTracking(bool b);

	// get the cached eye pose
	const GSXREyePoseState&  GetLatestEyePoseState();
	// get the cached head pose
	bool GetHeadPoseState(GSXRHeadPoseState& HeadPoseState);

	GSXRHeadPoseState GetCachedHeadPoseState() { return CachedHeadPoseState; }
private:
	bool Startup();
	void InitializeIfNecessaryOnResume();
	void CleanupIfNecessary();

	void ApplicationWillEnterBackgroundDelegate();
	void ApplicationWillDeactivateDelegate();
	void ApplicationHasEnteredForegroundDelegate();
	void ApplicationHasReactivatedDelegate();

	void PoseToOrientationAndPosition(const GSXRHeadPose& Pose, FQuat& CurrentOrientation, FVector& CurrentPosition, const float WorldToMetersScale);

	void BeginVRMode();
	void EndVRMode();

	void SendEvents();
	void SetupOcclusionMeshes();
	
	
	
	// wrapper around the raw device info - use this to get massaged values
	GSXRDeviceInfo GetDeviceInfo();

	// these are similar to what Oculus has

	//  Converts vector from SXR space to Unreal
	//  transform position SVR to UE4 (-Z[2], X[0], Y[1])
	FORCEINLINE FVector SXR2Unreal_FVector(const GSXRVector3& InVec)
	{
		return FVector(-InVec.z, InVec.x, InVec.y);
	}

	//  Converts vector from Unreal space to SXR 
	FORCEINLINE GSXRVector3 Unreal2SXR_Vector(const FVector& InVec)
	{
		return GSXRVector3{ InVec.Y, InVec.Z, -InVec.X };
	}

	// which view array index is this pass for?
	FORCEINLINE int32 ViewIndexFromStereoPass(const EStereoscopicPass StereoPassType) const
	{
		switch (StereoPassType)
			{
				case eSSP_LEFT_EYE:
				case eSSP_FULL:
					return 0;

				case eSSP_RIGHT_EYE:
					return 1;

				default:
					check(0);
					return -1;
		}
	}

private:
	bool bInitialized;
	bool bResumed;

// #if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
// 	TRefCountPtr<FSnapdragonVRHMDCustomPresent> pSnapdragonVRBridge;
// #else
// 	FRHICustomPresent* pSnapdragonVRBridge = nullptr;
// #endif

	bool InitializeExternalResources();
	
	//Temporary until sxr pose is integrated
	FQuat					CurHmdOrientation;
	FQuat					LastHmdOrientation;
	FVector                 CurHmdPosition;
	FVector					LastHmdPosition;
    FRotator				DeltaControlRotation;    
    FQuat					DeltaControlOrientation; 
	FQuat					BaseOrientation;
	double					LastSensorTime;
	GSXREyePoseState			CachedEyePoseState;
	GSXRHeadPoseState		CachedHeadPoseState;

	FIntPoint				RenderTargetSize;
	FIntRect				EyeRenderViewport[2];
	FHMDViewMesh			HiddenAreaMeshes[2];
	TRefCountPtr<FGSXRRenderBridge> RenderBridge;
	TMap<uint32,FGSXRLayerPtr> LayerMap;
	uint32 CurrentLayerId;
	bool bIsMobileMultiViewEnabled;
	bool bStartRendering;
	int32 MobileMSAAValue;
	FString RHIString;
	uint32_t mFrameIndex;
//	jclass			webHelperClass;
//	jmethodID		webHelperSendScore;
public:
	void StartRendering_RHIThread();
	void BeginFrame_RHIThread();
	void PreRenderFrame_RHIThread();
	void PostRenderFrame_RHIThread();
	void UpdateSensorData();
	void EndFrame_RHIThread();
	void StopRendering_RHIThread();
	FFrameSensorState CurrentFrame_RenderThread;
	FFrameSensorState CurrentFrame_GameThread;
	FFrameSensorState LastFrame_RenderThread;
	FFrameSensorState LastFrame_GameThread;
	bool bIsEndGameFrame;
};

//bool InRenderThread()
//{
//	if (GIsThreadedRendering && !GIsRenderingThreadSuspended.Load(EMemoryOrder::Relaxed))
//	{
//		return IsInActualRenderingThread();
//	}
//	else
//	{
//		return; //InGameThread();
//	}
//}

// #if WITH_TELEMETRY
// #define TM_MSG(...) tmMessage(__VA_ARGS__)
// #else
// #define TM_MSG(...)
// #endif

bool InGameThread()
{
	if (GIsGameThreadIdInitialized)
	{
		return FPlatformTLS::GetCurrentThreadId() == GGameThreadId;
	}
	else
	{
		return true;
	}
}

bool InRenderThread()
{
	if (GIsThreadedRendering && !GIsRenderingThreadSuspended.Load(EMemoryOrder::Relaxed))
	{
		return IsInActualRenderingThread();
	}
	else
	{
		return InGameThread();
	}
}