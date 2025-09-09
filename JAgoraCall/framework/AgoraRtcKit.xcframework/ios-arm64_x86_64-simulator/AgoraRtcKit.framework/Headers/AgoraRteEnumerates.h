#import <Foundation/Foundation.h>

/**
 * Player states. When the player state changes, the state will be notified through the [AgoraRtePlayerObserver onStateChanged:newState:error] callback interface.
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRtePlayerState) {
  /**
   * 0: Idle state.
   */
  AgoraRtePlayerStateIdle = 0,
  /**
   * 1: Opening state. This state is notified after calling [AgoraRtePlayer openWithUrl:startTime:cb].
   */
  AgoraRtePlayerStateOpening = 1,
  /**
   * 2: Open completed state. This state is notified after successfully calling [AgoraRtePlayer openWithUrl:startTime:cb].
   */
  AgoraRtePlayerStateOpenCompleted = 2,
  /**
   * 3: Playing state. This state is notified when the url source is playing.
   */
  AgoraRtePlayerStatePlaying = 3,
  /**
   * 4: Paused state. This state is notified when playback is paused.
   */
  AgoraRtePlayerStatePaused = 4,
  /**
   * Playback completed state. This state is notified when the url source playback completed
   */
  AgoraRtePlayerStatePlaybackCompleted = 5,
  /**
   * 6: Stopped state. This state is entered after the user calls [AgoraRtePlayer stop:].
   */
  AgoraRtePlayerStateStopped = 6,
  /**
   * 7: Failed state. This state is entered when an internal error occurs.
   */
  AgoraRtePlayerStateFailed = 7
};

/**
 * Player events. When an event occurs, it will be notified through the [AgoraRtePlayerObserver onEvent:] callback interface.
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRtePlayerEvent) {
  /**
   * 0: Start seeking to a specified position for playback.
   */
  AgoraRtePlayerEventSeekBegin = 0,
  /**
   * 1: Seeking completes.
   */
  AgoraRtePlayerEventSeekComplete = 1,
  /**
   * 2: An error occurs when seeking to a new playback position.
   */
  AgoraRtePlayerEventSeekError = 2,
  /**
   * 3: The currently buffered data is not enough to support playback.
   */
  AgoraRtePlayerEventBufferLow = 3,
  /**
   * 4: The currently buffered data is just enough to support playback.
   */
  AgoraRtePlayerEventBufferRecover = 4,
  /**
   * 5: Audio or video playback starts freezing.
   */
  AgoraRtePlayerEventFreezeStart = 5,
  /**
   * 6: The audio or video playback resumes without freezing.
   */
  AgoraRtePlayerEventFreezeStop = 6,
  /**
   * 7: One loop playback completed.
   */
  AgoraRtePlayerEventOneLoopPlaybackCompleted = 7,
  /**
   * 8: URL authentication will expire.
   */
  AgoraRtePlayerEventAuthenticationWillExpire = 8,
  /**
   * 9: When the fallback option is enabled, ABR revert to the audio-only layer due to poor network.
   */
  AgoraRtePlayerEventAbrFallbackToAudioOnlyLayer = 9,
  /**
   * 10: ABR recovers from audio-only layer to video layer when fallback option is enabled.
   */
  AgoraRtePlayerEventAbrRecoverFromAudioOnlyLayer = 10,
  /**
   * 11: Start switching to a new URL.
   */
  AgoraRtePlayerEventSwitchBegin = 11,
   /**
   * 12: Switching to a new URL completes.
   */
  AgoraRtePlayerEventSwitchComplete = 12,
  /**
   * 13: An error occurs when switching to a new URL.
   */
  AgoraRtePlayerEventSwitchError = 13,
  /**
   * 14: The first frame of the video is displayed.
   */
  AgoraRtePlayerEventFirstDisplayed = 14,
  /**
   * 15: The number of cached files reaches the maximum.
   */
  AgoraRtePlayerEventReachCacheFileMaxCount = 15,
  /**
   * 16: The size of the cached file reaches the maximum.
   */
  AgoraRtePlayerEventReachCacheFileMaxSize = 16,
  /**
   * 17: Start trying to open a new URL.
   */
  AgoraRtePlayerEventTryOpenStart = 17,
  /**
   * 18: Trying to open a new URL succeeds.
   */
  AgoraRtePlayerEventTryOpenSucceed = 18,
  /**
   * 19: Trying to open a new URL fails.
   */
  AgoraRtePlayerEventTryOpenFailed = 19,
  /**
   * 20: Audio track changed.
   */
  AgoraRtePlayerEventAudioTrackChanged = 20
};

/**
 * Render mode
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRteVideoRenderMode) {
  /**
   * 0: The hidden mode will fill the entire view. Parts of the image that exceed the view will be cropped.
   */
  AgoraRteVideoRenderModeHidden = 0,
  /**
   * 1: The fit mode will render the entire image within the view.
   */
  AgoraRteVideoRenderModeFit = 1
};

