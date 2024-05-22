//
//  JMessageContent.h
//  JetIM
//
//  Created by Nathan on 2023/11/28.
//

#import <Foundation/Foundation.h>
#import "JMessageMentionInfo.h"

typedef NS_OPTIONS(int32_t, JMessageFlag) {
    JMessageFlagNone = 0,
    //是否命令消息
    JMessageFlagIsCmd = 1 << 0,
    //是否计未读数
    JMessageFlagIsCountable = 1 << 1,
    //是否状态消息
    JMessageFlagIsStatus = 1 << 2,
    //是否存入历史消息
    JMessageFlagIsSave = 1 << 3,
    //是否被修改过
    JMessageFlagIsModified = 1 << 4,
    //是否是合并转发消息
    JMessageFlagIsMerged = 1 << 5,
    //是否是免打扰消息
    JMessageFlagIsMute = 1 << 6,
    //是否广播消息（这类消息在发送方更新会话列表，但不更新会话的排序；接收方与普通消息一样）
    JMessageFlagIsBroadcast = 1 << 7
};

NS_ASSUME_NONNULL_BEGIN
@interface JMessageContent : NSObject

/// 消息类型
+ (NSString *)contentType;

/// 消息属性，如果不设置则默认为 JMessageFlagIsCountable & JMessageFlagIsSave
+ (JMessageFlag)flags;

/// 数据序列化方法
-(NSData *)encode;

/// 数据反序列化方法，生成可用的消息内容
/// - Parameter date: 数据内容
-(void)decode:(NSData *)data;

/// 会话列表中显示的消息摘要
- (NSString *)conversationDigest;

/// 本地模糊搜索内容
- (NSString *)searchContent;

/// @ 信息（非 @ 消息本字段为 nil）
@property (nonatomic, strong) JMessageMentionInfo *mentionInfo;

@end

NS_ASSUME_NONNULL_END
