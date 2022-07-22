#pragma once


#if defined(__ANDROID__) // allow other platforms to compile for faster syntax checking
#include <jni.h>
#include <android/native_window.h>
#endif

#define GSXR_MAX_RENDER_LAYERS   16

#ifndef GSXR_EXPORT
# if defined(__GNUC__) && __GNUC__ >= 4
#  define GSXR_EXPORT __attribute__ ((visibility("default")))
# else
#  define GSXR_EXPORT
# endif
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum GSXRResult {
    GSXR_ERROR_NONE = 0,
    GSXR_ERROR_NOT_INIT,
    GSXR_ERROR_UNKNOWN,
    GSXR_ERROR_OPEN_LIBRARY,
    GSXR_ERROR_JAVA,
    GSXR_ERROR_LOAD_FILE,
    GSXR_ERROR_SLAM,
} GSXRResult;
//! \brief Events
enum GSXREventType {
    kGSXREventNone = 0,
    kGSXREventSdkServiceStarting = 1,
    kGSXREventSdkServiceStarted = 2,
    kGSXREventSdkServiceStopped = 3,
    kGSXREventControllerConnecting = 4,
    kGSXREventControllerConnected = 5,
    kGSXREventControllerDisconnected = 6,
    kGSXREventThermal = 7,
    kGSXREventVrModeStarted = 8,
    kGSXREventVrModeStopping = 9,
    kGSXREventVrModeStopped = 10,
    kGSXREventSensorError = 11,
    kGSXREventMagnometerUncalibrated = 12,
    kGSXREventBoundarySystemCollision = 13,
    kGSXREvent6dofRelocation = 14,
    kGSXREvent6dofWarningFeatureCount = 15,
    kGSXREvent6dofWarningLowLight = 16,
    kGSXREvent6dofWarningBrightLight = 17,
    kGSXREvent6dofWarningCameraCalibration = 18
};

struct ANativeWindow;

//! \brief Simple structure to hold 2-component vector data
struct GSXRVector2 {
    float x, y;
};

//! \brief Simple structure to hold 3-component vector data
struct GSXRVector3 {
    float x, y, z;
};

//! \brief Simple structure to hold 4-component vector data
struct GSXRVector4 {
    float x, y, z, w;
};

//! \brief Simple structure to hold quaternion data
struct GSXRQuaternion {
    float x, y, z, w;
};

//! \brief Simple structure to hold 4x4 matrix data
struct GSXRMatrix4 {
    float M[4][4];
};

//! \brief Enum used to indicate which eye is being used
enum GSXRWhichEye {
    kGSXRLeftEye = 0,
    kGSXRRightEye,
    kGSXRNumEyes
};

//! \brief Enum used to indicate which eye to apply a render layer
enum GSXREyeMask {
    kGSXREyeMaskLeft = 0x00000001,
    kGSXREyeMaskRight = 0x00000002,
    kGSXREyeMaskBoth = 0x00000003
};

//! \brief Enum used to indicate flags passed as part of a render layer
enum GSXRLayerFlags {
    kGSXRLayerFlagNone = 0x00000000,
    kGSXRLayerFlagHeadLocked = 0x00000001,
    kGSXRLayerFlagOpaque = 0x00000002
};

enum GSXRColorSpace {
    kGSXRColorSpaceLinear = 0,
    kGSXRColorSpaceSRGB = 1,
    kGSXRNumColorSpaces
};

//! \brief Structure containing the position and orientation of the head
struct GSXRHeadPose {
    GSXRQuaternion rotation;
    GSXRVector3 position;
    float rawQuaternion[4];
    float rawPosition[3];
    uint64_t rawTS;
};

//! \brief Enum used to indicate valid components of an pose state
enum GSXRTrackingMode {
    kGSXRTrackingRotation = (1 << 0),
    kGSXRTrackingPosition = (1 << 1),
    kGSXRTrackingEye = (1 << 2)
};

//! \brief Structure containing the full set of pose data
struct GSXRHeadPoseState {
    GSXRHeadPose pose;                   //!< Head pose
    int32_t poseStatus;             //!< Bit field (GSXRTrackingMode) indicating pose status
    uint64_t poseTimeStampNs;        //!< Time stamp in which the head pose was generated (nanoseconds)
    uint64_t poseFetchTimeNs;        //!< Time stamp when this pose was retrieved
    uint64_t expectedDisplayTimeNs;  //!< Expected time when this pose should be on screen (nanoseconds)
    float deltaYaw;
    float headRotateLeftY;
    float headRotateRightY;
};

