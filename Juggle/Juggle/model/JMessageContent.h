//
//  JMessageContent.h
//  Juggle
//
//  Created by Nathan on 2023/11/28.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN
@interface JMessageContent : NSObject

/// 消息类型
+ (NSString *)contentType;


/// 将 json 数据反序列化，生成可用的消息内容
/// - Parameter json: 消息中的原始 json 数据
- (void)decodeWithJson:(NSDictionary *)json;


/// 将消息内容序列化，编码成 json 数据
- (NSDictionary *)encodeToJson;


/// 会话列表中显示的消息摘要
- (NSString *)conversationDigest;

@end

NS_ASSUME_NONNULL_END
