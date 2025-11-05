//
//  JCallMediaZegoEngine.m
//  JZegoCall
//
//  Created by Fei Li on 2024/10/31.
//

#import "JCallMediaZegoEngine.h"
#import <JuggleIM/JIM.h>

#define jSeperator @"+++"

@interface JCallMediaZegoEngine () <ZegoEventHandler>
@property (nonatomic, weak) id<JCallMediaEngineDelegate> delegate;
@end

@implementation JCallMediaZegoEngine

#pragma mark - JCallMediaEnginProtocol
- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int, NSDictionary *))completeBlock {
    ZegoUser *zegoUser = [ZegoUser userWithUserID:user.userId];
    ZegoRoomConfig *zegoConfig = [[ZegoRoomConfig alloc] init];
    zegoConfig.isUserStatusNotify = config.isUserStatusNotify;
    zegoConfig.token = config.token;
    [[ZegoExpressEngine sharedEngine] loginRoom:room.roomId
                                           user:zegoUser
                                         config:zegoConfig
                                       callback:^(int errorCode, NSDictionary * _Nonnull extendedData) {
        if (errorCode == 0) {
            NSString *streamId = [self streamIdWithRoomId:room.roomId
                                                   userId:user.userId];
            [[ZegoExpressEngine sharedEngine] startPublishingStream:streamId];
        }
        if (completeBlock) {
            completeBlock(errorCode, extendedData);
        }
    }];
}

- (void)leaveRoom:(NSString *)roomId {
    [[ZegoExpressEngine sharedEngine] logoutRoom];
}

- (void)startPreview:(UIView *)view {
    if (view) {
        ZegoCanvas *canvas = [self createCanvasWithView:view];
        [[ZegoExpressEngine sharedEngine] startPreview:canvas];
    }
}

- (void)stopPreview {
    [[ZegoExpressEngine sharedEngine] stopPreview];
}

- (void)enableCamera:(BOOL)isEnable {
    [[ZegoExpressEngine sharedEngine] enableCamera:isEnable];
}

- (void)useFrontCamera:(BOOL)isEnable {
    [[ZegoExpressEngine sharedEngine] useFrontCamera:isEnable];
}

- (void)setVideoView:(UIView *)view
              roomId:(NSString *)roomId
              userId:(NSString *)userId {
    if (roomId.length == 0 || userId.length == 0) {
        return;
    }
    NSString *streamId = [self streamIdWithRoomId:roomId
                                           userId:userId];
    [[ZegoExpressEngine sharedEngine] startPlayingStream:streamId
                                                  canvas:[self createCanvasWithView:view]];
}

- (void)muteMicrophone:(BOOL)isMute {
    [[ZegoExpressEngine sharedEngine] muteMicrophone:isMute];
}

- (void)muteSpeaker:(BOOL)isMute {
    [[ZegoExpressEngine sharedEngine] muteSpeaker:isMute];
}

- (void)setSpeakerEnable:(BOOL)isEnable {
    [[ZegoExpressEngine sharedEngine] setAudioRouteToSpeaker:isEnable];
}

- (void)enableAEC:(BOOL)isEnable {
    [[ZegoExpressEngine sharedEngine] enableAEC:isEnable];
}

- (void)setVideoDenoiseParams:(JCallVideoDenoiseParams *)params {
    ZegoVideoDenoiseParams *zp = [ZegoVideoDenoiseParams new];
    zp.mode = (ZegoVideoDenoiseMode)params.mode;
    zp.strength = (ZegoVideoDenoiseStrength)params.strength;
    [[ZegoExpressEngine sharedEngine] setVideoDenoiseParams:zp channel:ZegoPublishChannelMain];
}

#pragma mark - ZegoEventHandler
- (void)onRoomStreamUpdate:(ZegoUpdateType)updateType
                streamList:(NSArray<ZegoStream *> *)streamList
              extendedData:(NSDictionary *)extendedData
                    roomID:(NSString *)roomID {
    if (updateType == ZegoUpdateTypeAdd) {
        NSMutableArray <NSString *> *userIdList = [NSMutableArray array];
        for (ZegoStream *stream in streamList) {
            NSString *streamId = stream.streamID;
            NSString *userId = [self userIdWithStreamId:streamId];
            UIView *view = nil;
            if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
                view = [self.delegate viewForUserId:userId];
            }
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamId canvas:[self createCanvasWithView:view]];
            [[ZegoExpressEngine sharedEngine] startSoundLevelMonitor];
            [userIdList addObject:userId];
        }
        if ([self.delegate respondsToSelector:@selector(usersDidConnect:)]) {
            [self.delegate usersDidConnect:userIdList];
        }
    } else if (updateType == ZegoUpdateTypeDelete) {
        for (ZegoStream *stream in streamList) {
            [[ZegoExpressEngine sharedEngine] stopPlayingStream:stream.streamID];
        }
    }
    if ([sHandler respondsToSelector:@selector(onRoomStreamUpdate:streamList:extendedData:roomID:)]) {
        [sHandler onRoomStreamUpdate:updateType streamList:streamList extendedData:extendedData roomID:roomID];
    }
}

