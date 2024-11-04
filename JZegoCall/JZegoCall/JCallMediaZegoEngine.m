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
    [[ZegoExpressEngine sharedEngine] loginRoom:room.roomId
                                           user:zegoUser
                                         config:zegoConfig
                                       callback:^(int errorCode, NSDictionary * _Nonnull extendedData) {
        if (errorCode == 0) {
            [[ZegoExpressEngine sharedEngine] startPublishingStream:<#(nonnull NSString *)#>]
        }
    }];
}

#pragma mark - ZegoEventHandler

#pragma mark -
- (void)createEngineWith:(NSNumber *)appId appSign:(NSString *)appSign {
    ZegoEngineProfile *profile = [[ZegoEngineProfile alloc] init];
    profile.appID = appId.intValue;
    profile.appSign = appSign;
    profile.scenario = ZegoScenarioStandardVoiceCall;
    [ZegoExpressEngine createEngineWithProfile:profile eventHandler:self];
}

@end
