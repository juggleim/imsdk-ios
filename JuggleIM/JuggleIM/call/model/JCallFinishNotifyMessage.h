//
//  JCallFinishNotifyMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2024/11/27.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JCallFinishNotifyType) {
    // Caller canceled.
    JCallFinishNotifyTypeCancel = 0,
    // Callee rejected.
    JCallFinishNotifyTypeReject = 1,
    // Callee did not respond.
    JCallFinishNotifyTypeNoResponse = 2,
    // Call ended.
    JCallFinishNotifyTypeComplete = 3
};

@interface JCallFinishNotifyMessage : JMessageContent
// Finish type.
@property (nonatomic, assign) JCallFinishNotifyType finishType;
// Call duration, in milliseconds.
@property (nonatomic, assign) long long duration;
// Call type.
@property (nonatomic, assign) JCallMediaType mediaType;

@end

NS_ASSUME_NONNULL_END
