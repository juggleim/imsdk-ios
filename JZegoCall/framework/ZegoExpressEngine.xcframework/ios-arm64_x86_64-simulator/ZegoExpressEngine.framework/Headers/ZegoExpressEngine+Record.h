//
//  ZegoExpressEngine+Record.h
//  ZegoExpressEngine
//
//  Copyright © 2019 Zego. All rights reserved.
//

#import "ZegoExpressDefines.h"
#import "ZegoExpressEngine.h"

NS_ASSUME_NONNULL_BEGIN

@interface ZegoExpressEngine (Record)

/// Starts to record and directly save the data to a file.
///
/// Available since: 1.10.0
/// Description: Starts to record locally captured audio or video and directly save the data to a file, The recorded data will be the same as the data publishing through the specified channel.
/// Restrictions: None.
/// Caution: Developers should not [stopPreview] or [stopPublishingStream] during recording, otherwise the SDK will end the current recording task. The data of the media player needs to be mixed into the publishing stream to be recorded.
/// Related callbacks: Developers will receive the [onCapturedDataRecordStateUpdate] and the [onCapturedDataRecordProgressUpdate] callback after start recording.
///
/// @param config Record config.
/// @param channel Publishing stream channel.
- (void)startRecordingCapturedData:(ZegoDataRecordConfig *)config
                           channel:(ZegoPublishChannel)channel;

/// Stops recording locally captured audio or video.
///
/// Available since: 1.10.0
/// Description: Stops recording locally captured audio or video.
/// When to call: After [startRecordingCapturedData].
/// Restrictions: None.
///
/// @param channel Publishing stream channel.
- (void)stopRecordingCapturedData:(ZegoPublishChannel)channel;

/// Sets up the event callback handler for data recording.
///
/// Available since: 1.10.0
/// Description: Implemente the functions of ZegoDataRecordEventHandler to get notified when recode state and process changed.
/// When to call: After [createEngine].
/// Restrictions: None.
///
/// @param eventHandler Data record event handler.
- (void)setDataRecordEventHandler:(nullable id<ZegoDataRecordEventHandler>)eventHandler;

@end

NS_ASSUME_NONNULL_END
