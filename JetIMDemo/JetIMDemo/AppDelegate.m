//
//  AppDelegate.m
//  JetIMDemo
//
//  Created by Nathan on 2023/11/27.
//

#import "AppDelegate.h"
#import <JuggleIM/JuggleIM.h>

#define kToken1 @"CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"
#define kToken2 @"CgZhcHBrZXkaINodQgLnbhTbt0SzC8b/JFwjgUAdIfUZTEFK8DvDLgM1"
#define kToken3 @"CgZhcHBrZXkaINMDzs7BBTTZTwjKtM10zyxL4DBWFuZL6Z/OAU0Iajpv"
#define kToken4 @"CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"
#define kToken5 @"CgZhcHBrZXkaIOx2upLCsmsefp8U/KNb52UGnAEu/xf+im3QaUd0HTC2"
#define kToken1181 @"ChBuc3czc3VlNzJiZWd5djd5GiB3vwQOFxILM02aHvzk0yXsSWIyWy-vkA4CLovMyoelAQ=="
#define kToken1182 @"ChBuc3czc3VlNzJiZWd5djd5GiDuv7mgMhk4e9roYlO9WeWer6_KZGn-hpJGuiMKsCI7Yw=="

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate, JMessageSyncDelegate, JConversationSyncDelegate, JConversationDelegate, JMessageReadReceiptDelegate, JMessageUploadProvider, JChatroomDelegate, JChatroomAttributesDelegate, JCallSessionDelegate, JCallReceiveDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JIM.shared setServerUrls:@[@"wss://ws.juggleim.com"]];
    [JIM.shared setConsoleLogLevel:JLogLevelVerbose];
    [JIM.shared initWithAppKey:@"nsw3sue72begyv7y"];
    [JIM.shared.connectionManager connectWithToken:kToken1182];
    [JIM.shared.connectionManager addDelegate:self];
    [JIM.shared.messageManager addDelegate:self];
    [JIM.shared.messageManager addSyncDelegate:self];
    [JIM.shared.conversationManager addSyncDelegate:self];
    [JIM.shared.conversationManager addDelegate:self];
    [JIM.shared.messageManager addReadReceiptDelegate:self];
    [JIM.shared.messageManager setMessageUploadProvider:self];
    [JIM.shared.chatroomManager addDelegate:self];
    [JIM.shared.chatroomManager addAttributesDelegate:self];
    
    [JIM.shared.callManager addReceiveDelegate:self];
    
    [JIM.shared.callManager initZegoEngineWith:1881186044 appSign:@"fa122239ebb969ac7be4b3c09a8e1350f34abc1bdb6d24af216470060c84fd6f"];
    
    
    
    return YES;
}

#pragma mark - JConnectionDelegate
- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code
                            extra:(NSString *)extra {
    NSLog(@"AppDelegate, connectionStatusDidChange status is %lu, code is %lu", (unsigned long)status, (unsigned long)code);
    if (JConnectionStatusConnected == status) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            
            [JIM.shared.callManager startSingleCall:@"nkXFkybGA" delegate:self];
        });
    }
}

- (void)dbDidOpen {
    NSLog(@"AppDelegate, dbDidOpen");
}

- (void)uploadMessage:(JMessage *)message
             progress:(void (^)(int))progressBlock
              success:(void (^)(JMessage * _Nonnull))successBlock
                error:(void (^)(void))errorBlock cancel:(void (^)(void))cancelBlock {
    dispatch_async(dispatch_get_global_queue(0, 0), ^{
        progressBlock(50);
    });
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(1 * NSEC_PER_SEC)), dispatch_get_global_queue(0, 0), ^{
        JMediaMessageContent *content = (JMediaMessageContent *)message.content;
        content.url = @"www.baidu.com";
        successBlock(message);
//        errorBlock();
    });
}

- (void)conversationSyncDidComplete {
    NSLog(@"AppDelegate, conversationSyncDidComplete");
}

