//
//  Copyright (c) 2018 Agora. All rights reserved.
//

#import <Foundation/Foundation.h>

@class AgoraMediaRecorder;
@class AgoraMediaRecorderInfo;
typedef NS_ENUM(NSInteger, AgoraMediaRecorderState);
typedef NS_ENUM(NSInteger, AgoraMediaRecorderReasonCode);

@protocol AgoraMediaRecorderDelegate <NSObject>
#pragma mark Media Recorder Delegate Methods
/**-----------------------------------------------------------------------------
 * @name Media Recorder Delegate Methods
 * -----------------------------------------------------------------------------
 */

/** Occurs when media recorder state is changed.
 *
 * @since v4.0.0
 * 
 * When the local audio and video recording state changes, the SDK triggers this callback to report the current
 * recording state and the reason for the change.
 * 
 * @param recorder AgoraMediaRecorder object.
 * @param state    Recorder state. See AgoraMediaRecorderState.
 * @param reason    Error code. See AgoraMediaRecorderReasonCode.
 * @param channelId The channel name.
 * @param uid ID of the user.
 */
- (void)mediaRecorder:(AgoraMediaRecorder* _Nonnull)recorder stateDidChanged:(NSString * _Nonnull)channelId uid:(NSUInteger)uid state:(AgoraMediaRecorderState)state reason:(AgoraMediaRecorderReasonCode)reason;

/** Occurs when media recorder information is updated.
 *
 * @since v4.0.0
 * 
 * After you successfully register this callback and enable the local audio and video recording, the SDK periodically triggers
 * the `onRecorderInfoUpdated` callback based on the set value of `recorderInfoUpdateInterval`. This callback reports the
 * filename, duration, and size of the current recording file.
 * 
 * @param recorder AgoraMediaRecorder object.
 * @param info     Information about the recording file.. See AgoraMediaRecorderInfo.
 * @param channelId The channel name.
 * @param uid ID of the user.
 */
- (void)mediaRecorder:(AgoraMediaRecorder* _Nonnull)recorder informationDidUpdated:(NSString * _Nonnull)channelId uid:(NSUInteger)uid info:(AgoraMediaRecorderInfo* _Nonnull)info;

@end
