//
//  JCallActiveCallMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2025/9/1.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JCallActiveCallMessage : JMessageContent

@property (nonatomic, strong) JCallInfo *callInfo;
@property (nonatomic, assign) BOOL isFinished;

@end

NS_ASSUME_NONNULL_END
