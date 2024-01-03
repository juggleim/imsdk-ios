//
//  JetIM.h
//  JetIM
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>
#import <JetIM/JConnectionProtocol.h>
#import <JetIM/JMessageProtocol.h>
#import <JetIM/JConversationProtocol.h>

@protocol JConnectionProtocol;
@protocol JMessageProtocol;
@protocol JConversationProtocol;

NS_ASSUME_NONNULL_BEGIN

@interface JetIM : NSObject

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

 @param serverUrl  服务器地址
 */
- (void)setServer:(NSString *)serverUrl;

/// 连接管理类
@property(nonatomic, strong) id<JConnectionProtocol> connectionManager;
/// 消息管理类
@property(nonatomic, strong) id<JMessageProtocol> messageManager;
/// 会话管理类
@property(nonatomic, strong) id<JConversationProtocol> conversationManager;
/// SDK 中所有代理的回调线程，如果不指定则默认使用主线程回调
@property(nonatomic, strong) dispatch_queue_t delegateQueue;
@end

NS_ASSUME_NONNULL_END