//! \brief Structure containing flags indicating data valididty of an eye pose
enum GSXREyePoseStatus {
    kGSXRGazePointValid = (1 << 0),
    kGSXRGazeVectorValid = (1 << 1),
    kGSXREyeOpennessValid = (1 << 2),
    kGSXREyePupilDilationValid = (1 << 3),
    kGSXREyePositionGuideValid = (1 << 4)
};

//! \brief Structure containing the eye tracking data
struct GSXREyePoseState {
    int32_t leftEyePoseStatus;          //!< Bit field (GSXREyePoseStatus) indicating left eye pose status
    int32_t rightEyePoseStatus;         //!< Bit field (GSXREyePoseStatus) indicating right eye pose status
    int32_t combinedEyePoseStatus;      //!< Bit field (GSXREyePoseStatus) indicating combined eye pose status

    float leftEyeGazePoint[3];        //!< Left Eye Gaze Point
    float rightEyeGazePoint[3];       //!< Right Eye Gaze Point
    float combinedEyeGazePoint[3];    //!< Combined Eye Gaze Point (HMD center-eye point)

    float leftEyeGazeVector[3];       //!< Left Eye Gaze Point
    float rightEyeGazeVector[3];      //!< Right Eye Gaze Point
    float combinedEyeGazeVector[3];   //!< Comnbined Eye Gaze Vector (HMD center-eye point)

    float leftEyeOpenness;            //!< Left eye value between 0.0 and 1.0 where 1.0 means fully open and 0.0 closed.
    float rightEyeOpenness;           //!< Right eye value between 0.0 and 1.0 where 1.0 means fully open and 0.0 closed.

    float leftEyePupilDilation;       //!< Left eye value in millimeters indicating the pupil dilation
    float rightEyePupilDilation;      //!< Right eye value in millimeters indicating the pupil dilation

    float leftEyePositionGuide[3];    //!< Position of the inner corner of the left eye in meters from the HMD center-eye coordinate system's origin.
    float rightEyePositionGuide[3];   //!< Position of the inner corner of the right eye in meters from the HMD center-eye coordinate system's origin.
};

//! \brief Enum used for indicating the CPU/GPU performance levels
//! \sa GSXRBeginVr, GSXRSetPerformanceLevels
enum GSXRPerfLevel {
    kGSXRPerfSystem = 0,            //!< System defined performance level (default)
    kGSXRPerfMinimum = 1,            //!< Minimum performance level
    kGSXRPerfMedium = 2,            //!< Medium performance level
    kGSXRPerfMaximum = 3,            //!< Maximum performance level
    kGSXRNumPerfLevels
};

//! \brief Flags used to set SVR options
enum GSXROptionFlags {
    kGSXRProtectedContent = (1 << 0),
    kGSXRMotionAwareFrames = (1 << 1)
};

//! \brief Structure containing parameters needed to enable VR mode
//! \sa GSXRBeginVr, GSXROptionFlags
struct GSXRBeginParams {
    int32_t mainThreadId;           //!< Thread Id of the primary render thread
    GSXRPerfLevel cpuPerfLevel;           //!< Desired CPU performance level
    GSXRPerfLevel gpuPerfLevel;           //!< Desired GPU performance level
    ANativeWindow *nativeWindow;           //!< Pointer to the Android native window
    int32_t optionFlags;            //!< Flags to specify SVR options (GSXROptionFlags)
    GSXRColorSpace colorSpace;             //!< Color space to utilize for the display surface
};

//! \brief Options which can be set when submitting a frame to modify the behavior of asynchronous time warp
//! \sa GSXRSubmitFrame
enum GSXRFrameOption {
    kGSXRDisableDistortionCorrection = (1
            << 0),   //!< Disables the lens distortion correction (useful for debugging)
    kGSXRDisableReprojection = (1 << 1),   //!< Disables re-projection
    kGSXREnableMotionToPhoton = (1 << 2),   //!< Enables motion to photon testing
    kGSXRDisableChromaticCorrection = (1
            << 3)   //!< Disables the lens chromatic aberration correction (performance optimization)
};

