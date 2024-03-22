//
//  JMessageContent.h
//  JetIM
//
//  Created by Nathan on 2023/11/28.
//

#import <Foundation/Foundation.h>

typedef NS_OPTIONS(int32_t, JMessageFlag) {
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
    JMessageFlagIsMute = 1 << 6
};

NS_ASSUME_NONNULL_BEGIN
@interface JMessageContent : NSObject

/// 消息类型
+ (NSString *)contentType;

/// 消息属性，如果不设置则默认为 JMessageFlagIsCountable & JMessageFlagIsSave
+ (JMessageFlag)flags;

/// 将 json 数据反序列化，生成可用的消息内容
/// - Parameter json: 消息中的原始 json 数据
- (void)decodeWithJson:(NSDictionary *)json;


/// 将消息内容序列化，编码成 json 数据
- (NSDictionary *)encodeToJson;


/// 会话列表中显示的消息摘要
- (NSString *)conversationDigest;

@end

NS_ASSUME_NONNULL_END
