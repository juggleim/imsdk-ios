//
//  JCallMediaManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/10/31.
//

#import "JCallMediaManager.h"
#import "JCallMediaEngineProtocol.h"
#import "JIM.h"

@interface JCallMediaManager () <JCallMediaEngineDelegate>
@property (nonatomic, strong) id<JCallMediaEngineProtocol> engine;
@property (nonatomic, weak) id<JCallMediaDelegate> delegate;
@end

@implementation JCallMediaManager

static JCallMediaManager *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
    });
    return _instance;
}

- (void)initZegoEngineWith:(int)appId appSign:(NSString *)appSign {
//    NSDictionary *dic = @{@"appId":@(appId),
//                          @"appSign":appSign};
//    NSData *data = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
//    NSString *json = [[NSString alloc] initWithData:data encoding:NSUTF8StringEncoding];
    
    Class zegoEngineClass = NSClassFromString(@"JCallMediaZegoEngine");
    self.engine = [[zegoEngineClass alloc] init];
    if ([self.engine respondsToSelector:@selector(createEngineWith:appSign:)]) {
        [self.engine performSelector:@selector(createEngineWith:appSign:) withObject:@(appId) withObject:appSign];
    }
    [self.engine setDelegate:self];
}

- (void)joinRoom:(JCallSessionImpl *)callSession
        complete:(void (^)(int, NSDictionary *))completeBlock {
    JCallMediaRoom *room = [[JCallMediaRoom alloc] init];
    room.roomId = callSession.callId;
    JCallMediaUser *user = [[JCallMediaUser alloc] init];
    user.userId = JIM.shared.currentUserId;
    JCallMediaRoomConfig *config = [[JCallMediaRoomConfig alloc] init];
    config.isUserStatusNotify = YES;
    config.zegoToken = callSession.zegoToken;
    
    [self.engine joinRoom:room
                     user:user
                   config:config
                 complete:^(int errorCode, NSDictionary *data) {
        if (errorCode == 0) {
            self.delegate = callSession;
        }
        if (completeBlock) {
            completeBlock(errorCode, data);
        }
    }];
}

- (void)leaveRoom:(NSString *)callId {
    self.delegate = nil;
    [self.engine leaveRoom:callId];
}

- (void)enableCamera:(BOOL)isEnable {
    [self.engine enableCamera:isEnable];
}

- (void)startPreview:(UIView *)view {
    [self.engine startPreview:view];
}

- (void)stopPreview {
    [self.engine stopPreview];
}

- (void)setVideoView:(UIView *)view roomId:(NSString *)roomId userId:(NSString *)userId {
    [self.engine setVideoView:view roomId:roomId userId:userId];
}

- (void)muteMicrophone:(BOOL)isMute {
    [self.engine muteMicrophone:isMute];
}

- (void)muteSpeaker:(BOOL)isMute {
    [self.engine muteSpeaker:isMute];
}

- (void)setSpeakerEnable:(BOOL)isEnable {
    [self.engine setSpeakerEnable:isEnable];
}

- (void)useFrontCamera:(BOOL)isEnable {
    [self.engine useFrontCamera:isEnable];
}

#pragma mark - JCallMediaEngineDelegate
- (UIView *)viewForUserId:(NSString *)userId {
    if ([self.delegate respondsToSelector:@selector(viewForUserId:)]) {
        return [self.delegate viewForUserId:userId];
    }
    return nil;
}

- (void)usersDidJoin:(NSArray<NSString *> *)userIdList {
    if ([self.delegate respondsToSelector:@selector(usersDidJoin:)]) {
        [self.delegate usersDidJoin:userIdList];
    }
}

- (void)userCamaraDidChange:(BOOL)enable userId:(NSString *)userId {
    if ([self.delegate respondsToSelector:@selector(userCamaraDidChange:userId:)]) {
        [self.delegate userCamaraDidChange:enable userId:userId];
    }
}

@end
