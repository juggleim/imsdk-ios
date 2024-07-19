//
//  JChatroomProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import "JuggleIMConst.h"
#import "JChatroomInfoOption.h"
#import "JChatroomInfo.h"

@protocol JChatroomDelegate <NSObject>

- (void)chatroomDidJoin:(NSString *)chatroomId;

- (void)chatroomDidQuit:(NSString *)chatroomId;

- (void)chatroomJoinFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

- (void)chatroomQuitFail:(NSString *)chatroomId
               errorCode:(JErrorCode)errorCode;

@end

@protocol JChatroomProtocol <NSObject>

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - createIfNotExist:  YES，当聊天室不存在时自动创建，默认值是 NO
- (void)joinChatroom:(NSString *)chatroomId
    createIfNotExist:(BOOL)createIfNotExist;

/// 退出聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)quitChatroom:(NSString *)chatroomId;

/// 获取聊天室信息
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - option: 拉取 option
///   - successBlock: 成功回调
///   - errorBlock: 失败回调
- (void)getChatroomInfo:(NSString *)chatroomId
                 option:(JChatroomInfoOption *)option
                success:(void (^)(JChatroomInfo *chatroomInfo))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

- (void)addDelegate:(id<JChatroomDelegate>)delegate;

@end
