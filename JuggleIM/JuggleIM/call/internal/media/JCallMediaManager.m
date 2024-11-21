//
//  JCallMediaManager.m
//  JuggleIM
//
//  Created by Fei Li on 2024/10/31.
//

#import "JCallMediaManager.h"
#import "JCallMediaEngineProtocol.h"
#import "JIM.h"

@interface JCallMediaManager ()
@property (nonatomic, strong) id<JCallMediaEngineProtocol> engine;
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
                 complete:completeBlock];
}

- (void)leaveRoom:(NSString *)callId {
    [self.engine leaveRoom:callId];
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

@end
