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
    NSLog(@"demo, conversationSyncDidComplete");
}

- (void)messageSyncDidComplete {
    NSLog(@"demo, messageSyncDidComplete");
}

- (void)messagesDidRead:(NSArray<NSString *> *)messageIds inConversation:(JConversation *)conversation {
    NSLog(@"demo, messagesDidRead, count is %lu, conversationType is %lu, conversationId is %@", (unsigned long)messageIds.count, (unsigned long)conversation.conversationType, conversation.conversationId);
}

- (void)groupMessagesDidRead:(NSDictionary<NSString *,JGroupMessageReadInfo *> *)msgs inConversation:(JConversation *)conversation {
    NSLog(@"groupMessagesDidRead, groupId is %@", conversation.conversationId);
    [msgs enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, JGroupMessageReadInfo * _Nonnull obj, BOOL * _Nonnull stop) {
        NSLog(@"messageId is %@, readCount is %d, memberCount is %d", key, obj.readCount, obj.memberCount);
    }];
}

- (void)dbDidOpen {
    NSLog(@"demo, dbDidOpen");
}

- (void)application:(UIApplication *)application didRegisterForRemoteNotificationsWithDeviceToken:(NSData *)deviceToken {
    [JIM.shared.connectionManager registerDeviceToken:deviceToken];
}

- (void)connectionStatusDidChange:(JConnectionStatus)status
                        errorCode:(JErrorCode)code
                            extra:(NSString *)extra {
    NSLog(@"demo, connectionStatusDidChange status is %lu, code is %lu", (unsigned long)status, (unsigned long)code);
    if (JConnectionStatusConnected == status) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            
            JQueryMessageOptions *option = [[JQueryMessageOptions alloc] init];
            option.searchContent = @"a";
            NSMutableArray *senderIdList = [[NSMutableArray alloc] init];
            [senderIdList addObject:@"CYXf6GNeM"];
            [senderIdList addObject:@"nkXFkybGA"];
//            option.senderUserIds = senderIdList;
            NSMutableArray *contentTypes = [NSMutableArray array];
            [contentTypes addObject:@"jg:text"];
            option.contentTypes = contentTypes;
            JConversation *c1 = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"NT9cTOPsU"];
            NSMutableArray *conversations = [NSMutableArray array];
            [conversations addObject:c1];
//            option.conversations = conversations;
            NSMutableArray *states = [NSMutableArray array];
            [states addObject:@(2)];
//            option.states = states;
            NSMutableArray *conversationTypes = [NSMutableArray array];
//            [conversationTypes addObject:@(1)];
//            [conversationTypes addObject:@(2)];
            option.conversationTypes = conversationTypes;
            
            [JIM.shared.messageManager searchMessageInConversations:option
                                                           complete:^(NSArray<JSearchConversationsResult *> *result) {
                int i = 1;
            }];
            
//            
//            JQueryMessageOptions *options = [[JQueryMessageOptions alloc] init];
//            options.searchContent = @"a";
//            NSMutableArray *conversationTypes = [NSMutableArray array];
////            [conversationTypes addObject:@(1)];
//            [conversationTypes addObject:@(2)];
////            options.conversationTypes = conversationTypes;
//            NSArray *results = [JIM.shared.messageManager getMessages:100 time:0 direction:JPullDirectionOlder queryOption:options];
//            int i = 1;
        });
    }
}

- (void)dbDidClose {
    
}

- (void)chatroomDidJoin:(NSString *)chatroomId {
    NSLog(@"demo, chatroomDidJoin, chatroomId is %@", chatroomId);
    
    JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeChatroom conversationId:@"chatroom1001"];
//    JTextMessage *t = [[JTextMessage alloc] initWithContent:@"0904 test chatroom"];
//    [JIM.shared.messageManager sendMessage:t
//                            inConversation:c
//                                   success:^(JMessage *message) {
//        NSLog(@"demo, chatroom send message success");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"demo, chatroom send message error");
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
//        NSLog(@"demo, chatroom send message success");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"demo, chatroom send message error");
//    }];
}

- (void)chatroomDidQuit:(NSString *)chatroomId {
    NSLog(@"demo, chatroomDidQuit, chatroomId is %@", chatroomId);
}

