//
//  JChatroomProtocol.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import "JuggleIMConst.h"
#import "JChatroomInfoOptions.h"
#import "JChatroomInfo.h"

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

@end

@protocol JChatroomProtocol <NSObject>

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
- (void)joinChatroom:(NSString *)chatroomId;

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
                 option:(JChatroomInfoOptions *)option
                success:(void (^)(JChatroomInfo *chatroomInfo))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

- (void)addDelegate:(id<JChatroomDelegate>)delegate;

@end
