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

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate, JMessageSyncDelegate, JConversationSyncDelegate, JConversationDelegate, JMessageReadReceiptDelegate, JMessageUploadProvider, JChatroomDelegate, JChatroomAttributesDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JIM.shared setServer:@[@"https://nav.juggleim.com"]];
    [JIM.shared setConsoleLogLevel:JLogLevelVerbose];
    [JIM.shared initWithAppKey:@"nsw3sue72begyv7y"];
    [JIM.shared.connectionManager connectWithToken:kToken1181];
    [JIM.shared.connectionManager addDelegate:self];
    [JIM.shared.messageManager addDelegate:self];
    [JIM.shared.messageManager addSyncDelegate:self];
    [JIM.shared.conversationManager addSyncDelegate:self];
    [JIM.shared.conversationManager addDelegate:self];
    [JIM.shared.messageManager addReadReceiptDelegate:self];
    [JIM.shared.messageManager setMessageUploadProvider:self];
    [JIM.shared.chatroomManager addDelegate:self];
    [JIM.shared.chatroomManager addAttributesDelegate:self];
    return YES;
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
    NSLog(@"lifei, conversationSyncDidComplete");
}

- (void)messageSyncDidComplete {
    NSLog(@"lifei, messageSyncDidComplete");
}

- (void)messagesDidRead:(NSArray<NSString *> *)messageIds inConversation:(JConversation *)conversation {
    NSLog(@"lifei, messagesDidRead, count is %lu, conversationType is %lu, conversationId is %@", (unsigned long)messageIds.count, (unsigned long)conversation.conversationType, conversation.conversationId);
}

- (void)groupMessagesDidRead:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs inConversation:(JConversation *)conversation {
    NSLog(@"groupMessagesDidRead, groupId is %@", conversation.conversationId);
    [msgs enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, JGroupMessageReadInfo * _Nonnull obj, BOOL * _Nonnull stop) {
        NSLog(@"messageId is %@, readCount is %d, memberCount is %d", key, obj.readCount, obj.memberCount);
    }];
}

- (void)dbDidOpen {
    NSLog(@"lifei, dbDidOpen");
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    [JIM.shared.connectionManager registerDeviceToken:deviceToken];
}

- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code
                            extra:(NSString *)extra {
    NSLog(@"lifei, connectionStatusDidChange status is %lu, code is %lu", (unsigned long)status, (unsigned long)code);
    if (JConnectionStatusConnected == status) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"NT9cTOPsU"];
            JMessageOptions *o = [[JMessageOptions alloc] init];
            JPushData *pushData = [[JPushData alloc] init];
            pushData.content = @"push content";
            pushData.extra = @"push extra";
            o.pushData = pushData;
            JMessageContent *content = [[JMessageContent alloc] init];
            JTextMessage *t = [[JTextMessage alloc] initWithContent:@"text message"];
            [JIM.shared.messageManager sendMessage:content
                                     messageOption:o
                                    inConversation:c
                                           success:^(JMessage *message) {
                int i = 1;
            } error:^(JErrorCode errorCode, JMessage *message) {
                int i = 1;
            }];
//            dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//                [JIM.shared.chatroomManager getAllAttributesFromChatroom:@"chatroom1001"
//                                                                                   complete:^(JErrorCode code, NSDictionary<NSString *,NSString *> *attributes) {
//                    int i = 1;
//                }];
//            });
            
            
        });
    }
}

- (void)dbDidClose {
    
}

- (void)chatroomDidJoin:(NSString *)chatroomId {
    NSLog(@"lifei, chatroomDidJoin, chatroomId is %@", chatroomId);
    
    JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeChatroom conversationId:@"chatroom1001"];
//    JTextMessage *t = [[JTextMessage alloc] initWithContent:@"0904 test chatroom"];
//    [JIM.shared.messageManager sendMessage:t
//                            inConversation:c
//                                   success:^(JMessage *message) {
//        NSLog(@"lifei, chatroom send message success");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"lifei, chatroom send message error");
//    }];
    
    
    NSDictionary <NSString *, NSString *> *attr = @{@"key4":@"value4", @"key5":@"value5"};
//    [JIM.shared.chatroomManager setAttributes:attr
//                                  forChatroom:@"chatroom1001"
//                                     complete:^(JErrorCode code, NSDictionary<NSString *,NSNumber *> *failedKeys) {
//        int i = 1;
//    }];
//    
    NSArray <NSString *> *keys = @[@"key4"];
    
    [JIM.shared.chatroomManager removeAttributes:keys
                                     forChatroom:@"chatroom1001"
                                        complete:^(JErrorCode code, NSDictionary<NSString *,NSNumber *> *failedKeys) {
        int i = 1;
    }];
    