//! \brief Enum used to indicate the type of texture passed in as a render layer
//! \sa GSXRSubmitFrame
enum GSXRTextureType {
    kGSXRTypeTexture = 0,               //!< Standard texture
    kGSXRTypeTextureArray,              //!< Standard texture array (Left eye is first layer, right eye is second layer)
    kGSXRTypeImage,                     //!< EGL Image texture
    kGSXRTypeEquiRectTexture,           //!< Equirectangular texture
    kGSXRTypeEquiRectImage,             //!< Equirectangular Image texture
    kGSXRTypeCubemapTexture,            //!< Cubemap texture (Not supporting cubemap image)
    kGSXRTypeVulkan,                    //!< Vulkan texture
};

//! \brief Information about texture if type is kTypeVulkan
//! \sa GSXRSubmitFrame
struct GSXRVulkanTexInfo {
    uint32_t memSize;
    uint32_t width;
    uint32_t height;
    uint32_t numMips;
    uint32_t bytesPerPixel;
    uint32_t renderSemaphore;
};


//! \brief Enum used to indicate the type of warp/composition that should be used for a frame
enum GSXRWarpType {
    kGSXRSimple                         //!< Basic single layer (world) warp
};

//! \brief Enumeration of possible warp mesh types
//! \sa GSXRDeviceInfo
enum GSXRWarpMeshType {
    kGSXRMeshTypeColumsLtoR = 0,    // Columns Left to Right
    kGSXRMeshTypeColumsRtoL,        // Columns Right to Left
    kGSXRMeshTypeRowsTtoB,          // Rows Top to Bottom
    kGSXRMeshTypeRowsBtoT,          // Rows Bottom to Top
};

//! \brief Enumeration of possible warp meshes
//! \sa GSXRSetWarpMesh, GSXRWarpMeshType
enum GSXRWarpMeshEnum {
    kGSXRMeshEnumLeft = 0,  // Column mesh for left half of screen
    kGSXRMeshEnumRight,     // Column mesh for right half of screen
    kGSXRMeshEnumUL,        // Row mesh for Upper Left part of the screen
    kGSXRMeshEnumUR,        // Row mesh for Upper Right part of the screen
    kGSXRMeshEnumLL,        // Row mesh for Lower Left part of the screen
    kGSXRMeshEnumLR,        // Row mesh for Lower Right part of the screen
    kGSXRWarpMeshCount
};

//! \brief Render layer screen position and UV coordinates
//! \sa GSXRSubmitFrame
struct GSXRLayoutCoords {
    float LowerLeftPos[4];                        //!< 0 = X-Position; 1 = Y-Position; 2 = Z-Position; 3 = W-Component
    float LowerRightPos[4];                       //!< 0 = X-Position; 1 = Y-Position; 2 = Z-Position; 3 = W-Component
    float UpperLeftPos[4];                        //!< 0 = X-Position; 1 = Y-Position; 2 = Z-Position; 3 = W-Component
    float UpperRightPos[4];                       //!< 0 = X-Position; 1 = Y-Position; 2 = Z-Position; 3 = W-Component

    float LowerUVs[4];                            //!< [0,1] = Lower Left UV values; [2,3] = Lower Right UV values
    float UpperUVs[4];                            //!< [0,1] = Upper Left UV values; [2,3] = Upper Right UV values

    float TransformMatrix[16];                    //!< Column major uv transform matrix data. Applies to video textures (see SurfaceTexture::getTransformMatrix())
};

//! \brief Description of render layers
//! \sa GSXRSubmitFrame
struct GSXRRenderLayer {
    int32_t imageHandle;        //!< Handle to the texture/image to be rendered
    GSXRTextureType imageType;          //!< Type of texture: Standard Texture or EGL Image
    GSXRLayoutCoords imageCoords;        //!< Layout of this layer on the screen
    GSXREyeMask eyeMask;            //!< Determines which eye[s] receive this render layer
    uint32_t layerFlags;         //!< Flags applied to this render layer
    GSXRVulkanTexInfo vulkanInfo;         //!< Information about the data if it is a Vulkan texture
};


//! \brief Per-frame data needed for time warp, distortion/aberration correction
//! \sa GSXRSubmitFrame
struct GSXRFrameParams {
    int32_t frameIndex;                             //!< Frame Index
    int32_t minVsyncs;                              //!< Minimum number of vysnc events before displaying the frame (1=display refresh, 2=half refresh, etc...)

