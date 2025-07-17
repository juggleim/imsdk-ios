//
//  JCallMediaEngine.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JCallMediaRoom.h>
#import <JuggleIM/JCallMediaUser.h>
#import <JuggleIM/JCallMediaRoomConfig.h>

@protocol JCallMediaEngineDelegate <NSObject>

- (UIView *)viewForUserId:(NSString *)userId;

- (UIView *)viewForSelf;

- (void)usersDidJoin:(NSArray <NSString *> *)userIdList;

- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId;

@optional
//暂不处理
- (void)usersDidLeave:(NSArray <NSString *> *)userIdList;

- (void)soundLevelDidUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels;

- (void)videoFirstFrameDidRender:(NSString *)userId;

@end

@protocol JCallMediaEngineProtocol <NSObject>

- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int errorCode, NSDictionary *data))completeBlock;

- (void)leaveRoom:(NSString *)roomId;

- (void)enableCamera:(BOOL)isEnable;

- (void)startPreview:(UIView *)view;

- (void)stopPreview;

- (void)setVideoView:(UIView *)view
              roomId:(NSString *)roomId
              userId:(NSString *)userId;

- (void)muteMicrophone:(BOOL)isMute;

- (void)muteSpeaker:(BOOL)isMute;

- (void)setSpeakerEnable:(BOOL)isEnable;

- (void)useFrontCamera:(BOOL)isEnable;

// 当前 active 的 CallSession
- (void)setDelegate:(id<JCallMediaEngineDelegate>)delegate;

@end

