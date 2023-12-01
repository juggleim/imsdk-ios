//
//  JuggleIM.h
//  Juggle
//
//  Created by Nathan on 2023/11/27.
//

#import <Foundation/Foundation.h>
#import "JConnectionProtocol.h"
#import "JMessageProtocol.h"
#import "JConversationProtocol.h"

@protocol JConnectionProtocol;
@protocol JMessageProtocol;
@protocol JConversationProtocol;

NS_ASSUME_NONNULL_BEGIN

@interface JuggleIM : NSObject

+ (instancetype)shared;

/*!
 初始化 SDK

 @param appKey  创建应用后获取到的 App Key
 @discussion
 在使用 SDK 所有功能之前，您必须先调用此方法初始化 SDK。
 在 App 整个生命周期中，您只需要执行一次初始化。
 */
- (void)initWithAppKey:(NSString *)appKey;

/// 连接管理类
@property(nonatomic, strong) id<JConnectionProtocol> connectionManager;
/// 消息管理类
@property(nonatomic, strong) id<JMessageProtocol> messageManager;
/// 会话管理类
@property(nonatomic, strong) id<JConversationProtocol> conversationManager;


@end

NS_ASSUME_NONNULL_END