    GSXRRenderLayer renderLayers[GSXR_MAX_RENDER_LAYERS];    //!< Description of each render layer

    uint32_t frameOptions;                           //!< Options for adjusting the frame warp behavior (bitfield of GSXRFrameOption)
    GSXRHeadPoseState headPoseState;                          //!< Head pose state used to generate the frame
    GSXRWarpType warpType;                               //!< Type of warp to be used on the frame
    float fieldOfView;                            //!< Field of view used to generate this frame (larger than device fov to provide timewarp margin)
};

//! \brief Initialization parameters that are constant over the life-cycle of the application
//! \sa GSXRInitialize
struct GSXRInitParams {
#if defined(__ANDROID__)
	JavaVM* javaVm;                 //!< Java Virtual Machine pointer
	JNIEnv* javaEnv;                //!< Java Environment
	jobject javaActivityObject;     //!< Reference to the Android activity
#endif
};

//! \brief View Frustum.  These values are based on physical device properties, except the far plane is arbitrary
//! \sa GSXRSubmitFrame
struct GSXRViewFrustum {
    float left;           //!< Left Plane of Frustum
    float right;          //!< Right Plane of Frustum
    float top;            //!< Top Plane of Frustum
    float bottom;         //!< Bottom Plane of Frustum

    float near;           //!< Near Plane of Frustum
    float far;            //!< Far Plane of Frustum (Arbitrary)
};

//! \brief Basic device information to allow the client code to optimally setup their simulation and rendering pipelines
struct GSXRDeviceInfo {
    int32_t displayWidthPixels;         //!< Physical width of the display (pixels)
    int32_t displayHeightPixels;        //!< Physical height of the display (pixels)
    float displayRefreshRateHz;       //!< Refresh rate of the display
    int32_t displayOrientation;         //!< Display orientation (degrees at initialization - 0,90,180,270)
    int32_t targetEyeWidthPixels;       //!< Recommended eye buffer width (pixels)
    int32_t targetEyeHeightPixels;      //!< Recommended eye buffer height (pixels)
    float targetFovXRad;              //!< Recommended horizontal FOV
    float targetFovYRad;              //!< Recommended vertical FOV
    GSXRViewFrustum leftEyeFrustum;             //!< Recommended Frustum information for left eye
    GSXRViewFrustum rightEyeFrustum;            //!< Recommended Frustum information for right eye
    float targetEyeConvergence;       //!< Recommended eye convergence
    float targetEyePitch;             //!< Recommended eye pitch
    int32_t deviceOSVersion;            //!< Android OS Version of the device
    GSXRWarpMeshType warpMeshType;               //!< Type of mesh used to render eye buffer
};

enum GSXRThermalLevel {
    kGSXRSafe,
    kGSXRLevel1,
    kGSXRLevel2,
    kGSXRLevel3,
    kGSXRCritical,
    kGSXRNumThermalLevels
};

enum GSXRThermalZone {
    kGSXRCpu,
    kGSXRGpu,
    kGSXRSkin,
    kGSXRNumThermalZones
};

struct GSXREventData_Thermal {
    GSXRThermalZone zone;               //!< Thermal zone
    GSXRThermalLevel level;              //!< Indication of the current zone thermal level
};

typedef union {
    GSXREventData_Thermal thermal;
    uint32_t data[2];
} GSXREventData;

struct GSXREvent {
    GSXREventType eventType;              //!< Type of event
    uint32_t deviceId;               //!< An identifier for the device that generated the event (0 == HMD)
    float eventTimeStamp;         //!< Time stamp for the event in seconds since the last GSXRBeginVr call
    GSXREventData eventData;              //!< Event specific data payload
};

//! \brief Events to use in GSXRControllerSendMessage
enum GSXRControllerMessageType {
    kGSXRControllerMessageRecenter = 0,
    kGSXRControllerMessageVibration = 1
};

//! \brief Query Values
enum GSXRControllerQueryType {
    kGSXRControllerQueryBatteryRemaining = 0,
    kGSXRControllerQueryControllerCaps = 1
};

//! Controller Connection state
enum GSXRControllerConnectionState {
    kGSXRNotInitialized = 0,
    kGSXRDisconnected = 1,
    kGSXRConnected = 2,
    kGSXRConnecting = 3,
    kGSXRError = 4
};

