//
//  JConversationProtocol.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <JuggleIM/JConversation.h>
#import <JuggleIM/JConversationInfo.h>
#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JGetConversationOptions.h>

@protocol JConversationDelegate <NSObject>

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList;

- (void)totalUnreadMessageCountDidUpdate:(int)count;

@end

@protocol JConversationSyncDelegate <NSObject>

- (void)conversationSyncDidComplete;

@end

@protocol JConversationTagDelegate <NSObject>

- (void)conversationsDidAddToTag:(NSString *)tagId
                   conversations:(NSArray <JConversation *> *)conversationList;

- (void)conversationsDidRemoveFromTag:(NSString *)tagId
                        conversations:(NSArray <JConversation *> *)conversationList;

@end

@protocol JConversationProtocol <NSObject>

/// 获取全部会话信息列表，按照从新到旧的顺序排列
- (NSArray<JConversationInfo *> *)getConversationInfoList;

/// 分页获取会话信息列表，结果按照会话时间倒序排列（新的在前，旧的在后）
/// - Parameters:
///   - conversationTypes: 会话类型列表
///   - count: 拉取数量
///   - ts: 拉取时间戳（传 0 表示当前时间）
///   - direction: 拉取方向
- (NSArray<JConversationInfo *> *)getConversationInfoListWithTypes:(NSArray<NSNumber *> *)conversationTypes
                                                             count:(int)count
                                                         timestamp:(long long)ts
                                                         direction:(JPullDirection)direction;

/// 分页获取会话信息列表，结果按照会话时间倒序排列（新的在前，旧的在后）
/// - Parameters:
///   - count: 拉取数量
///   - ts: 拉取时间戳（传 0 表示当前时间）
///   - direction: 拉取方向
- (NSArray<JConversationInfo *> *)getConversationInfoListByCount:(int)count
                                                       timestamp:(long long)ts
                                                       direction:(JPullDirection)direction;

/// 根据查询条件获取会话信息列表
/// - Parameter options: 查询条件
- (NSArray<JConversationInfo *> *)getConversationInfoListWith:(JGetConversationOptions *)options;

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

/// 根据会话类型获取消息未读总数
/// - Parameter conversationTypes: 会话类型的数组，需要将 JConversationType 转为 NSNumber 并构建 NSArray
- (int)getUnreadCountWithTypes:(NSArray<NSNumber *> *)conversationTypes;

/// 根据标签 id 获取消息未读总数
/// - Parameter tagId: 标签 id
- (int)getUnreadCountWithTag:(NSString *)tagId;

/// 清除某一个会话的未读数
/// - Parameter conversation: 会话对象
- (void)clearUnreadCountByConversation:(JConversation *)conversation
                               success:(void (^)(void))successBlock
                                 error:(void (^)(JErrorCode code))errorBlock;

/// 将会话添加到标签
/// - Parameters:
///   - conversationList: 会话列表
///   - tagId: 标签 id
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)addConversationList:(NSArray <JConversation *> *)conversationList
                      toTag:(NSString *)tagId
                    success:(void (^)(void))successBlock
                      error:(void (^)(JErrorCode code))errorBlock;

/// 将会话从标签中移除
/// - Parameters:
///   - conversationList: 待移除的会话列表
///   - tagId: 标签 id
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)removeConversationList:(NSArray <JConversation *> *)conversationList
                       fromTag:(NSString *)tagId
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

- (void)addTagDelegate:(id<JConversationTagDelegate>)delegate;

@end
