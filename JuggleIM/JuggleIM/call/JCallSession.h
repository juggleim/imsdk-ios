//
//  JCallSession.h
//  JuggleCall
//
//  Created by Fei Li on 2024/10/9.
//

#import <Foundation/Foundation.h>
#import <UIKit/UIKit.h>
#import <JuggleIM/JCallConst.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JCallMember.h>
#import <JuggleIM/JCallVideoDenoiseParams.h>
#import <JuggleIM/JConversation.h>

@protocol JCallSessionDelegate <NSObject>

@optional

/// The call has connected.
- (void)callDidConnect;

/// The call has finished.
/// - Parameter finishReason: Finish reason.
- (void)callDidFinish:(JCallFinishReason)finishReason;

/// Users were invited (used in multi-party calls).
/// - Parameter userId: Invited user IDs.
- (void)usersDidInvite:(NSArray <NSString *> *)userIdList
             inviterId:(NSString *)inviterId;

/// Users joined the call (used in multi-party calls).
/// - Parameter userId: User IDs.
- (void)usersDidConnect:(NSArray <NSString *> *)userIdList;

/// Users left the call (used in multi-party calls).
/// - Parameter userId: User IDs.
- (void)usersDidLeave:(NSArray <NSString *> *)userIdList;

/// A user enabled or disabled the camera.
/// - Parameters:
///   - enable: Whether it is enabled.
///   - userId: User ID.
- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId;

/// A user enabled or disabled the microphone.
/// - Parameters:
///   - enable: Whether it is enabled.
///   - userId: User ID.
- (void)userMicrophoneDidChange:(BOOL)enable
                         userId:(NSString *)userId;

/// Callback for user sound level changes.
/// - Parameter soundLevels: A dictionary with userId as the key and sound level as the value.
- (void)soundLevelDidUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels;

/// Callback when the first video frame is rendered.
/// - Parameter userId: User ID.
- (void)videoFirstFrameDidRender:(NSString *)userId;

/// Callback for errors during a call.
/// - Parameter errorCode: Error code.
- (void)errorDidOccur:(JCallErrorCode)errorCode;

@end

@protocol JCallSession <NSObject>
/// Call ID.
@property (nonatomic, copy) NSString *callId;
/// Whether this is a multi-party call. NO means a one-to-one call.
@property (nonatomic, assign) BOOL isMultiCall;
/// Media type (audio/video).
@property (nonatomic, assign) JCallMediaType mediaType;
/// Call status.
@property (nonatomic, assign) JCallStatus callStatus;
/// Call start time. In multi-party sessions, this is when the current user was called and may differ from the overall call start time.
@property (nonatomic, assign) long long startTime;
/// Time when the current user joined the call.
@property (nonatomic, assign) long long connectTime;
/// Time when the current user finished the call.
@property (nonatomic, assign) long long finishTime;
/// ID of the call initiator.
@property (nonatomic, copy) NSString *owner;
/// ID of the user who invited the current user to the call.
@property (nonatomic, copy) NSString *inviterId;
/// Call finish reason.
@property (nonatomic, assign) JCallFinishReason finishReason;
/// Call participants, excluding the current user.
@property (nonatomic, copy, readonly) NSArray <JCallMember *> *members;
/// Current user.
@property (nonatomic, strong, readonly) JCallMember *currentCallMember;
/// Associated conversation.
@property (nonatomic, strong) JConversation *conversation;
/// Extra field.
@property (nonatomic, copy) NSString *extra;

- (void)addDelegate:(id<JCallSessionDelegate>)delegate;

/// Accept an incoming call.
- (void)accept;

/// Hang up the call.
- (void)hangup;

/// Enable the camera.
/// - Parameter isEnable: Whether to enable it.
- (void)enableCamera:(BOOL)isEnable;

/// Set the video view for a user.
/// - Parameters:
///   - view: Video view.
///   - userId: User ID, either the current user or another user in the session.
- (void)setVideoView:(UIView *)view
           forUserId:(NSString *)userId;

/// Start preview.
/// - Parameter view: Video view for the preview.
- (void)startPreview:(UIView *)view;

/// Stop preview.
- (void)stopPreview;

/// Mute or unmute the microphone.
/// - Parameter isMute: Whether to mute it.
- (void)muteMicrophone:(BOOL)isMute;

/// Mute or unmute the speaker.
/// - Parameter isMute: Whether to mute it.
- (void)muteSpeaker:(BOOL)isMute;

/// Enable or disable speakerphone audio.
/// - Parameter isEnable: YES to use the speakerphone; NO to use the receiver.
- (void)setSpeakerEnable:(BOOL)isEnable;

/// Switch the camera. The default is YES, which uses the front camera.
/// - Parameter isEnable: YES to use the front camera; NO to use the rear camera.
- (void)useFrontCamera:(BOOL)isEnable;

#pragma mark -
/// Invite users to join the call. This is not supported when isMultiCall is NO.
/// - Parameter userIdList: List of user IDs to invite.
- (void)inviteUsers:(NSArray <NSString *> *)userIdList;

#pragma mark - only for Zego
/// Enable acoustic echo cancellation.
/// - Parameter isEnable: Whether to enable it.
- (void)enableAEC:(BOOL)isEnable;

/// Set video denoise parameters.
/// - Parameter params: Video denoise parameters.
- (void)setVideoDenoiseParams:(JCallVideoDenoiseParams *)params;

@end
