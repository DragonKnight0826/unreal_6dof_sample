#include "FGSXRRenderBridge.h"
#include "SnapdragonVRHMD.h"
#include "FGSXRSwapChain.h"

FGSXRRenderBridge::FGSXRRenderBridge(FSnapdragonVRHMD* HMD)
	:FXRRenderBridge()
	,pSnapdragonVRHMD(HMD)
{
}

FGSXRRenderBridge::~FGSXRRenderBridge()
{
}

bool FGSXRRenderBridge::NeedsNativePresent()
{
	return false;
}

bool FGSXRRenderBridge::Present(int32& InOutSyncInterval)
{
	bool bNeedsNativePresent = true;

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
	if (pSnapdragonVRHMD)
	{
		pSnapdragonVRHMD->EndFrame_RHIThread();

#if ENGINE_MINOR_VERSION < 27 && ENGINE_MAJOR_VERSION < 5
		bNeedsNativePresent = !FPlatformMisc::IsStandaloneStereoOnlyDevice();
#else
		bNeedsNativePresent = !pSnapdragonVRHMD->IsStandaloneStereoOnlyDevice();
#endif
		
	}
#endif

	InOutSyncInterval = 0; // VSync off

	return bNeedsNativePresent;
}

class FGSXRRenderBridge_OpenGL : public FGSXRRenderBridge
{
public:
	FGSXRRenderBridge_OpenGL(FSnapdragonVRHMD* HMD)
		:FGSXRRenderBridge(HMD)
	{
		
	}
#if ENGINE_MINOR_VERSION >25 || ENGINE_MAJOR_VERSION == 5
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#else	
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) override
#endif
	{
		SwapChain = CreateSwapChain_OpenGL(Format, SizeX, SizeY, ArraySize, NumMips, NumSamples, Flags, TargetableTextureFlags,MSAAValue, TexutreResources);
		return  SwapChain;
	}
};

FGSXRRenderBridge* CreateRenderBridge_OpenGL(FSnapdragonVRHMD* HMD)
{
	return new FGSXRRenderBridge_OpenGL(HMD);
}
