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
#import <JuggleIM/JMomentProtocol.h>

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
- (void)setServerUrls:(NSArray <NSString *> *)serverUrls;

/// 设置控制台日志输出等级
/// @param level 日志等级
- (void)setConsoleLogLevel:(JLogLevel)level;

/*!
 获取 SDK 版本
 */
- (NSString *)getSDKVersion;

- (NSString *)getDeviceId;

//服务端时间与本地时间的差值（服务端时间 - 本地时间）
- (long long)getTimeDifference;

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
/// 朋友圈管理类
@property (nonatomic, strong) id<JMomentProtocol> momentManager;

/// 按照起止时间生成日志 zip 文件，返回值为本地路径
/// - Parameters:
///   - startTime: 开始时间，单位毫秒
///   - endTime: 结束时间，单位毫秒
- (NSString *)generateZipFile:(long long)startTime
                      endTime:(long long)endTime;

/// 删除生成的日志 zip 文件
/// - Parameter fileName: 文件本地路径
- (void)removeZipFile:(NSString *)fileName;
@end

NS_ASSUME_NONNULL_END
