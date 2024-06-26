//
//  JMessageOptions.h
//  JetIM
//
//  Created by 郑开 on 2024/6/18.
//

#import <Foundation/Foundation.h>
#import "JMessageMentionInfo.h"
#import "JMessageReferredInfo.h"


@interface JMessageOptions : NSObject

/// @ 信息（非 @ 消息本字段为 nil）
@property (nonatomic, strong) JMessageMentionInfo *mentionInfo;

/// 引用消息
@property (nonatomic, copy) NSString * referredMsgId;


@end


