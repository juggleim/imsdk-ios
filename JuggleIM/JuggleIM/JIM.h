//
//  JIM.h
// JuggleIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JConnectionProtocol.h>
#import <JuggleIM/JMessageProtocol.h>
#import <JuggleIM/JConversationProtocol.h>
#import <JuggleIM/JUserInfoProtocol.h>
#import <JuggleIM/JChatroomProtocol.h>
#import <JuggleIM/JCallProtocol.h>

@protocol JConnectionProtocol;
@protocol JMessageProtocol;
@protocol JConversationProtocol;

NS_ASSUME_NONNULL_BEGIN

@interface JIM : NSObject

+ (instancetype)shared;

/*!
 初始化 SDK

 @param appKey  创建应用后获取到的 App Key
 @discussion
 在使用 SDK 所有功能之前，您必须先调用此方法初始化 SDK。
 在 App 整个生命周期中，您只需要执行一次初始化。
 */
- (void)initWithAppKey:(NSString *)appKey;

/*!
 设置服务器地址

 @param serverUrls  服务器地址
 */
- (void)setServer:(NSArray <NSString *> *)serverUrls;

/// 设置控制台日志输出等级
/// @param level 日志等级
- (void)setConsoleLogLevel:(JLogLevel)level;

/*!
 获取 SDK 版本
 */
- (NSString *)getSDKVersion;

- (NSString *)getDeviceId;

/// 激活通话功能（默认不激活，请在 initWithAppKey: 之前调用）
- (void)enableCall;

/// 连接管理类
@property (nonatomic, strong) id<JConnectionProtocol> connectionManager;
/// 消息管理类
@property (nonatomic, strong) id<JMessageProtocol> messageManager;
/// 会话管理类
@property (nonatomic, strong) id<JConversationProtocol> conversationManager;
/// 用户管理类
@property(nonatomic, strong) id<JUserInfoProtocol> userInfoManager;
/// 聊天室管理类
@property (nonatomic, strong) id<JChatroomProtocol> chatroomManager;
/// SDK 中所有代理的回调线程，如果不指定则默认使用主线程回调
@property (nonatomic, strong) dispatch_queue_t delegateQueue;
/// 当前 app key
@property (nonatomic, copy, readonly) NSString *appKey;
/// 当前用户 id
@property (nonatomic, copy, readonly) NSString *currentUserId;
/// 通话管理类
@property (nonatomic, strong) id<JCallProtocol> callManager;


@end

NS_ASSUME_NONNULL_END