- (void)chatroomJoinFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"demo, chatroomJoinFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomQuitFail:(NSString *)chatroomId errorCode:(JErrorCode)errorCode {
    NSLog(@"demo, chatroomQuitFail, chatroomId is %@, errorCode is %ld", chatroomId, errorCode);
}

- (void)chatroomDidDestroy:(NSString *)chatroomId {
    NSLog(@"demo, chatroomDidDestroy, chatroomId is %@", chatroomId);
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
        NSLog(@"demo, sendMessage success, ");
    } error:^(JErrorCode errorCode, JMessage *message) {
        NSLog(@"demo, sendMessage error");
        JMessage *msg = [JIM.shared.messageManager resend:message
                                  success:^(JMessage *message) {
            NSLog(@"demo, resend success, ");
        } error:^(JErrorCode errorCode, JMessage *message) {
            NSLog(@"demo, resend error");
        }];
        NSLog(@"after resend, msgNo is %lld", msg.clientMsgNo);
    }];
//    NSLog(@"after send video, msgNo is %lld", m.clientMsgNo);
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:image
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"demo, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"demo, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:file
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"demo, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"demo, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:voice
//                                 inConversation:conversation
//                                        success:^(JMessage *message) {
//        NSLog(@"demo, sendMessage success, ");
//    } error:^(JErrorCode errorCode, JMessage *message) {
//        NSLog(@"demo, sendMessage error");
//    }];
}

- (void)messageDidReceive:(JMessage *)message {
    NSLog(@"demo, messageDidReceive conversationType is %ld, conversationId is %@", message.conversation.conversationType, message.conversation.conversationId);
    JMessageContent *content = message.content;
    if ([content isKindOfClass:[JTextMessage class]]) {
        NSLog(@"demo, text messageDidReceive, content is %@, extra is %@", ((JTextMessage *)content).content, ((JTextMessage *)content).extra);
    } else if ([content isKindOfClass:[JImageMessage class]]) {
        NSLog(@"demo, image messageDidReceive, thumb is %@, url is %@, width is %d, height is %d, extra is %@", ((JImageMessage *)content).thumbnailUrl, ((JImageMessage *)content).url, ((JImageMessage *)content).width, ((JImageMessage *)content).height, ((JImageMessage *)content).extra);
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        NSLog(@"demo, file messageDidReceive, name is %@, url is %@, size is %lld, type is %@, extra is %@", ((JFileMessage *)content).name, ((JFileMessage *)content).url, ((JFileMessage *)content).size, ((JFileMessage *)content).type, ((JFileMessage *)content).extra);
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        NSLog(@"demo, voice messageDidReceive, url is %@, duration is %ld, extra is %@", ((JVoiceMessage *)content).url, ((JVoiceMessage *)content).duration, ((JVoiceMessage *)content).extra);
    }
    
////    if ([content isKindOfClass:[JMediaMessageContent class]]) {
////        [JIM.shared.messageManager downloadMediaMessage:message.messageId
////                                               progress:^(JMessage *message, int progress) {
////            NSLog(@"demo, download progress %d", progress);
//////            [JIM.shared.messageManager cancelDownloadMediaMessage:message.messageId];
////        } success:^(JMessage *message) {
////            NSLog(@"demo, download success");
////        } error:^(JErrorCode errorCode) {
////            NSLog(@"demo, download error code is %d", errorCode);
////        }];
//    }
}

- (void)messageDidRecall:(JMessage *)message {
    NSLog(@"demo, messageDidRecall");
}

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"demo, conversationInfoDidAdd, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"demo, conversationInfoDidUpdate, count is %ld", conversationInfoList.count);
}

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"demo, conversationInfoDidDelete, count is %ld", conversationInfoList.count);
}

- (void)totalUnreadMessageCountDidUpdate:(int)count {
    NSLog(@"demo, totalUnreadMessageCountDidUpdate, count is %d", count);
}

- (void)attributesDidDelete:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"demo, attributesDidDelete, count is %ld, chatroom is %@", attributes.count, chatroomId);
}

- (void)attributesDidUpdate:(NSDictionary<NSString *,NSString *> *)attributes forChatroom:(NSString *)chatroomId {
    NSLog(@"demo, attributesDidUpdate, count is %ld, chatroom is %@", attributes.count, chatroomId);
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