//! Controller Touch button enumerations
namespace GSXRControllerTouch {
    enum {
        None = 0x00000000,
        One = 0x00000001,
        Two = 0x00000002,
        Three = 0x00000004,
        Four = 0x00000008,
        PrimaryThumbstick = 0x00000010,
        SecondaryThumstick = 0x00000020,
        Any = ~None
    };
}

//! Controller Trigger enumerations
namespace GSXRControllerAxis1D {
    enum {
        PrimaryIndexTrigger = 0x00000000,
        SecondaryIndexTrigger = 0x00000001,
        PrimaryHandTrigger = 0x00000002,
        SecondaryHandTrigger = 0x00000003
    };
}

//! Controller Joystick enumerations
namespace GSXRControllerAxis2D {
    enum {
        PrimaryThumbstick = 0x00000000,
        SecondaryThumbstick = 0x00000001
    };
}

//! Controller Button enumerations
namespace GSXRControllerButton {
    enum {
        None = 0x00000000,
        One = 0x00000001,
        Two = 0x00000002,
        Three = 0x00000004,
        Four = 0x00000008,
        DpadUp = 0x00000010,
        DpadDown = 0x00000020,
        DpadLeft = 0x00000040,
        DpadRight = 0x00000080,
        Start = 0x00000100,
        Back = 0x00000200,
        PrimaryShoulder = 0x00001000,
        PrimaryIndexTrigger = 0x00002000,
        PrimaryHandTrigger = 0x00004000,
        PrimaryThumbstick = 0x00008000,
        PrimaryThumbstickUp = 0x00010000,
        PrimaryThumbstickDown = 0x00020000,
        PrimaryThumbstickLeft = 0x00040000,
        PrimaryThumbstickRight = 0x00080000,
        SecondaryShoulder = 0x00100000,
        SecondaryIndexTrigger = 0x00200000,
        SecondaryHandTrigger = 0x00400000,
        SecondaryThumbstick = 0x00800000,
        SecondaryThumbstickUp = 0x01000000,
        SecondaryThumbstickDown = 0x02000000,
        SecondaryThumbstickLeft = 0x04000000,
        SecondaryThumbstickRight = 0x08000000,
        Up = 0x10000000,
        Down = 0x20000000,
        Left = 0x40000000,
        Right = 0x80000000,
        Any = ~None
    };
}

// Current state of the controller
struct GSXRControllerState {
    //! Orientation
    GSXRQuaternion rotation;

    //! Position
    GSXRVector3 position;

    //! Gyro
    GSXRVector3 gyroscope;

    //! Accelerometer
    GSXRVector3 accelerometer;

    //! timestamp
    uint64_t timestamp;

    //! All digital button states as bitflags
    uint32_t buttonState;   //!< all digital as bit flags

    //! Touchpads, Joysticks
    GSXRVector2 analog2D[4];       //!< analog 2D's

    //! Triggers
    float analog1D[8];       //!< analog 1D's

    //! Whether the touchpad area is being touched.
    uint32_t isTouching;

    //! Controller Connection state
    GSXRControllerConnectionState connectionState;
};

// Caps of the Controller.
struct GSXRControllerCaps {
    //! Device Manufacturer
    char deviceManufacturer[64];

    //! Device Identifier from the module
    char deviceIdentifier[64];

    //! Controller Capabilities
    uint32_t caps; //0 bit = Provides Rotation; 1 bit = Position;

    //! Enabled Buttons Bitfield
    uint32_t activeButtons;

    //! Active 2D Analogs Bitfield
    uint32_t active2DAnalogs;

    //! Active 1D Analogs Bitfield
    uint32_t active1DAnalogs;

    //! Active Touch Buttons
    uint32_t activeTouchButtons;
};

struct GSXRSingleLayerData {
    uint32_t layerId;
    uint32_t parentLayerId;
    uint32_t layerTextureId;
    float modelMatrixData[16];
    uint32_t editFlag;
    int z;
    float vertexPosition[12];
    float vertexUV[8];
    float alpha;
    bool bOpaque;
    uint32_t taskId;
};

struct GSXRAllLayers {
    uint32_t layerNum;
    float viewMatrixData[16];
    GSXRSingleLayerData *layerData = nullptr;
};

