//
//  JCallMediaEngine.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <Foundation/Foundation.h>
#import "JCallMediaRoom.h"
#import "JCallMediaUser.h"
#import "JCallMediaRoomConfig.h"

@protocol JCallMediaRoomConfig <NSObject>
@property (nonatomic, assign) BOOL isUserStatusNotify;
@end

@protocol JCallMediaEngineProtocol <NSObject>

- (void)joinRoom:(JCallMediaRoom *)room
            user:(JCallMediaUser *)user
          config:(JCallMediaRoomConfig *)config
        complete:(void (^)(int errorCode, NSDictionary *data))completeBlock;

@end

