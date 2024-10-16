//
//  JConversationProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JuggleIM/JConversation.h>
#import <JuggleIM/JConversationInfo.h>
#import <JuggleIM/JuggleIMConst.h>

@protocol JConversationDelegate <NSObject>

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)totalUnreadMessageCountDidUpdate:(int)count;

@end

@protocol JConversationSyncDelegate <NSObject>

- (void)conversationSyncDidComplete;

@end

@protocol JConversationProtocol <NSObject>

/// 获取全部会话信息列表，按照从新到旧的顺序排列
- (NSArray<JConversationInfo *> *)getConversationInfoList;

/// 分页获取会话信息列表
- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction;

/// 分页获取会话信息列表
- (NSArray<JConversationInfo *> *)getConversationInfoListByCount:(int)count
                                                       timestamp:(long long)ts
                                                       direction:(JPullDirection)direction;

/// 获取某个特定会话信息
/// - Parameter conversation: 会话标识
- (JConversationInfo *)getConversationInfo:(JConversation *)conversation;


/// 删除特定会话
/// - Parameter conversation: 会话标识
- (void)deleteConversationInfoBy:(JConversation *)conversation
                         success:(void (^)(void))successBlock
                           error:(void (^)(JErrorCode code))errorBlock;

/// 设置草稿内容
/// - Parameters:
///   - draft: 草稿内容
///   - conversation: 会话标识
- (void)setDraft:(NSString *)draft
  inConversation:(JConversation *)conversation;

/// 清除草稿
/// - Parameter conversation: 会话标识
- (void)clearDraftInConversation:(JConversation *)conversation;

/// 创建会话信息
/// - Parameters:
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)createConversationInfo:(JConversation *)conversation
                       success:(void (^)(JConversationInfo *))successBlock
                         error:(void (^)(JErrorCode code))errorBlock;

/// 获取消息未读的总数
- (int)getTotalUnreadCount;

/// 根据类型获取会话信息列表
/// - Parameter conversationTypes: 会话类型的数组，需要将 JConversationType 转为 NSNumber 并构建 NSArray
- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;

/// 清除某一个会话的未读数
/// - Parameter conversation: 会话对象
- (void)clearUnreadCountByConversation:(JConversation *)conversation
                               success:(void (^)(void))successBlock
                                 error:(void (^)(JErrorCode code))errorBlock;

/// 设置静音
/// - Parameters:
///   - isMute: YES 表示静音，NO 表示解除静音
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)setMute:(BOOL)isMute
   conversation:(JConversation *)conversation
        success:(void (^)(void))successBlock
          error:(void (^)(JErrorCode code))errorBlock;

/// 设置置顶
/// - Parameters:
///   - isTop: YES 表示置顶，NO 表示不置顶
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)setTop:(BOOL)isTop
  conversation:(JConversation *)conversation
       success:(void (^)(void))successBlock
         error:(void (^)(JErrorCode code))errorBlock;

/// 分页获取置顶会话信息列表
- (NSArray<JConversationInfo *> *)getTopConversationInfoListByCount:(int)count
                                                          timestamp:(long long)ts
                                                          direction:(JPullDirection)direction;

/// 清除总未读数
- (void)clearTotalUnreadCount:(void (^)(void))successBlock
                        error:(void (^)(JErrorCode code))errorBlock;

/// 标记未读
/// - Parameters:
///   - conversation: 会话标识
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)setUnread:(JConversation *)conversation
          success:(void (^)(void))successBlock
            error:(void (^)(JErrorCode code))errorBlock;

/// 设置置顶会话排序规则
/// - Parameter type: 排序规则
- (void)setTopConversationsOrderType:(JTopConversationsOrderType)type;

- (void)addDelegate:(id<JConversationDelegate>)delegate;

- (void)addSyncDelegate:(id<JConversationSyncDelegate>)delegate;

@end
