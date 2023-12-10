//
//  JConversationProtocol.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import <Juggle/JConversation.h>
#import <Juggle/JConversationInfo.h>

@protocol JConversationProtocol <NSObject>

/// 获取全部会话信息列表，按照从新到旧的顺序排列
- (NSArray<JConversationInfo *> *)getConversationInfoList;

/////TODO:
//- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
//                                                             count:(int)count
//                                                         timestamp:(long long)ts
//                                                         direction:(JPullDirection)direction;

/// 获取某个特定会话信息
/// - Parameter conversation: 会话标识
- (JConversationInfo *)getConversationInfo:(JConversation *)conversation;

/// 获取消息未读的总数
- (int)getTotalUnreadCount;

/// TODO:
/// 根据类型获取会话信息列表
/// - Parameter conversationTypes: 会话类型的数组，需要将 JConversationType 转为 NSNumber 并构建 NSArray
//- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;

/// 获取某一个会话的未读数
/// - Parameter conversation: 会话对象
- (int)getUnreadCountByConversation:(JConversation *)conversation;

/// 清除某一个会话的未读数
/// - Parameter conversation: 会话对象
- (void)clearUnreadCountByConversation:(JConversation *)conversation;

@end
