#pragma once
#include "CoreMinimal.h"
#include "Launch/Resources/Version.h"
#include "XRRenderBridge.h"
#include "XRSwapChain.h"


class FSkyWorthVRHMD;

class FGSXRRenderBridge : public FXRRenderBridge
{
public:
	FGSXRRenderBridge(FSkyWorthVRHMD* HMD);
	virtual ~FGSXRRenderBridge() override;
	virtual bool NeedsNativePresent() override;
	
#if ENGINE_MINOR_VERSION > 25 || ENGINE_MAJOR_VERSION == 5
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, ETextureCreateFlags Flags, ETextureCreateFlags TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) = 0;
#else
	virtual FXRSwapChainPtr CreateSwapChain(uint8 Format, uint32 SizeX, uint32 SizeY, uint32 ArraySize, uint32 NumMips, uint32 NumSamples, uint32 Flags, uint32 TargetableTextureFlags,uint32 MSAAValue,TArray<uint32> TexutreResources) = 0;
#endif
	/** FRHIRenderBridge */
	virtual bool Present(int32& InOutSyncInterval) override;

	FSkyWorthVRHMD* pSkyWorthVRHMD;
	FXRSwapChainPtr SwapChain;
};


FGSXRRenderBridge* CreateRenderBridge_OpenGL(FSkyWorthVRHMD* HMD);