//
//  JCallMediaZegoEngine.m
//  JZegoCall
//
//  Created by Fei Li on 2024/10/31.
//

#import "JCallMediaZegoEngine.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

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
    zegoConfig.token = config.zegoToken;
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
        [[ZegoExpressEngine sharedEngine] startPreview:[ZegoCanvas canvasWithView:view]];
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
                                                  canvas:[ZegoCanvas canvasWithView:view]];
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

#pragma mark - ZegoEventHandler
- (void)onRoomStreamUpdate:(ZegoUpdateType)updateType
                streamList:(NSArray<ZegoStream *> *)streamList
              extendedData:(NSDictionary *)extendedData
                    roomID:(NSString *)roomID {
    if (updateType == ZegoUpdateTypeAdd) {
        for (ZegoStream *stream in streamList) {
            NSString *streamId = stream.streamID;
            NSString *userId = [self userIdWithStreamId:streamId];
            UIView *view = nil;
            if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
                view = [self.delegate viewForUserId:userId];
            }
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamId canvas:[ZegoCanvas canvasWithView:view]];
        }
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

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType
                userList:(NSArray<ZegoUser *> *)userList
                  roomID:(NSString *)roomID {
    if (updateType == ZegoUpdateTypeAdd) {
        NSMutableArray <NSString *> *userIdList = [NSMutableArray array];
        [userList enumerateObjectsUsingBlock:^(ZegoUser * _Nonnull zegoUser, NSUInteger idx, BOOL * _Nonnull stop) {
            [userIdList addObject:zegoUser.userID];
        }];
        if ([self.delegate respondsToSelector:@selector(usersDidJoin:)]) {
            [self.delegate usersDidJoin:userIdList];
        }
    } else if (updateType == ZegoUpdateTypeDelete) {
        //暂不处理
    }
}

#pragma mark -
- (void)createEngineWith:(NSNumber *)appId appSign:(NSString *)appSign {
    ZegoEngineProfile *profile = [[ZegoEngineProfile alloc] init];
    profile.appID = appId.intValue;
    profile.appSign = appSign;
    profile.scenario = ZegoScenarioStandardVideoCall;
    [ZegoExpressEngine createEngineWithProfile:profile eventHandler:self];
    [[ZegoExpressEngine sharedEngine] enableCamera:NO];
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

@end
