#pragma once

#include <GSXRApi.h>


namespace SC {


#ifdef __cplusplus
    extern "C" {
#endif
    /**
     * Initializes GSXR components
     * @param pInitParams activity info
     * @return GSXR_ERROR_NONE if no error.
     */
    GSXR_EXPORT GSXRResult GSXR_nativeInitSlam(const GSXRInitParams *pInitParams);
    /**
     * Releases GSXR components
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeShutdown();
    /**
     * Queries for device specific information
     * @return GSXRDeviceInfo structure containing device specific information (resolution, fov, etc..)
     */
    GSXR_EXPORT GSXRDeviceInfo GSXR_nativeGetDeviceInfo();
    /**
     * Requests specific brackets of CPU/GPU performance
     * @param cpuPerfLevel Requested performance level for CPU
     * @param gpuPerfLevel Requested performance level for GPU
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeSetPerformanceLevels(GSXRPerfLevel cpuPerfLevel,
                                                           GSXRPerfLevel gpuPerfLevel);
    /**
     * Enables GSXR services
     * @param pBeginParams GSXRBeginParams structure
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeStartSlam(const GSXRBeginParams *pBeginParams);
    /**
     * Disables VR services
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeStopSlam();
    /**
     * Calculates a predicted time when the current frame would be displayed. Assumes a pipeline depth of 1
     * @return Predicted display time for the current frame in milliseconds
     */
    GSXR_EXPORT float GSXR_nativeGetPredictedDisplayTime();
    /**
     * Calculates a predicted time when the current frame would be displayed.
     * Allows for specifying the current depth of the pipeline
     * @param depth
     * @return Predicted display time for the current frame in milliseconds
     */
    GSXR_EXPORT float GSXR_nativeGetPredictedDisplayTimePipelined(unsigned int depth);
    /**
     * Calculates a predicted head pose
     * @param predictedTimeMs Time ahead of the current time in ms to predict a head pose for
     * @return  The predicted head pose and relevant pose state information
     */
    GSXR_EXPORT GSXRHeadPoseState GSXR_nativeGetPredictedPose(float predictedTimeMs);
    /**
     * Calculates a predicted head pose
     * @param predictedTimeMs  Time ahead of the current time in ms to predict a head pose for
     * @param virHandGesture whether to enable correction and headoffset
     * @return The predicted head pose and relevant pose state information
     */
    GSXR_EXPORT GSXRHeadPoseState GSXR_nativeGetPredictedHeadPoseExt(float predictedTimeMs,
                                                                     bool virHandGesture);
    /**
     * Retrieves a historic head pose
     * @param timestampNs  Time in ns to retrieve a head pose for
     * @param bLoadCalibration whether to load calibration file
     * @return The head pose and relevant pose state information
     */
    GSXR_EXPORT GSXRHeadPoseState GSXR_nativeGetHistoricHeadPose(int64_t timestampNs,
                                                                 bool bLoadCalibration);
    /**
     * Retrieves a current eye pose
     * @param pReturnPose output eye pose
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeGetEyePose(GSXREyePoseState *pReturnPose);

    GSXR_EXPORT bool GSXR_nativeSupportEyeTracking();

    GSXR_EXPORT GSXRResult GSXR_nativeStartEyeTracking();

    GSXR_EXPORT GSXRResult GSXR_nativeStopEyeTracking();

    GSXR_EXPORT GSXRResult GSXR_nativeGetEyeFocus(float &outFocusX, float &outFocusY);

    /**
     * Recenters the head position and orientation at the current values
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeRecenterPose();
    /**
     * Recenters the head position at the current position
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeRecenterPosition();
    /**
     * Recenters the head orientation (Yaw only) at the current value
     * @param yawOnly
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeRecenterOrientation(bool yawOnly);
    /**
     * Returns the supported tracking types
     * @return Bitfield of svrTrackingType values indicating the supported tracking modes
     */
    GSXR_EXPORT uint32_t GSXR_nativeGetSupportSlamMode();
    /**
     * Sets the current head tracking mode
     * @param trackingModes Bitfield of svrTrackingType values indicating the tracking modes to enable
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeSetSlamType(uint32_t trackingModes);
    /**
     * Returns the current head tracking mode
     * @return Bitfield of svrTrackingType values indicating the tracking modes enabled
     */
    GSXR_EXPORT uint32_t GSXR_nativeGetSlamType();
    /**
     * Polls for an available event.  If event(s) are present pEvent will be filled with the event details
     * @param pEvent
     * @return true if an event was present, false if not
     */
    GSXR_EXPORT bool GSXR_nativePollEvent(GSXREvent *pEvent);
    /**
     * Called after eye buffer is bound but before game rendering starts
     * @param whichEye Which eye is being rendered
     * @param imageType Type of eye render buffer
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeBeginEye(GSXRWhichEye whichEye, GSXRTextureType imageType);
    /**
     * Called after eye buffer is rendered but before frame is submitted
     * @param whichEye Which eye is being rendered
     * @param imageType Type of eye render buffer
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeEndEye(GSXRWhichEye whichEye, GSXRTextureType imageType);
    /**
     * Submits a frame to asynchronous time warp
     * @param pFrameParams svrFrameParams structure
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeSubmitDataFrame(const GSXRFrameParams *pFrameParams);
    /**
     * Starts Tracking
     * @param controllerDesc Controller Description
     * @return handle for the controller
     */
    GSXR_EXPORT int GSXR_nativeControllerStartTracking(const char *controllerDesc);
    /**
     * Stops tracking the controller
     * @param controllerHandle handle for the controller
     */
    GSXR_EXPORT void GSXR_nativeControllerStopTracking(int controllerHandle);
    /**
     * Get the current state of the controller
     * @param controllerHandle handle for the controller
     * @param controllerSpace
     * @return
     */
    GSXR_EXPORT GSXRControllerState GSXR_nativeControllerGetState(int controllerHandle,
                                                                  int controllerSpace);
    /**
     * Send a message to the controller
     * @param controllerHandle handle for the controller
     * @param what type of event
     * @param arg1 argument for the event
     * @param arg2 argument for the event
     */
    GSXR_EXPORT void GSXR_nativeControllerSendMessage(int controllerHandle, int what, int arg1, int arg2);
    /**
     * make a query to the controller
     * @param controllerHandle handle for the controller
     * @param what info
     * @param memory memory to fill in
     * @param memorySize size of the memory to fill in
     * @return number of bytes written into the memory
     */
    GSXR_EXPORT int GSXR_nativeControllerQuery(int controllerHandle, int what, void *memory,
                                               unsigned int memorySize);
    /**
     * Replaces the current Time Warp mesh the supplied mesh
     * @param whichMesh Which mesh is to be replaced
     * @param pVertexData 15 float values per vertex: Position = 3; R|G|B UV values 4 each (Fourth value is Vignette multiplier [0,1]). NULL reverts the override.
     * @param vertexSize Size of pVertexData
     * @param nVertices Number of vertices contained in pVertexData
     * @param pIndices Pointer to indice data. Expected to be used with GL_TRIANGLES
     * @param nIndices Number of indices contained in pIndices. Expected to be used with GL_TRIANGLES
     * @return GSXR_ERROR_NONE for success; GSXR_ERROR_UNSUPPORTED if vertexSize is incorrect; GSXR_ERROR_UNKNOWN for any other error
     */
    GSXR_EXPORT GSXRResult GSXR_nativeSetWarpMesh(GSXRWarpMeshEnum whichMesh, void *pVertexData,
                                                  int vertexSize,
                                                  int nVertices, unsigned int *pIndices, int nIndices);
    /**
     * Gets triangle data for the occlusion area mesh
     * @param whichEye which eye to get the occlusion mesh for
     * @param nTriangleCount the number of triangles needed for the mesh, set to required size if pTriangles == NULL
     * @param pVertexStride
     * @param pTriangles buffer to store occlusion mesh data
     * @return GSXR_ERROR_NONE for success; GSXR_ERROR_UNKNOWN for any other error
     */
    GSXR_EXPORT GSXRResult GSXR_nativeGetOcclusionMesh(GSXRWhichEye whichEye, int *pTriangleCount,
                                                       int *pVertexStride, float *pTriangles);
    /**
     * Check whether the Boundary system is enabled
     * @return True/False for Enabled/Disabled
     */
    GSXR_EXPORT bool GSXR_nativeIsBoundarySystemEnabled();
    /**
     * Enable/Disable the Boundary system
     * @param enableBoundary Whether to enable or disable the Boundary system
     */
    GSXR_EXPORT void GSXR_nativeEnableBoundarySystem(bool enableBoundary);
    /**
     * Enable/Disable the forced display of Boundary system
     * @param forceDisplayBoundary Whether to enable or disable the forced display of Boundary system
     */
    GSXR_EXPORT void GSXR_nativeForceDisplayBoundarySystem(bool forceDisplayBoundary);
    /**
     * Set parameters for the Boundary system
     * @param pMinValues
     * @param pMaxValues
     * @param visibilityRadius
     * @return
     */
    GSXR_EXPORT GSXRResult GSXR_nativeSetBoundaryParameters(float *pMinValues, float *pMaxValues,
                                                            float visibilityRadius);
    /**
     * Get parameters for the Boundary system
     * @param pMinValues Real world minimum physical boundaries (Meters. 0 = X-Position; 1 = Y-Position; 2 = Z-Position)
     * @param pMaxValues Real world maximum physical boundaries (Meters. 0 = X-Position; 1 = Y-Position; 2 = Z-Position)
     * @param pVisibilityRadius Distance from physical boundaries when visual notification starts to appear
     * @return GSXR_ERROR_NONE for success; GSXR_ERROR_UNKNOWN for any other error
     */
    GSXR_EXPORT GSXRResult GSXR_nativeGetBoundaryParameters(float *pMinValues, float *pMaxValues,
                                                            float *pVisibilityRadius);
    GSXR_EXPORT int
    GSXR_nativeGetPointCloudData(int &outNum, uint64_t &outTimestamp, float *outDataArray);
    GSXR_EXPORT float GSXR_nativeFetchDeflection();
    GSXR_EXPORT void GSXR_nativeUpdateRelativeDeflection(int deflection);
    GSXR_EXPORT int GSXR_nativeGetOfflineMapRelocState();
    GSXR_EXPORT void GSXR_nativeResaveMap(const char *path);
    GSXR_EXPORT void GSXR_nativeGetGnss(double &dt, float *gnss);
    GSXR_EXPORT void GSXR_nativeSet6Dof(bool is6dof);
    GSXR_EXPORT void GSXR_nativeSetMeshOffset(int type, float value);
    GSXR_EXPORT float GSXR_nativeGetMeshOffset(int type);
    GSXR_EXPORT int GSXR_nativeGetPlaneNum();
    GSXR_EXPORT void GSXR_nativeGetPlaneData(float *info);
    GSXR_EXPORT void
    GSXR_nativeSetControllerKeyEventCallback(GSXR_HANDSHANK_KEY_EVENT_CALLBACK callback);
    GSXR_EXPORT void GSXR_nativeSetControllerKeyTouchEventCallback(
            GSXR_HANDSHANK_KEY_TOUCH_EVENT_CALLBACK callback);
    GSXR_EXPORT void
    GSXR_nativeSetControllerRockerCallback(GSXR_HANDSHANK_TOUCH_EVENT_CALLBACK callback);
    GSXR_EXPORT void
    GSXR_nativeSetControllerTouchPanelEventCallback(GSXR_HANDSHANK_TOUCH_PANEL_EVENT_CALLBACK callback);
    GSXR_EXPORT void
    GSXR_nativeSetControllerTriggerCallback(GSXR_HANDSHANK_HALL_EVENT_CALLBACK callback);
    GSXR_EXPORT void GSXR_nativeSetControllerChargingEventCallback(
            GSXR_HANDSHANK_CHARGING_EVENT_CALLBACK callback);
    GSXR_EXPORT void GSXR_nativeSetControllerBatteryEventCallback(
            GSXR_HANDSHANK_BATTERY_EVENT_CALLBACK callback);
    GSXR_EXPORT void GSXR_nativeSetControllerConnectEventCallback(
            GSXR_HANDSHANK_CONNECT_EVENT_CALLBACK callback);
    GSXR_EXPORT int GSXR_nativeGetControllerBattery(int lr);
    GSXR_EXPORT int GSXR_nativeGetControllerVersion();
    GSXR_EXPORT int GSXR_nativeGetControllerMode();
    GSXR_EXPORT int GSXR_nativeGetControllerList();
    GSXR_EXPORT void GSXR_nativeSetControllerVibrate(int value);
    GSXR_EXPORT bool GSXR_nativeIsControllerConnect(int lr);
    GSXR_EXPORT void GSXR_nativeSetControllerLed(int enable);
    GSXR_EXPORT void GSXR_nativeHandShankVibrateControl(int value);
    GSXR_EXPORT int GSXR_nativeGetControllerPosture(float *orientationArray, int lr);
    GSXR_EXPORT void GSXR_nativeSetAppExit(GSXR_APP_EXIT_CALLBACK callback);
    GSXR_EXPORT int GSXR_nativeStartGesture(GSXR_LOW_POWER_WARNING_CALLBACK callback);
    GSXR_EXPORT int GSXR_nativeStopGesture();
    GSXR_EXPORT int GSXR_nativeGetHandTrackingData(uint64_t *index, float *model, float *pose);
    GSXR_EXPORT void GSXR_nativeSetGestureData(float *model, float *pose);
    GSXR_EXPORT void GSXR_nativeSetGestureCallback(GSXR_GESTURE_CHANGE_CALLBACK callback);
    GSXR_EXPORT void GSXR_nativeSetGestureModelCallback(
            GSXR_GESTURE_MODEL_DATA_CHANGE_CALLBACK callback);
    GSXR_EXPORT void
    GSXR_nativeSetHandTrackingLowPowerWarningCallback(GSXR_LOW_POWER_WARNING_CALLBACK callback);
    GSXR_EXPORT GSXRResult
    GSXR_nativeGetLatestFishEyeFrameData(bool &outBUpdate, uint32_t &outFrameIndex,
                                         uint64_t &outFrameExposureNano,
                                         uint8_t *outFrameData,
                                         float *outTRDataArray);
    GSXR_EXPORT GSXRResult GSXR_nativeGetLatestFishEyeFrameDataNoTransform(bool &outBUpdate,
                                                                           uint32_t &outFrameIndex,
                                                                           uint64_t &outFrameExposureNano,
                                                                           uint8_t *outFrameData,
                                                                           float *outTRDataArray);
    GSXR_EXPORT GSXRResult
    GSXR_nativeGetOpticsCalibrationMatrix(bool &outBLoaded, float *outTransformArray);
    GSXR_EXPORT GSXRHeadPoseState GSXR_nativeGetOpticsCalibrationPosture(float *outLeftEyeMatrix,
                                                                         float *outRightEyeMatrix,
                                                                         float *outT, float *outR,
                                                                         float predictedTimeMs);
    GSXR_EXPORT GSXRResult GSXR_nativeGetLatestFishEyeBinocularData(bool &outBUpdate,
                                                                    uint32_t &outFrameIndex,
                                                                    uint64_t &outFrameExposureNano,
                                                                    uint8_t *outLeftFrameData,
                                                                    uint8_t *outRightFrameData);
    GSXR_EXPORT int GSXR_nativeInitLayer();
    GSXR_EXPORT int GSXR_nativeStartLayerRendering();
    GSXR_EXPORT int GSXR_nativeGetAllLayersData(GSXRAllLayers *outAllLayers);
    GSXR_EXPORT int GSXR_nativeEndLayerRendering(GSXRAllLayers *allLayers);
    GSXR_EXPORT int GSXR_nativeUpdateModelMatrix(uint32_t layerId, float *modelMatrixArray);
    GSXR_EXPORT int GSXR_nativeSendActionBarCMD(uint32_t layerId, int cmd);
    GSXR_EXPORT int GSXR_nativeSetForcedDisplaySize(int width, int height);
    GSXR_EXPORT int GSXR_nativeInjectMotionEvent(uint32_t layerId, int displayID, int action, float x,
                                                 float y);
    GSXR_EXPORT int GSXR_nativeDestroyLayer();

    GSXR_EXPORT bool GSXR_nativeSupportController();
    GSXR_EXPORT int GSXR_nativeGetControllerNum();
    GSXR_EXPORT bool GSXR_nativeSupportGesture();
    GSXR_EXPORT bool GSXR_nativeSupportOpticsCalibration();
    GSXR_EXPORT void GSXR_nativeGetDeviceName(char *outDeviceName);
    GSXR_EXPORT int GSXR_nativeGetXRType();
    GSXR_EXPORT void GSXR_nativeSaveMap();
    GSXR_EXPORT bool GSXR_nativeSupportMap();
    GSXR_EXPORT bool GSXR_nativeSupportPanel();
    GSXR_EXPORT GSXRResult GSXR_nativeGetFishEyeInfo(int &outNum, int &outWidth, int &outHeight);

    GSXR_EXPORT void GSXR_nativeGetVersion(char *outVersionName);

#ifdef __cplusplus
    }
#endif

}