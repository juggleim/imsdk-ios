//
//  JChatroomProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <JuggleIM/JuggleIMConst.h>
#import <JuggleIM/JChatroomInfoOptions.h>
#import <JuggleIM/JChatroomInfo.h>

@protocol JChatroomDelegate <NSObject>

/// 当前用户加入聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)chatroomDidJoin:(NSString *)chatroomId;

/// 当前用户退出聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)chatroomDidQuit:(NSString *)chatroomId;

/// 加入聊天室失败
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - errorCode: 错误码
- (void)chatroomJoinFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

/// 退出聊天室失败
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - errorCode: 错误码
- (void)chatroomQuitFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

/// 当前用户被踢出聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)chatroomDidKick:(NSString *)chatroomId;

/// 聊天室被销毁
/// - Parameter chatroomId: 聊天室 id
- (void)chatroomDidDestroy:(NSString *)chatroomId;

@end

@protocol JChatroomAttributesDelegate <NSObject>

/// 聊天室属性更新（新增或者 value 有变化）
/// - Parameters:
///   - attributes: 更新的聊天室属性列表
///   - chatroomId: 聊天室 id
- (void)attributesDidUpdate:(NSDictionary <NSString *, NSString *> *)attributes
                forChatroom:(NSString *)chatroomId;

/// 聊天室属性删除
/// - Parameters:
///   - attributes: 删除的聊天室属性列表
///   - chatroomId: 聊天室 id
- (void)attributesDidDelete:(NSDictionary <NSString *, NSString *> *)attributes
                forChatroom:(NSString *)chatroomId;

@end

@protocol JChatroomProtocol <NSObject>

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
- (void)joinChatroom:(NSString *)chatroomId;

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - count: 加入聊天室时获取的历史消息数量
- (void)joinChatroom:(NSString *)chatroomId
    prevMessageCount:(int)count;

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - count: 加入聊天室时获取的历史消息数量
///   - isAutoCreate: 当聊天室不存在时是否自动创建（默认不创建）
- (void)joinChatroom:(NSString *)chatroomId
    prevMessageCount:(int)count
        isAutoCreate:(BOOL)isAutoCreate;

/// 退出聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)quitChatroom:(NSString *)chatroomId;

/// 获取聊天室信息
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - option: 拉取 option
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
//- (void)getChatroomInfo:(NSString *)chatroomId
//                 option:(JChatroomInfoOptions *)option
//                success:(void (^)(JChatroomInfo *chatroomInfo))successBlock
//                  error:(void (^)(JErrorCode code))errorBlock;

/// 设置聊天室属性。
/// - Parameters:
///   - attributes: 聊天室属性，key 和 value 都是字符串，最多支持设置 100 个不同的属性。非当前用户设置的 key 在客户端不能进行操作（返回 JErrorCodeChatroomKeyUnauthorized）。
///   - chatroomId: 聊天室 id
///   - completeBlock: 完成回调。
///                    code 返回 JErrorCodeNone 时表示所有属性都设置成功。
///                    其它 code 表示存在设置失败的 key，所有设置失败的 key 都会回调，并返回对应的错误码，可以从 JErrorCode 的定义中找到对应的错误码。
- (void)setAttributes:(NSDictionary <NSString *, NSString *> *)attributes
          forChatroom:(NSString *)chatroomId
             complete:(void (^)(JErrorCode code, NSDictionary<NSString *, NSNumber *> *failedKeys))completeBlock;

/// 删除聊天室属性
/// - Parameters:
///   - keys: 待删除的属性 key 列表。非当前用户设置的 key 不能删除。
///   - chatroomId: 聊天室 id
///   - completeBlock: 完成回调。
///                    code 返回 JErrorCodeNone 时表示所有属性都删除成功。
///                    其它 code 表示存在删除失败的 key，所有删除失败的 key 都会回调，并返回对应的错误码，可以从 JErrorCode 的定义中找到对应的错误码。
- (void)removeAttributes:(NSArray <NSString *> *)keys
             forChatroom:(NSString *)chatroomId
                complete:(void (^)(JErrorCode code, NSDictionary<NSString *, NSNumber *> *failedKeys))completeBlock;

/// 获取聊天室所有属性
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - completeBlock: 完成回调，JErrorCodeNone 表示获取成功。
- (void)getAllAttributesFromChatroom:(NSString *)chatroomId
                            complete:(void (^)(JErrorCode code, NSDictionary <NSString *, NSString *> *attributes))completeBlock;

- (void)addDelegate:(id<JChatroomDelegate>)delegate;

- (void)addAttributesDelegate:(id<JChatroomAttributesDelegate>)delegate;

@end