- (void)messageSyncDidComplete {
    NSLog(@"AppDelegate, messageSyncDidComplete");
}

- (void)messagesDidRead:(NSArray<NSString *> *)messageIds inConversation:(JConversation *)conversation {
    NSLog(@"AppDelegate, messagesDidRead, count is %lu, conversationType is %lu, conversationId is %@", (unsigned long)messageIds.count, (unsigned long)conversation.conversationType, conversation.conversationId);
}

- (void)groupMessagesDidRead:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs inConversation:(JConversation *)conversation {
    NSLog(@"groupMessagesDidRead, groupId is %@", conversation.conversationId);
    [msgs enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, JGroupMessageReadInfo * _Nonnull obj, BOOL * _Nonnull stop) {
        NSLog(@"messageId is %@, readCount is %d, memberCount is %d", key, obj.readCount, obj.memberCount);
    }];
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    [JIM.shared.connectionManager registerDeviceToken:deviceToken];
}


- (void)chatroomDidJoin:(NSString *)chatroomId {
    NSLog(@"AppDelegate, chatroomDidJoin, chatroomId is %@", chatroomId);
}

- (void)chatroomDidQuit:(NSString *)chatroomId {
    NSLog(@"AppDelegate, chatroomDidQuit, chatroomId is %@", chatroomId);
}

- (void)chatroomJoinFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"AppDelegate, chatroomJoinFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomQuitFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"AppDelegate, chatroomQuitFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomDidDestroy:(NSString *)chatroomId {
    NSLog(@"AppDelegate, chatroomDidDestroy, chatroomId is %@", chatroomId);
}

- (void)chatroomDidKick:(NSString *)chatroomId {
}


- (void)sendMessage {
    JTextMessage *text = [[JTextMessage alloc] initWithContent:@"user5 testSendMessage"];
    text.extra = @"extra";
    JImageMessage *image = [[JImageMessage alloc] init];
    image.thumbnailUrl = @"http://sadfasdf.com";
    image.url = @"http://fsadfasdf.dd";
    image.width = 640;
    image.height = 480;
    image.extra = @"extra";
    JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];

    JFileMessage *file = [[JFileMessage alloc] init];
    file.name = @"fileName";
    file.url = @"fsafasdf";
    file.size = 1000;
    file.type = @"exe";
    file.extra = @"extra";

    JVoiceMessage *voice = [[JVoiceMessage alloc] init];
    voice.url = @"voiceURL";
    voice.duration = 60;
    voice.extra = @"extra";
    
    JVideoMessage *video = [[JVideoMessage alloc] init];
    video.url = @"http://video.com";
    video.snapshotUrl = @"http://snapshot.url";
    video.height = 480;
    video.width = 640;
    video.extra = @"extra";

    JMessage *m = [JIM.shared.messageManager sendMessage:text
                                 inConversation:conversation
                                        success:^(JMessage *message) {
        NSLog(@"AppDelegate, sendMessage success, ");
    } error:^(JErrorCode errorCode, JMessage *message) {
        NSLog(@"AppDelegate, sendMessage error");
        JMessage *msg = [JIM.shared.messageManager resend:message
                                  success:^(JMessage *message) {
            NSLog(@"AppDelegate, resend success, ");
        } error:^(JErrorCode errorCode, JMessage *message) {
            NSLog(@"AppDelegate, resend error");
        }];
        NSLog(@"after resend, msgNo is %lld", msg.clientMsgNo);
    }];
//    NSLog(@"after send video, msgNo is %lld", m.clientMsgNo);
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:image
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:file
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:voice
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"AppDelegate, sendMessage error");
//    }];
}