- (void)onRemoteCameraStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    NSString *userId = [self userIdWithStreamId:streamID];
    if (state == ZegoRemoteDeviceStateOpen) {
        if ([self.delegate respondsToSelector:@selector(userCamaraDidChange:userId:)]) {
            [self.delegate userCamaraDidChange:YES userId:userId];
        }
    } else if (state == ZegoRemoteDeviceStateDisable) {
        if ([self.delegate respondsToSelector:@selector(userCamaraDidChange:userId:)]) {
            [self.delegate userCamaraDidChange:NO userId:userId];
        }
    }
}

- (void)onRemoteMicStateUpdate:(ZegoRemoteDeviceState)state streamID:(NSString *)streamID {
    NSString *userId = [self userIdWithStreamId:streamID];
    if (state == ZegoRemoteDeviceStateOpen) {
        if ([self.delegate respondsToSelector:@selector(userMicrophoneDidChange:userId:)]) {
            [self.delegate userMicrophoneDidChange:YES userId:userId];
        }
    } else if (state == ZegoRemoteDeviceStateMute) {
        if ([self.delegate respondsToSelector:@selector(userMicrophoneDidChange:userId:)]) {
            [self.delegate userMicrophoneDidChange:NO userId:userId];
        }
    }
}

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType
                userList:(NSArray<ZegoUser *> *)userList
                  roomID:(NSString *)roomID {
//    NSMutableArray <NSString *> *userIdList = [NSMutableArray array];
//    [userList enumerateObjectsUsingBlock:^(ZegoUser * _Nonnull zegoUser, NSUInteger idx, BOOL * _Nonnull stop) {
//        [userIdList addObject:zegoUser.userID];
//    }];
//    if (updateType == ZegoUpdateTypeAdd) {
//        [self addUserList:userIdList];
//        if ([self.delegate respondsToSelector:@selector(usersDidConnect:)]) {
//            [self.delegate usersDidConnect:userIdList];
//        }
//    } else if (updateType == ZegoUpdateTypeDelete) {
//        [self removeUserList:userIdList];
//        //暂不处理
//    }
    if ([sHandler respondsToSelector:@selector(onRoomUserUpdate:userList:roomID:)]) {
        [sHandler onRoomUserUpdate:updateType userList:userList roomID:roomID];
    }
}

- (void)onEngineStateUpdate:(ZegoEngineState)state {
    if ([sHandler respondsToSelector:@selector(onEngineStateUpdate:)]) {
        [sHandler onEngineStateUpdate:state];
    }
}

- (void)onIMRecvCustomCommand:(NSString *)command fromUser:(ZegoUser *)fromUser roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onIMRecvCustomCommand:fromUser:roomID:)]) {
        [sHandler onIMRecvCustomCommand:command fromUser:fromUser roomID:roomID];
    }
}

- (void)onPublisherStateUpdate:(ZegoPublisherState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    if ([sHandler respondsToSelector:@selector(onPublisherStateUpdate:errorCode:extendedData:streamID:)]) {
        [sHandler onPublisherStateUpdate:state errorCode:errorCode extendedData:extendedData streamID:streamID];
    }
}

- (void)onCapturedSoundLevelUpdate:(NSNumber *)soundLevel {
    NSString *userId = JIM.shared.currentUserId;
    if (userId.length > 0) {
        if ([self.delegate respondsToSelector:@selector(soundLevelDidUpdate:)]) {
            [self.delegate soundLevelDidUpdate:@{userId: soundLevel}];
        }
    }
    if ([sHandler respondsToSelector:@selector(onCapturedSoundLevelUpdate:)]) {
        [sHandler onCapturedSoundLevelUpdate:soundLevel];
    }
}

- (void)onPlayerRenderVideoFirstFrame:(NSString *)streamID {
    NSString *userId = [self userIdWithStreamId:streamID];
    if ([self.delegate respondsToSelector:@selector(videoFirstFrameDidRender:)]) {
        [self.delegate videoFirstFrameDidRender:userId];
    }
    if ([sHandler respondsToSelector:@selector(onPlayerRenderVideoFirstFrame:)]) {
        [sHandler onPlayerRenderVideoFirstFrame:streamID];
    }
}

