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

@protocol JCallMediaEngineProtocol <NSObject>

- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int errorCode, NSDictionary *data))completeBlock;

- (void)leaveRoom:(NSString *)roomId;

- (void)muteMicrophone:(BOOL)isMute;

- (void)muteSpeaker:(BOOL)isMute;

- (void)setSpeakerEnable:(BOOL)isEnable;

@end

