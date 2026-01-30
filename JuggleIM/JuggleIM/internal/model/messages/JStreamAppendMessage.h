//
//  JStreamAppendMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JStreamAppendMessage : JMessageContent

@property (nonatomic, copy) NSString *streamId;
@property (nonatomic, copy) NSString *content;
@property (nonatomic, assign) int seq;
@property (nonatomic, assign) BOOL isFinished;

@end

NS_ASSUME_NONNULL_END