- (void)onRemoteSoundLevelUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    if ([self.delegate respondsToSelector:@selector(soundLevelDidUpdate:)]) {
        NSMutableDictionary *dic = [NSMutableDictionary dictionary];
        [soundLevels enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull streamId, NSNumber * _Nonnull soundLevel, BOOL * _Nonnull stop) {
            NSString *userId = [self userIdWithStreamId:streamId];
            [dic setObject:soundLevel forKey:userId];
        }];
        [self.delegate soundLevelDidUpdate:dic];
    }
    if ([sHandler respondsToSelector:@selector(onRemoteSoundLevelUpdate:)]) {
        [sHandler onRemoteSoundLevelUpdate:soundLevels];
    }
}

- (void)onPlayerStateUpdate:(ZegoPlayerState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData streamID:(NSString *)streamID {
    if ([sHandler respondsToSelector:@selector(onPlayerStateUpdate:errorCode:extendedData:streamID:)]) {
        [sHandler onPlayerStateUpdate:state errorCode:errorCode extendedData:extendedData streamID:streamID];
    }
}

- (void)onRoomStateChanged:(ZegoRoomStateChangedReason)reason errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onRoomStateChanged:errorCode:extendedData:roomID:)]) {
        [sHandler onRoomStateChanged:reason errorCode:errorCode extendedData:extendedData roomID:roomID];
    }
}

- (void)onRoomStateUpdate:(ZegoRoomState)state errorCode:(int)errorCode extendedData:(NSDictionary *)extendedData roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onRoomStateUpdate:errorCode:extendedData:roomID:)]) {
        [sHandler onRoomStateUpdate:state errorCode:errorCode extendedData:extendedData roomID:roomID];
    }
}

- (void)onRoomOnlineUserCountUpdate:(int)count roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onRoomOnlineUserCountUpdate:roomID:)]) {
        [sHandler onRoomOnlineUserCountUpdate:count roomID:roomID];
    }
}

- (void)onRoomExtraInfoUpdate:(NSArray<ZegoRoomExtraInfo *> *)roomExtraInfoList roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onRoomExtraInfoUpdate:roomID:)]) {
        [sHandler onRoomExtraInfoUpdate:roomExtraInfoList roomID:roomID];
    }
}

- (void)onNetworkQuality:(NSString *)userID upstreamQuality:(ZegoStreamQualityLevel)upstreamQuality downstreamQuality:(ZegoStreamQualityLevel)downstreamQuality {
    if ([sHandler respondsToSelector:@selector(onNetworkQuality:upstreamQuality:downstreamQuality:)]) {
        [sHandler onNetworkQuality:userID upstreamQuality:upstreamQuality downstreamQuality:downstreamQuality];
    }
}

- (void)onIMRecvBroadcastMessage:(NSArray<ZegoBroadcastMessageInfo *> *)messageList roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onIMRecvBroadcastMessage:roomID:)]) {
        [sHandler onIMRecvBroadcastMessage:messageList roomID:roomID];
    }
}

- (void)onRoomTokenWillExpire:(int)remainTimeInSecond roomID:(NSString *)roomID {
    if ([sHandler respondsToSelector:@selector(onRoomTokenWillExpire:roomID:)]) {
        [sHandler onRoomTokenWillExpire:remainTimeInSecond roomID:roomID];
    }
}

#pragma mark -
- (void)createZegoEngineWith:(NSNumber *)appId appSign:(NSString *)appSign {
    [ZegoExpressEngine destroyEngine:nil];
    ZegoEngineProfile *profile = [[ZegoEngineProfile alloc] init];
    profile.appID = appId.intValue;
    profile.appSign = appSign;
    profile.scenario = ZegoScenarioStandardVideoCall;
    [ZegoExpressEngine createEngineWithProfile:profile eventHandler:self];
    [[ZegoExpressEngine sharedEngine] enableCamera:NO];
}

+ (void)setEventHandler:(id<ZegoEventHandler>)handler {
    sHandler = handler;
}

- (NSString *)streamIdWithRoomId:(NSString *)roomId
                          userId:(NSString *)userId {
    NSString *streamId = [roomId stringByAppendingString:jSeperator];
    streamId = [streamId stringByAppendingString:userId];
    return streamId;
}

- (NSString *)userIdWithStreamId:(NSString *)streamId {
    NSArray *array = [streamId componentsSeparatedByString:jSeperator];
    NSUInteger length = array.count;
    NSString *userId = @"";
    if (length > 1) {
        userId = array[length-1];
    }
    return userId;
}

- (ZegoCanvas *)createCanvasWithView:(UIView *)view {
    ZegoCanvas *canvas = [ZegoCanvas canvasWithView:view];
    canvas.viewMode = ZegoViewModeAspectFill;
    return canvas;
}

static id<ZegoEventHandler> sHandler;
@end
