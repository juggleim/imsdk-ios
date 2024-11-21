//
//  JCallMediaZegoEngine.m
//  JZegoCall
//
//  Created by Fei Li on 2024/10/31.
//

#import "JCallMediaZegoEngine.h"
#import <ZegoExpressEngine/ZegoExpressEngine.h>

@interface JCallMediaZegoEngine () <ZegoEventHandler>
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
            NSString *streamId = [room.roomId stringByAppendingString:@"+++"];
            streamId = [streamId stringByAppendingString:user.userId];
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
            [[ZegoExpressEngine sharedEngine] startPlayingStream:streamId];
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
    profile.scenario = ZegoScenarioStandardVoiceCall;
    [ZegoExpressEngine createEngineWithProfile:profile eventHandler:self];
    [[ZegoExpressEngine sharedEngine] enableCamera:NO];
}

@end