/**
 * Mirror mode
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRteVideoMirrorMode) {
  /**
   * 0: The SDK decides the mirror mode.
   */
  AgoraRteVideoMirrorModeAuto = 0,
  /**
   * 1: Enable mirror mode.
   */
  AgoraRteVideoMirrorModeEnabled = 1,
  /**
   * 2: Disable mirror mode.
   */
  AgoraRteVideoMirrorModeDisabled = 2
};

/**
 * Metadata type
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRtePlayerMetadataType) {
  /**
   * SEI type
   */
  AgoraRtePlayerMetadataTypeSei = 0,
};

/**
 * Error codes
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRteErrorCode) {
  /**
   * 0: Success
   */
  AgoraRteOk = 0,
  /**
   * 1: Default, not specifically categorized
   */
  AgoraRteErrorDefault = 1,
  /**
   * 2: Invalid argument passed to API call
   */
  AgoraRteErrorInvalidArgument = 2,
  /**
   * 3: Unsupported API operation
   */
  AgoraRteErrorInvalidOperation = 3,
  /**
   * 4: Network error
   */
  AgoraRteErrorNetworkError = 4,
  /**
   * 5: Authentication failed
   */
  AgoraRteErrorAuthenticationFailed = 5,
  /**
   * 6: Stream not found
   */
  AgoraRteErrorStreamNotFound = 6,
};


/**
 * ABR subscription layer. This enumeration can be used to set the value of the abr_subscription_layer query parameter in the rte URL. 
 * It can also be used in the [AgoraRtePlayerConfig setAbrSubscriptionLayer:error:] setting interface.
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRteAbrSubscriptionLayer) {
  /**
   * 0: High-quality video stream, this layer has the highest resolution and bitrate.
   */
  AgoraRteAbrSubscriptionHigh = 0,
  /**
   * 1: Low-quality video stream, this layer has the lowest resolution and bitrate.
   */
  AgoraRteAbrSubscriptionLow = 1,
  /**
   * 2: Layer1 video stream, this layer has lower resolution and bitrate than that of the high-quality video stream.
   */
  AgoraRteAbrSubscriptionLayer1 = 2,
  /**
   * 3: Layer2 video stream, this layer has lower resolution and bitrate than layer1.
   */
  AgoraRteAbrSubscriptionLayer2 = 3,
  /**
   * 4: Layer3 video stream, this layer has lower resolution and bitrate than layer2.
   */
  AgoraRteAbrSubscriptionLayer3 = 4,
  /**
   * 5: Layer4 video stream, this layer has lower resolution and bitrate than layer3.
   */
  AgoraRteAbrSubscriptionLayer4 = 5,
  /**
   * 6: Layer5 video stream, this layer has lower resolution and bitrate than layer4.
   */
  AgoraRteAbrSubscriptionLayer5 = 6,
  /**
   * 7: Layer6 video stream, this layer has lower resolution and bitrate than layer5.
   */
  AgoraRteAbrSubscriptionLayer6 = 7,
};


/**
 * ABR fallback layer. This enumeration can be used to set the value of the abr_fallback_layer query parameter in the rte URL. 
 * It can also be used in the [AgoraRtePlayerConfig setAbrFallbackLayer:error:] setting interface.
 * @since v4.4.0
 */
typedef NS_ENUM(NSUInteger, AgoraRteAbrFallbackLayer) {
  /**
   * 0: When the network quality is poor, it will not revert to a lower resolution stream. 
   * It may still revert to scalable video coding but will maintain the high-quality video resolution.
   */
  AgoraRteAbrFallbackDisabled = 0,
  /**
   * 1: (Default) In a poor network environment, the receiver's SDK will receive the kRteAbrSubscriptionLow layer video stream.
   */
  AgoraRteAbrFallbackLow = 1,
  /**
   * 2: In a poor network environment, the SDK may first receive the kRteAbrSubscriptionLow layer, and if the relevant layer exists, 
   * it will revert to kRteAbrSubscriptionLayer1 to kRteAbrSubscriptionLayer6. If the network environment is too poor to play video, the SDK will only receive audio.
   */
  AgoraRteAbrFallbackAudioOnly = 2,
  /**
   * 3~8: If the receiving end SDK sets the fallback option, when the network quality deteriorates and the relevant layer exists, 
   * it will receive one of the layers from kRteAbrSubscriptionLayer1 to kRteAbrSubscriptionLayer6. The lower boundary of the fallback depends on the set fallback option.
   */
  AgoraRteAbrFallbackLayer1 = 3,
  AgoraRteAbrFallbackLayer2 = 4,
  AgoraRteAbrFallbackLayer3 = 5,
  AgoraRteAbrFallbackLayer4 = 6,
  AgoraRteAbrFallbackLayer5 = 7,
  AgoraRteAbrFallbackLayer6 = 8,
};
