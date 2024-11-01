//
//  ZegoExpressEngine+Mixer.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (Mixer)

/// Starts a stream mixing task.
///
/// Available since: 1.2.1
/// Description: Initiate a mixing stream request to the ZEGO RTC server, the server will look for the stream currently being pushed, and mix the layers according to the parameters of the mixing stream task requested by the SDK. When you need to update a mixing task, that is, when the input stream increases or decreases, you need to update the input stream list. At this time, you can update the field of the [ZegoMixerTask] object inputList and call this function again to pass in the same [ZegoMixerTask] object to update the mixing task.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
/// When to call: After calling [loginRoom] to log in to the room.
/// Restrictions: None.
/// Caution: Due to the performance considerations of the client device, the SDK muxing is to start the mixing task on the ZEGO RTC server for mixing. If an exception occurs when the mixing task is requested to start, for example, the most common mixing input stream does not exist, the error code will be given from the callback callback. If a certain input stream does not exist in the middle, the muxing task will automatically retry to pull this input stream for 90 seconds, and will not retry after 90 seconds. If all input streams no longer exist, the server will automatically stop the mixing task after 90 seconds.
/// Related callbacks: [OnMixerRelayCDNStateUpdate] can be used to obtain the CDN status update notification of the mixed stream repost, and the sound update notification of each single stream in the mixed stream can be obtained through [onMixerSoundLevelUpdate].
/// Related APIs: the mixing task can be stopped by the [stopMixerTask] function.
///
/// @param task Mixing task object. Required: Yes.
/// @param callback Start notification of mixing task results.Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)startMixerTask:(ZegoMixerTask *)task callback:(nullable ZegoMixerStartCallback)callback;

/// Stops a stream mixing task.
///
/// Available since: 1.2.1
/// Description: Initiate a request to end the mixing task to the ZEGO RTC server.
/// Use cases: It is often used when multiple video images are required to synthesize a video using mixed streaming, such as education, live broadcast of teacher and student images.
/// When to call: After calling [startMixerTask] to start mixing.
/// Restrictions: None.
/// Caution: If the developer starts the next mixing task without stopping the previous mixing task, the previous mixing task will not automatically stop until the input stream of the previous mixing task does not exist for 90 seconds. Before starting the next mixing task, you should stop the previous mixing task, so that when an anchor has already started the next mixing task to mix with other anchors, the audience is still pulling the output stream of the previous mixing task.
/// Related APIs: You can start mixing by using the [startMixerTask] function.
///
/// @param task Mixing task object. Required: Yes.
/// @param callback Stop stream mixing task result callback notification.Required: No. Caution: Passing [nil] means not receiving callback notifications.
- (void)stopMixerTask:(ZegoMixerTask *)task callback:(nullable ZegoMixerStopCallback)callback;

/// Starts an auto stream mixing task.
///
/// Available since: 2.10.0
/// Description: Local users can use this function to start an auto stream mixing task, all streams in a room wil be mixed. Currently, only audio streams can be mixed. When auto stream mixing is enabled, the audio of all streams in the room is automatically mixed, and any further pushed streams in the room are automatically mixed into the final output stream.
/// Use cases: Often used in voice chat room scenarios.
/// When to call: After the engine is created, user can call this function to enable auto stream mixing in the target room if the target room has already been created
/// Restrictions: Please refer to https://docs.zegocloud.com/article/7611 or contact ZEGO technical support.
/// Caution: Before starting the next auto stream mixing task in the same room, call [stopAutoMixerTask] function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
/// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStartCallback] callback.Users can get automatic mixed flow information through [onAutoMixerSoundLevelUpdate] callback.
/// Related APIs: Users can call [stopAutoMixerTask] function to stop an auto stream mixing task.
///
/// @param task Auto mix stream task object
/// @param callback Start auto mix stream task result callback notification
- (void)startAutoMixerTask:(ZegoAutoMixerTask *)task
                  callback:(nullable ZegoMixerStartCallback)callback;

/// Stops an auto stream mixing task.
///
/// Available since: 2.10.0
/// Description: Local users can use this function to stop an auto stream mixing task.
/// Use cases: Often used in voice chat room scenarios.
/// When to call: Call this function after [startAutoMixerTask] function is called to start an auto stream mixing task.
/// Caution: Before calling [startAutoMixerTask] function to start the next auto stream mixing task in the same room, call this function to end the last auto stream mixing task. Otherwise, when one anchor has started the next auto stream mixing task and other anchors have started the next auto stream mixing task, the audience still pulls the output stream of the first auto stream mixing task. If the user does not end the current auto stream mixing task, the task will automatically end after the room does not exist or the input streams do not exist for 90 seconds.
/// Related callbacks: Users can obtain the execution result of the function through [ZegoMixerStopCallback] callback.
/// Related APIs: Users can call [startAutoMixerTask] function to start an auto stream mixing task.
///
/// @param task Auto mix stream task object
/// @param callback Stop auto mix stream task result callback notification
- (void)stopAutoMixerTask:(ZegoAutoMixerTask *)task
                 callback:(nullable ZegoMixerStopCallback)callback;

@end

NS_ASSUME_NONNULL_END
