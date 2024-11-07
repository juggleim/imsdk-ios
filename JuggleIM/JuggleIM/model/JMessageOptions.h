//
//  JMessageOptions.h
// JuggleIM
//
//  Created by 郑开 on 2024/6/18.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JMessageMentionInfo.h>
#import <JuggleIM/JPushData.h>

@interface JMessageOptions : NSObject

/// @ 信息（非 @ 消息本字段为 nil）
@property (nonatomic, strong) JMessageMentionInfo *mentionInfo;

/// 引用消息
@property (nonatomic, copy) NSString *referredMsgId;

/// 推送信息
@property (nonatomic, strong) JPushData *pushData;
@end
