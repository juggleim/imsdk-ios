//
//  JCallMediaManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/31.
//

#import <Foundation/Foundation.h>
#import "JCallSessionImpl.h"
#import "JCallMediaDelegate.h"

@interface JCallMediaManager : NSObject

+ (instancetype)shared;

- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

- (void)joinRoom:(JCallSessionImpl *)callSession
        complete:(void (^)(int, NSDictionary *))completeBlock;

- (void)leaveRoom:(NSString *)callId;

- (void)enableCamera:(BOOL)isEnable;

- (void)startPreview:(UIView *)view;

- (void)setVideoView:(UIView *)view
              roomId:(NSString *)roomId
              userId:(NSString *)userId;

- (void)muteMicrophone:(BOOL)isMute;

- (void)muteSpeaker:(BOOL)isMute;

- (void)setSpeakerEnable:(BOOL)isEnable;

@end

