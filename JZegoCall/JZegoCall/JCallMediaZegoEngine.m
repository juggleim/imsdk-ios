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

- (void)enableCamera:(BOOL)isEnable {
    [[ZegoExpressEngine sharedEngine] enableCamera:isEnable];
}

- (void)setVideoView:(UIView *)view
              roomId:(NSString *)roomId
              userId:(NSString *)userId {
    if (!view || roomId.length == 0 || userId.length == 0) {
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
            NSArray *array = [streamId componentsSeparatedByString:jSeperator];
            NSUInteger length = array.count;
            UIView *view = nil;
            if (length > 1) {
                NSString *userId = array[length-1];
                if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
                    view = [self.delegate viewForUserId:userId];
                }
            }
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamId canvas:[ZegoCanvas canvasWithView:view]];
        }
    }
}

- (void)onRoomUserUpdate:(ZegoUpdateType)updateType
                userList:(NSArray<ZegoUser *> *)userList
                  roomID:(NSString *)roomID {
    if (updateType == ZegoUpdateTypeAdd) {
        
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

@end