- (void)messageDidReceive:(JMessage *)message {
    NSLog(@"AppDelegate, messageDidReceive conversationType is %ld, conversationId is %@", message.conversation.conversationType, message.conversation.conversationId);
    JMessageContent *content = message.content;
    if ([content isKindOfClass:[JTextMessage class]]) {
        NSLog(@"AppDelegate, text messageDidReceive, content is %@, extra is %@", ((JTextMessage *)content).content, ((JTextMessage *)content).extra);
    } else if ([content isKindOfClass:[JImageMessage class]]) {
        NSLog(@"AppDelegate, image messageDidReceive, thumb is %@, url is %@, width is %d, height is %d, extra is %@", ((JImageMessage *)content).thumbnailUrl, ((JImageMessage *)content).url, ((JImageMessage *)content).width, ((JImageMessage *)content).height, ((JImageMessage *)content).extra);
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        NSLog(@"AppDelegate, file messageDidReceive, name is %@, url is %@, size is %lld, type is %@, extra is %@", ((JFileMessage *)content).name, ((JFileMessage *)content).url, ((JFileMessage *)content).size, ((JFileMessage *)content).type, ((JFileMessage *)content).extra);
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        NSLog(@"AppDelegate, voice messageDidReceive, url is %@, duration is %ld, extra is %@", ((JVoiceMessage *)content).url, ((JVoiceMessage *)content).duration, ((JVoiceMessage *)content).extra);
    }
    
////    if ([content isKindOfClass:[JMediaMessageContent class]]) {
////        [JIM.shared.messageManager downloadMediaMessage:message.messageId
////                                               progress:^(JMessage *message, int progress) {
////            NSLog(@"AppDelegate, download progress %d", progress);
//////            [JIM.shared.messageManager cancelDownloadMediaMessage:message.messageId];
////        } success:^(JMessage *message) {
////            NSLog(@"AppDelegate, download success");
////        } error:^(JErrorCode errorCode) {
////            NSLog(@"AppDelegate, download error code is %d", errorCode);
////        }];
//    }
}

- (void)messageDidRecall:(JMessage *)message {
    NSLog(@"AppDelegate, messageDidRecall");
}

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"AppDelegate, conversationInfoDidAdd, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"AppDelegate, conversationInfoDidUpdate, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"AppDelegate, conversationInfoDidDelete, count is %ld", conversationInfoList.count);
}

- (void)totalUnreadMessageCountDidUpdate:(int)count {
    NSLog(@"AppDelegate, totalUnreadMessageCountDidUpdate, count is %d", count);
}

- (void)attributesDidDelete:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"AppDelegate, attributesDidDelete, count is %ld, chatroom is %@", attributes.count, chatroomId);
}

- (void)attributesDidUpdate:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"AppDelegate, attributesDidUpdate, count is %ld, chatroom is %@", attributes.count, chatroomId);
}

#pragma mark - JCallReceiveDelegate
- (void)callDidReceive:(id<JCallSession>)callSession {
    NSLog(@"AppDelegate, callDidReceive, callId is %@", callSession.callId);
    [callSession addDelegate:self];
    [callSession hangup];
}

#pragma mark - JCallSessionDelegate
- (void)callDidConnect {
    NSLog(@"AppDelegate, callDidConnect");
}

- (void)callDidFinish:(JCallFinishReason)finishReason {
    NSLog(@"AppDelegate, callDidFinish reason is %ld", finishReason);
}

- (void)errorDidOccur:(JCallErrorCode)errorCode {
    NSLog(@"AppDelegate, callDidConnect");
}

#pragma mark - UISceneSession lifecycle


- (UISceneConfiguration *)application:(UIApplication *)application configurationForConnectingSceneSession:(UISceneSession *)connectingSceneSession options:(UISceneConnectionOptions *)options {
    // Called when a new scene session is being created.
    // Use this method to select a configuration to create the new scene with.
    return [[UISceneConfiguration alloc] initWithName:@"Default Configuration" sessionRole:connectingSceneSession.role];
}


- (void)application:(UIApplication *)application didDiscardSceneSessions:(NSSet<UISceneSession *> *)sceneSessions {
    // Called when the user discards a scene session.
    // If any sessions were discarded while the application was not running, this will be called shortly after application:didFinishLaunchingWithOptions.
    // Use this method to release any resources that were specific to the discarded scenes, as they will not return.
}

@end