typedef void (*GSXR_HANDSHANK_TOUCH_PANEL_EVENT_CALLBACK)(float x, float y, int touch, const int lr);
typedef void (*GSXR_HANDSHANK_KEY_EVENT_CALLBACK)(int keycode, int action, const int lr);
typedef void (*GSXR_HANDSHANK_KEY_TOUCH_EVENT_CALLBACK)(int keycode, bool touch,
                                                        const int lr);
typedef void (*GSXR_HANDSHANK_TOUCH_EVENT_CALLBACK)(int touch_x, int touch_y, const int lr);
typedef void (*GSXR_HANDSHANK_HALL_EVENT_CALLBACK)(int hall_x, int hall_y, const int lr);
typedef void (*GSXR_HANDSHANK_CHARGING_EVENT_CALLBACK)(bool isCharging, const int lr);
typedef void (*GSXR_HANDSHANK_BATTERY_EVENT_CALLBACK)(int battery, const int lr);
typedef void (*GSXR_HANDSHANK_CONNECT_EVENT_CALLBACK)(bool isConnected, const int lr);
typedef void (*GSXR_APP_EXIT_CALLBACK)();
typedef void (*GSXR_GESTURE_CHANGE_CALLBACK)(const int gesture);
typedef void (*GSXR_GESTURE_MODEL_DATA_CHANGE_CALLBACK)();
typedef void (*GSXR_LOW_POWER_WARNING_CALLBACK)(const int power);

typedef GSXRResult (*PFN_GSXR_initializeImpl)(const GSXRInitParams *pInitParams);
typedef GSXRResult (*PFN_GSXR_shutdownImpl)();
typedef GSXRDeviceInfo (*PFN_GSXR_getDeviceInfoImpl)();
typedef GSXRResult (*PFN_GSXR_setPerformanceLevelsImpl)(GSXRPerfLevel cpuPerfLevel,
                                                       GSXRPerfLevel gpuPerfLevel);
typedef GSXRResult (*PFN_GSXR_beginVrImpl)(const GSXRBeginParams *pBeginParams);
typedef GSXRResult (*PFN_GSXR_endVrImpl)();
typedef float (*PFN_GSXR_getPredictedDisplayTimeImpl)();
typedef float (*PFN_GSXR_getPredictedDisplayTimePipelinedImpl)(unsigned int depth);
typedef GSXRHeadPoseState (*PFN_GSXR_getPredictedHeadPoseImpl)(float predictedTimeMs);
typedef GSXRHeadPoseState (*PFN_GSXR_getPredictedHeadPoseExtImpl)(float predictedTimeMs,
                                                                 bool virHandGesture);
typedef GSXRHeadPoseState (*PFN_GSXR_getHistoricHeadPoseImpl)(int64_t timestampNs,
                                                             bool bLoadCalibration);

typedef bool (*PFN_GSXR_isSupportEyeTrackingImpl)();
typedef GSXRResult (*PFN_GSXR_startEyeTrackingImpl)();
typedef GSXRResult (*PFN_GSXR_stopEyeTrackingImpl)();
typedef GSXRResult (*PFN_GSXR_getEyePoseImpl)(GSXREyePoseState *pReturnPose);
typedef GSXRResult (*PFN_GSXR_getEyeFocusImpl)(float &outFocusX, float &outFocusY);

typedef GSXRResult (*PFN_GSXR_recenterPoseImpl)();
//! \brief Recenters the head position at the current position
typedef GSXRResult (*PFN_GSXR_recenterPositionImpl)();
typedef GSXRResult (*PFN_GSXR_recenterOrientationImpl)(bool yawOnly);
typedef uint32_t (*PFN_GSXR_getSupportedTrackingModesImpl)();
typedef GSXRResult (*PFN_GSXR_setTrackingModeImpl)(uint32_t trackingModes);
typedef uint32_t (*PFN_GSXR_getTrackingModeImpl)();
typedef bool (*PFN_GSXR_pollEventImpl)(GSXREvent *pEvent);
typedef GSXRResult (*PFN_GSXR_beginEyeImpl)(GSXRWhichEye whichEye, GSXRTextureType imageType);
typedef GSXRResult (*PFN_GSXR_endEyeImpl)(GSXRWhichEye whichEye, GSXRTextureType imageType);
typedef GSXRResult (*PFN_GSXR_submitFrameImpl)(const GSXRFrameParams *pFrameParams);
typedef int (*PFN_GSXR_controllerStartTrackingImpl)(const char *controllerDesc);
typedef void (*PFN_GSXR_controllerStopTrackingImpl)(int controllerHandle);
typedef GSXRControllerState (*PFN_GSXR_controllerGetStateImpl)(int controllerHandle,
                                                              int controllerSpace);
