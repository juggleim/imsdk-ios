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

/// 消息生存周期，单位毫秒。例：86400000 = 24 * 60 * 60 * 1000，该消息1天后会被自动删除。
/// 默认值为 0，表示不自动销毁。
@property (nonatomic, assign) long long lifeTime;

/// 消息已读后的生存周期，单位毫秒。通常小于 lifeTime。
/// 默认值为 0，表示读后不自动销毁。
@property (nonatomic, assign) long long lifeTimeAfterRead;
@end
