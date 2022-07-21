//=============================================================================
// FILE: SnapdragonVRHMD_CustomPresent.h
//
//                  Copyright (c) 2018 QUALCOMM Technologies Inc.
//                              All Rights Reserved.
//
//=============================================================================
#pragma once

#define USING_OPENGL_PATHWAY 1

#include "ISnapdragonVRHMDModule.h"

DECLARE_LOG_CATEGORY_EXTERN(LogSVR, Log, All);
// DECLARE_LOG_CATEGORY_EXTERN(LogSVR, Log, Fatal);

#include "GSXRApi.h"
#include "Core/Public/Containers/CircularBuffer.h"


template<typename ElementType> class FCircularQueue
{
public:

	/**
	* Default constructor.
	*
	* @param Size The number of elements that the queue can hold (will be rounded up to the next power of 2).
	*/
	FCircularQueue(uint32 Size)
		: Buffer(Size)
		, Head(0)
		, Tail(0)
	{ }

	/** Virtual destructor. */
	virtual ~FCircularQueue() { }

public:

	/**
	* Gets the number of elements in the queue.
	*
	* @return Number of queued elements.
	*/
	uint32 Count() const
	{
		int32 Count = Tail - Head;

		if (Count < 0)
		{
			Count += Buffer.Capacity();
		}

		return Count;
	}

	/**
	* Removes an item from the front of the queue.
	*
	* @param OutElement Will contain the element if the queue is not empty.
	* @return true if an element has been returned, false if the queue was empty.
	*/
	bool Dequeue(ElementType& OutElement)
	{
		if (Head != Tail)
		{
			OutElement = Buffer[Head];
			Head = Buffer.GetNextIndex(Head);

			return true;
		}

		return false;
	}

	/**
	* Empties the queue.
	*
	* @see IsEmpty
	*/
	void Empty()
	{
		Head = Tail;
	}

	/**
	* Adds an item to the end of the queue.
	*
	* @param Element The element to add.
	* @return true if the item was added, false if the queue was full.
	*/
	bool Enqueue(const ElementType& Element)
	{
		uint32 NewTail = Buffer.GetNextIndex(Tail);

		if (NewTail != Head)
		{
			Buffer[Tail] = Element;
			Tail = NewTail;

			return true;
		}

		return false;
	}

	/**
	* Checks whether the queue is empty.
	*
	* @return true if the queue is empty, false otherwise.
	* @see Empty, IsFull
	*/
	FORCEINLINE bool IsEmpty() const
	{
		return (Head == Tail);
	}

	/**
	* Checks whether the queue is full.
	*
	* @return true if the queue is full, false otherwise.
	* @see IsEmpty
	*/
	bool IsFull() const
	{
		return (Buffer.GetNextIndex(Tail) == Head);
	}

	/**
	* Returns the oldest item in the queue without removing it.
	*
	* @param OutItem Will contain the item if the queue is not empty.
	* @return true if an item has been returned, false if the queue was empty.
	*/
	bool Peek(ElementType& OutItem)
	{
		if (Head != Tail)
		{
			OutItem = Buffer[Head];

			return true;
		}

		return false;
	}

	ElementType* Get()
	{
		if (Head != Tail)
		{
			return &Buffer[Head];
		}

		return nullptr;
	}

	ElementType* CurrentPtr()
	{
		if (Head != Tail)
		{
			return &Buffer[Tail];
		}

		return nullptr;
	}

	bool Current(ElementType& OutItem)
	{
		if (Head != Tail)
		{
			OutItem = Buffer[Tail];

			return true;
		}

		return false;
	}

private:

	/** Holds the buffer. */
	FCircularQueue<ElementType> Buffer;

	/** Holds the index to the first item in the buffer. */
	MS_ALIGN(PLATFORM_CACHE_LINE_SIZE) volatile uint32 Head GCC_ALIGN(PLATFORM_CACHE_LINE_SIZE);

	/** Holds the index to the last item in the buffer. */
	MS_ALIGN(PLATFORM_CACHE_LINE_SIZE) volatile uint32 Tail GCC_ALIGN(PLATFORM_CACHE_LINE_SIZE);
};

//--------------------------------------------------------------------------------------------------------


#if SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS
class FSnapdragonVRHMDCustomPresent : public FRHICustomPresent
{
	friend class FViewExtension;
	friend class FSnapdragonVRHMD;

	enum
	{
		kMaxPoseHistoryLength = 8
	};

public:
	static FSnapdragonVRHMDCustomPresent* Create(class FSnapdragonVRHMD* pHMD);

	// FRHICustomPresent
	virtual void OnBackBufferResize() override;

	// Called from render thread to see if a native present will be requested for this frame.
	// @return	true if native Present will be requested for this frame; false otherwise.  Must
	// match value subsequently returned by Present for this frame.
	virtual bool NeedsNativePresent() override;

	// Called from RHI thread to perform custom present.
	// @param InOutSyncInterval - in out param, indicates if vsync is on (>0) or off (==0).
	// @return	true if native Present should be also be performed; false otherwise. If it returns
	// true, then InOutSyncInterval could be modified to switch between VSync/NoVSync for the normal 
	// Present.  Must match value previously returned by NeedsNormalPresent for this frame.
	virtual bool Present(int32& InOutSyncInterval) override;

	FSnapdragonVRHMDCustomPresent(class FSnapdragonVRHMD* pHMD);
	~FSnapdragonVRHMDCustomPresent();

	void BeginRendering(FRHICommandListImmediate& RHICmdList, FSceneViewFamily& InViewFamily);
	void FinishRendering();
	void UpdateViewport(const FViewport& Viewport, FRHIViewport* InViewportRHI);
	void DoBeginVR();
	void DoEndVR();

	struct FPoseStateFrame
	{
		GSXRHeadPoseState Pose;
		uint64 FrameNumber;
	};

	void CreateLayout(float centerX, float centerY, float radiusX, float radiusY, GSXRLayoutCoords *pLayout);

	virtual void SubmitFrame(const FPoseStateFrame& PoseState) = 0;

	virtual bool AllocateRenderTargetTexture(
		uint32 SizeX,
		uint32 SizeY,
		uint8 Format,
		uint32 NumMips,
		ETextureCreateFlags Flags,
		ETextureCreateFlags TargetableTextureFlags,
		FTexture2DRHIRef& OutTargetableTexture,
		FTexture2DRHIRef& OutShaderResourceTexture,
		uint32 NumSamples) = 0;

	// TODO: Refactor
	inline bool UsesArrayTexture() const { return bUsesArrayTexture; }

	static FCriticalSection InVRModeCriticalSection;
	static FCriticalSection	PerfLevelCriticalSection;

protected:
	bool bInVRMode;
	bool bContextInitialized;

	// TODO: Refactor
	bool bUsesArrayTexture;

	// This is the pose used to render the current frame (pass to SVR for warp)
	GSXRHeadPoseState	mRenderPose;
};

FSnapdragonVRHMDCustomPresent* CreateCustomPresent_OpenGLES(class FSnapdragonVRHMD* pHMD);
FSnapdragonVRHMDCustomPresent* CreateCustomPresent_Vulkan(class FSnapdragonVRHMD* pHMD);

#endif // SNAPDRAGONVR_HMD_SUPPORTED_PLATFORMS