typedef void (*PFN_GSXR_controllerSendMessageImpl)(int controllerHandle, int what, int arg1,
                                                  int arg2);
typedef int (*PFN_GSXR_controllerQueryImpl)(int controllerHandle, int what, void *memory,
                                           unsigned int memorySize);
typedef GSXRResult (*PFN_GSXR_setWarpMeshImpl)(GSXRWarpMeshEnum whichMesh, void *pVertexData,
                                              int vertexSize,
                                              int nVertices, unsigned int *pIndices, int nIndices);
typedef GSXRResult (*PFN_GSXR_getOcclusionMeshImpl)(GSXRWhichEye whichEye, int *pTriangleCount,
                                                   int *pVertexStride, float *pTriangles);
typedef bool (*PFN_GSXR_isBoundarySystemEnabledImpl)();
typedef void (*PFN_GSXR_enableBoundarySystemImpl)(bool enableBoundar);
typedef void (*PFN_GSXR_forceDisplayBoundarySystemImpl)(bool forceDisplayBoundary);
typedef GSXRResult (*PFN_GSXR_setBoundaryParametersImpl)(float *pMinValues, float *pMaxValues,
                                                        float visibilityRadius);
typedef GSXRResult (*PFN_GSXR_getBoundaryParametersImpl)(float *pMinValues, float *pMaxValues,
                                                        float *pVisibilityRadius);
typedef int (*PFN_GSXR_getPointCloudDataImpl)(int &outNum, uint64_t &outTimestamp,
                                             float *outDataArray);
typedef float (*PFN_GSXR_fetchDeflectionImpl)();
typedef void (*PFN_GSXR_updateRelativeDeflectionImpl)(int deflection);
typedef int (*PFN_GSXR_getOfflineMapRelocStateImpl)();
typedef void (*PFN_GSXR_resaveMapImpl)(const char *path);
typedef void (*PFN_GSXR_getGnssImpl)(double &dt, float *gnss);
typedef void (*PFN_GSXR_set6DofImpl)(bool is6dof);
typedef void (*PFN_GSXR_setMeshOffsetImpl)(int type, float value);
typedef float (*PFN_GSXR_getMeshOffsetImpl)(int type);
typedef int (*PFN_GSXR_getPanelImpl)();
typedef void (*PFN_GSXR_getPanelInfoImpl)(float *info);
typedef void (*PFN_GSXR_controllerSetKeyEventCallbackImpl)(
        GSXR_HANDSHANK_KEY_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetTouchPanelEventCallbackImpl)(
        GSXR_HANDSHANK_TOUCH_PANEL_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetKeyTouchEventCallbackImpl)(
        GSXR_HANDSHANK_KEY_TOUCH_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetTouchEventCallbackImpl)(
        GSXR_HANDSHANK_TOUCH_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetHallEventCallbackImpl)(
        GSXR_HANDSHANK_HALL_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetChargingEventCallbackImpl)(
        GSXR_HANDSHANK_CHARGING_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetBatteryEventCallbackImpl)(
        GSXR_HANDSHANK_BATTERY_EVENT_CALLBACK callback);
typedef void (*PFN_GSXR_controllerSetConnectEventCallbackImpl)(
        GSXR_HANDSHANK_CONNECT_EVENT_CALLBACK callback);
