//
//  CallCenter.h
//  QuickStart
//
//  Created by Fei Li on 2024/11/4.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIM.h>

@interface CallCenter : NSObject

+ (instancetype)shared;

- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

- (void)startSingleCall:(id<JCallSession>)callSession;

- (void)startMultiCall:(id<JCallSession>)callSession
               groupId:(NSString *)groupId;

- (void)dismissCallViewController:(UIViewController *)vc;

@end