//    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(10 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//        [JIM.shared.chatroomManager quitChatroom:chatroomId];
//    });
//    JTextMessage *t = [[JTextMessage alloc] initWithContent:@"test chatroom"];
//    JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeChatroom conversationId:@"chatroom1001"];
//    [JIM.shared.messageManager sendMessage:t
//                            inConversation:c
//                                   success:^(JMessage *message) {
//        NSLog(@"lifei, chatroom send message success");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"lifei, chatroom send message error");
//    }];
}

- (void)chatroomDidQuit:(NSString *)chatroomId {
    NSLog(@"lifei, chatroomDidQuit, chatroomId is %@", chatroomId);
}

- (void)chatroomJoinFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"lifei, chatroomJoinFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomQuitFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"lifei, chatroomQuitFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomDidDestroy:(NSString *)chatroomId {
    NSLog(@"lifei, chatroomDidDestroy, chatroomId is %@", chatroomId);
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
        NSLog(@"lifei, sendMessage success, ");
    } error:^(JErrorCode errorCode, JMessage *message) {
        NSLog(@"lifei, sendMessage error");
        JMessage *msg = [JIM.shared.messageManager resend:message
                                  success:^(JMessage *message) {
            NSLog(@"lifei, resend success, ");
        } error:^(JErrorCode errorCode, JMessage *message) {
            NSLog(@"lifei, resend error");
        }];
        NSLog(@"after resend, msgNo is %lld", msg.clientMsgNo);
    }];
//    NSLog(@"after send video, msgNo is %lld", m.clientMsgNo);
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:image
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"lifei, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:file
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"lifei, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:voice
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"lifei, sendMessage error");
//    }];
}

- (void)messageDidReceive:(JMessage *)message {
    NSLog(@"lifei, messageDidReceive conversationType is %ld, conversationId is %@", message.conversation.conversationType, message.conversation.conversationId);
    JMessageContent *content = message.content;
    if ([content isKindOfClass:[JTextMessage class]]) {
        NSLog(@"lifei, text messageDidReceive, content is %@, extra is %@", ((JTextMessage *)content).content, ((JTextMessage *)content).extra);
    } else if ([content isKindOfClass:[JImageMessage class]]) {
        NSLog(@"lifei, image messageDidReceive, thumb is %@, url is %@, width is %d, height is %d, extra is %@", ((JImageMessage *)content).thumbnailUrl, ((JImageMessage *)content).url, ((JImageMessage *)content).width, ((JImageMessage *)content).height, ((JImageMessage *)content).extra);
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        NSLog(@"lifei, file messageDidReceive, name is %@, url is %@, size is %lld, type is %@, extra is %@", ((JFileMessage *)content).name, ((JFileMessage *)content).url, ((JFileMessage *)content).size, ((JFileMessage *)content).type, ((JFileMessage *)content).extra);
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        NSLog(@"lifei, voice messageDidReceive, url is %@, duration is %ld, extra is %@", ((JVoiceMessage *)content).url, ((JVoiceMessage *)content).duration, ((JVoiceMessage *)content).extra);
    }
    
////    if ([content isKindOfClass:[JMediaMessageContent class]]) {
////        [JIM.shared.messageManager downloadMediaMessage:message.messageId
////                                               progress:^(JMessage *message, int progress) {
////            NSLog(@"lifei, download progress %d", progress);
//////            [JIM.shared.messageManager cancelDownloadMediaMessage:message.messageId];
////        } success:^(JMessage *message) {
////            NSLog(@"lifei, download success");
////        } error:^(JErrorCode errorCode) {
////            NSLog(@"lifei, download error code is %d", errorCode);
////        }];
//    }
}

- (void)messageDidRecall:(JMessage *)message {
    NSLog(@"lifei, messageDidRecall");
}

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidAdd, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidUpdate, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidDelete, count is %ld", conversationInfoList.count);
}

- (void)totalUnreadMessageCountDidUpdate:(int)count {
    NSLog(@"lifei, totalUnreadMessageCountDidUpdate, count is %d", count);
}

- (void)attributesDidDelete:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"lifei, attributesDidDelete, count is %ld, chatroom is %@", attributes.count, chatroomId);
}

- (void)attributesDidUpdate:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"lifei, attributesDidUpdate, count is %ld, chatroom is %@", attributes.count, chatroomId);
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