typedef int (*PFN_GSXR_controllerGetBatteryImpl)(int lr);
typedef int (*PFN_GSXR_controllerGetVersionImpl)();
typedef int (*PFN_GSXR_controllerGetControllerModeImpl)();
typedef int (*PFN_GSXR_controllerGetbondImpl)();
typedef bool (*PFN_GSXR_controllerGetConnectStateImpl)(int lr);
typedef void (*PFN_GSXR_controllerLedControlImpl)(int enable);
typedef void (*PFN_GSXR_controllerVibrateControlImpl)(int value);
typedef int (*PFN_GSXR_fetchControllerPostureImpl)(float *orientationArray, int lr);
typedef void (*PFN_GSXR_setAppExitCallbackImpl)(GSXR_APP_EXIT_CALLBACK callback);
typedef int (*PFN_GSXR_startGestureImpl)(GSXR_LOW_POWER_WARNING_CALLBACK callback);
typedef int (*PFN_GSXR_stopGestureImpl)();
typedef int (*PFN_GSXR_getGestureImpl)(uint64_t *index, float *model, float *pose);
typedef void (*PFN_GSXR_setGestureDataImpl)(float *model, float *pose);
typedef void (*PFN_GSXR_setGestureCallbackImpl)(GSXR_GESTURE_CHANGE_CALLBACK callback);
typedef void (*PFN_GSXR_setGestureModelDataCallbackImpl)(
        GSXR_GESTURE_MODEL_DATA_CHANGE_CALLBACK callback);
typedef void (*PFN_GSXR_setLowPowerWarningCallbackImpl)(
        GSXR_LOW_POWER_WARNING_CALLBACK callback);
typedef GSXRResult (*PFN_GSXR_getLatestCameraDataForUnityImpl)(bool &outBUpdate,
                                                              uint32_t &outFrameIndex,
                                                              uint64_t &outFrameExposureNano,
                                                              uint8_t *outFrameData,
                                                              float *outTRDataArray);
typedef GSXRResult (*PFN_GSXR_getLatestCameraDataForUnityNoTransformImpl)(bool &outBUpdate,
                                                                         uint32_t &outFrameIndex,
                                                                         uint64_t &outFrameExposureNano,
                                                                         uint8_t *outFrameData,
                                                                         float *outTRDataArray);
typedef GSXRResult (*PFN_GSXR_getTransformMatrixImpl)(bool &outBLoaded, float *outTransformArray);
typedef GSXRHeadPoseState (*PFN_GSXR_getLatestEyeMatricesImpl)(float *outLeftEyeMatrix,
                                                              float *outRightEyeMatrix,
                                                              float *outT, float *outR,
                                                              float predictedTimeMs);
typedef GSXRResult (*PFN_GSXR_getLatestCameraBinocularDataImpl)(bool &outBUpdate,
                                                               uint32_t &outFrameIndex,
                                                               uint64_t &outFrameExposureNano,
                                                               uint8_t *outLeftFrameData,
                                                               uint8_t *outRightFrameData);
typedef int (*PFN_GSXR_initLayerImpl)();
typedef int (*PFN_GSXR_startLayerRenderingImpl)();
typedef int (*PFN_GSXR_getAllLayersDataImpl)(GSXRAllLayers *outAllLayers);
typedef int (*PFN_GSXR_endLayerRenderingImpl)(GSXRAllLayers *allLayers);
typedef int (*PFN_GSXR_updateModelMatrixImpl)(uint32_t layerId, float *modelMatrixArray);
typedef int (*PFN_GSXR_sendActionBarCMDImpl)(uint32_t layerId, int cmd);
typedef int (*PFN_GSXR_setForcedDisplaySizeImpl)(int width, int height);
typedef int (*PFN_GSXR_injectMotionEventImpl)(uint32_t layerId, int displayID, int action, float x,
                                             float y);
typedef int (*PFN_GSXR_destroyLayerImpl)();
typedef bool (*PFN_GSXR_isSupportControllerImpl)();
typedef int (*PFN_GSXR_getControllerNumImpl)();
typedef bool (*PFN_GSXR_isSupportOpticsCalibrationImpl)();
typedef int (*PFN_GSXR_getOpticsCalibrationMatrixImpl)(bool &outBLoaded, float *outTransformArray);
typedef bool (*PFN_GSXR_isSupportGestureImpl)();
typedef void (*PFN_GSXR_getDeviceNameImpl)(char *outDeviceName);
typedef int (*PFN_GSXR_getXRTypeImpl)();
typedef void (*PFN_GSXR_saveMapImpl)();
typedef bool (*PFN_GSXR_isSupportPanelImpl)();
typedef bool (*PFN_GSXR_isSupportMapImpl)();
typedef GSXRResult (*PFN_GSXR_getFishEyeInfoImpl)(int &outNum, int &outWidth, int &outHeight);
typedef void (*PFN_GSXR_getVersionImpl)(char *outVersionName);

#ifdef __cplusplus
}
#endif