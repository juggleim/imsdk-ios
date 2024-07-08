//
//  JChatroomProtocol.h
//  JetIM
//
//  Created by Nathan on 2024/7/8.
//

#import <JetIM/JetIMConst.h>
#import "JChatroomInfoOption.h"
#import "JChatroomInfo.h"

@protocol JChatroomDelegate <NSObject>

- (void)onChatroomJoin:(NSString *)chatroomId;

- (void)onChatroomQuit:(NSString *)chatroomId;

- (void)onChatroomJoinFail:(NSString *)chatroomId
                 errorCode:(JErrorCode)errorCode;

@end

@protocol JChatroomProtocol <NSObject>

/// 加入聊天室
/// - Parameters:
///   - chatroomId: 聊天室 id
///   - createIfNotExist:  YES，当聊天室不存在时自动创建
- (void)joinChatroom:(NSString *)chatroomId
    createIfNotExist:(BOOL)createIfNotExist;

/// 退出聊天室
/// - Parameter chatroomId: 聊天室 id
- (void)quitChatroom:(NSString *)chatroomId;

- (void)getChatroomInfo:(NSString *)chatroomId
                 option:(JChatroomInfoOption *)option
                success:(void (^)(JChatroomInfo *chatroomInfo))successBlock
                  error:(void (^)(JErrorCode code))errorBlock;

- (void)addDelegate:(id<JChatroomDelegate>)delegate;

@end
