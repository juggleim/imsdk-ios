//
//  JStreamTextMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import <JuggleIM/JuggleIM.h>

@interface JStreamTextMessage : JMessageContent

/// Streaming message content
@property (nonatomic, copy) NSString *content;
/// Whether complete
@property (nonatomic, assign) BOOL isFinished;
/// Current fragment count
@property (nonatomic, assign) int seq;

@end

