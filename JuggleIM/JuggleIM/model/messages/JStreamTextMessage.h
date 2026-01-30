//
//  JStreamTextMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import <JuggleIM/JuggleIM.h>

@interface JStreamTextMessage : JMessageContent

/// 流式消息的内容
@property (nonatomic, copy) NSString *content;
/// 是否完成
@property (nonatomic, assign) BOOL isFinished;

@end

