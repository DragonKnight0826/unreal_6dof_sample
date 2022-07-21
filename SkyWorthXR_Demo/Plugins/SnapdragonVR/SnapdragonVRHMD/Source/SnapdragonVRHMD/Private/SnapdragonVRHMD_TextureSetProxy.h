//=============================================================================
// FILE: SnapdragonVRHMD_TextureSetProxy.h
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================
#pragma once

#define USING_VULKAN_PATHWAY 1

#include "SnapdragonVRHMDPrivateRHI.h"
#include "RHI/Public/RHIResources.h"

class FSnapdragonVRTextureSet
{
public:
	FSnapdragonVRTextureSet();

	virtual ~FSnapdragonVRTextureSet();

	void SwitchToNextElement();

	FRHITexture* GetRenderTarget() const { return SwapChainImages[SwapChainImageIndex]; }

	inline int GetSwapChainImageIndex() const { return SwapChainImageIndex; }

protected:
	virtual void SetNativeResource(const FTextureRHIRef& RenderTargetTexture) = 0;

	TArray<FTextureRHIRef> SwapChainImages;
	int					   SwapChainImageIndex;
};

// FVulkanTexture2D needs the sample count, FOpenGLTexture2D does not
#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_VULKAN
class FSnapdragonVRTextureSet_Vulkan* CreateTextureSetProxy_Vulkan(
	class FVulkanDynamicRHI* InVulkanRHI,
	uint32 InSizeX,
	uint32 InSizeY,
	uint32 InArraySize,
	uint32 InNumSamples,
	uint8 InFormat,
	uint32 InFlags,
	uint32 InTargetableTextureFlags,
	FTexture2DRHIRef& OutTargetableTexture,
	FTexture2DRHIRef& OutShaderResourceTexture);
#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_VULKAN

#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_OPENGLES

#include "OpenGLDrvPrivate.h"
#include "OpenGLResources.h"

// GL_QCOM_texture_foveated
#ifndef GL_QCOM_texture_foveated
#define GL_QCOM_texture_foveated 1
typedef void (GL_APIENTRYP PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC) (GLuint texture, GLuint layer, GLuint focalPoint, GLfloat focalX, GLfloat focalY, GLfloat gainX, GLfloat gainY, GLfloat foveaArea);
extern PFNGLTEXTUREFOVEATIONPARAMETERSQCOMPROC glTextureFoveationParametersQCOM;
#define GL_FOVEATION_ENABLE_BIT_QCOM                    0x0001
#define GL_FOVEATION_SCALED_BIN_METHOD_BIT_QCOM         0x0002
#define GL_TEXTURE_PREVIOUS_SOURCE_TEXTURE_QCOM         0x8BE8
#define GL_TEXTURE_FOVEATED_FEATURE_BITS_QCOM           0x8BFB
#define GL_TEXTURE_FOVEATED_MIN_PIXEL_DENSITY_QCOM      0x8BFC
#define GL_TEXTURE_FOVEATED_FEATURE_QUERY_QCOM          0x8BFD
#define GL_TEXTURE_FOVEATED_NUM_FOCAL_POINTS_QUERY_QCOM 0x8BFE
#define GL_FRAMEBUFFER_INCOMPLETE_FOVEATION_QCOM        0x8BFF
#endif // GL_QCOM_texture_foveated


class FSnapdragonVRTextureSet_OpenGL* CreateTextureSetProxy_OpenGLES(
	class FOpenGLDynamicRHI* InGLRHI,
	GLenum InTarget,
	uint32 InSizeX,
	uint32 InSizeY,
	uint32 InArraySize,
	uint8 InFormat,
	ETextureCreateFlags InFlags,
	ETextureCreateFlags InTargetableTextureFlags,
	FTexture2DRHIRef& OutTargetableTexture,
	FTexture2DRHIRef& OutShaderResourceTexture);
#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS_OPENGLES