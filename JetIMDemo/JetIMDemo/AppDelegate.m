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

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate, JMessageSyncDelegate, JConversationSyncDelegate, JConversationDelegate, JMessageReadReceiptDelegate, JMessageUploadProvider, JChatroomDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JIM.shared setServer:@[@"https://nav.juggleim.com"]];
    [JIM.shared setConsoleLogLevel:JLogLevelVerbose];
    [JIM.shared initWithAppKey:@"nsw3sue72begyv7y"];
    [JIM.shared.connectionManager connectWithToken:@"ChBuc3czc3VlNzJiZWd5djd5GiDuv7mgMhk4e9roYlO9WeWer6_KZGn-hpJGuiMKsCI7Yw=="];
    [JIM.shared.connectionManager addDelegate:self];
    [JIM.shared.messageManager addDelegate:self];
    [JIM.shared.messageManager addSyncDelegate:self];
    [JIM.shared.conversationManager addSyncDelegate:self];
    [JIM.shared.conversationManager addDelegate:self];
    [JIM.shared.messageManager addReadReceiptDelegate:self];
    [JIM.shared.messageManager setMessageUploadProvider:self];
    [JIM.shared.chatroomManager addDelegate:self];
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
    NSLog(@"lifei, messagesDidRead, count is %d, conversationType is %d, conversationId is %@", messageIds.count, conversation.conversationType, conversation.conversationId);
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
    NSLog(@"lifei, connectionStatusDidChange status is %d, code is %d", status, code);
    if (JConnectionStatusConnected == status) {
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(2 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"rdyIjfi8R"];
            
            
            
            
//            JGetMessageOptions *option = [[JGetMessageOptions alloc] init];
//            option.count = 100;
//            option.contentTypes = @[@"jg:text", @"jgd:grpntf"];
//            [JIM.shared.messageManager getMessages:c
//                                         direction:JPullDirectionOlder
//                                            option:option
//                                 localMessageBlock:^(NSArray<JMessage *> *messages, JErrorCode code) {
//                int i = 0;
//            } remoteMessageBlock:^(NSArray<JMessage *> *messages, long long timestamp, BOOL hasMore, JErrorCode code) {
//                int i = 0;
//            }];
            
//            JConversationInfo *info = [JIM.shared.conversationManager getConversationInfo:c];
//            [JIM.shared.messageManager getMentionMessages:c
//                                                    count:100
//                                                     time:0
//                                                direction:JPullDirectionOlder
//                                                  success:^(NSArray<JMessage *> *messages, BOOL isFinished) {
//                int i = 0;
//            } error:^(JErrorCode code) {
//                int i = 0;
//            }];
            
            
//            [JIM.shared.chatroomManager quitChatroom:@"chatroom1001"];
            
//            JConversation *c1 = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userId1"];
//            JConversation *c2 = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userId2"];
//            JConversation *c3 = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userId3"];
//            JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupId1"];
//            JConversationInfo *info = [JIM.shared.conversationManager getConversationInfo:conversation];
//
//            NSArray *conversations = @[c1, c2, c3, conversation];
//
//            [JIM.shared.messageManager setLocalAttribute:@"attribute1" forClientMsgNo:123];
            
            
            
           
            
//            JImageMessage *image = [[JImageMessage alloc] init];
//            image.localPath = @"asdfasdf";
//            image.size = 1000;
//            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//            [JIM.shared.messageManager sendMediaMessage:image
//                                         inConversation:c
//                                               progress:^(int progress, JMessage *message) {
//                NSLog(@"asdf");
//            } success:^(JMessage *message) {
//                NSLog(@"sadfasdf");
//            } error:^(JErrorCode errorCode, JMessage *message) {
//                NSLog(@"fasdf");
//            } cancel:^(JMessage *message) {
//                NSLog(@"fasdf");
//            }];
            
//            JThumbnailPackedImageMessage *message = [[JThumbnailPackedImageMessage alloc] init];
//            message.url = @"http://www.baidu.com";
//            message.width = 1000;
//            message.height = 2000;
//            message.size = 3434342;
//            message.extra = @"extra";
//            NSData *data = [message encode];
//            JThumbnailPackedImageMessage *m2 = [[JThumbnailPackedImageMessage alloc] init];
//            [m2 decode:data];
//            NSLog(@"asdfsadf");
        });
        
        
        
    }
}

- (void)dbDidClose {
    
}

- (void)chatroomDidJoin:(NSString *)chatroomId {
    NSLog(@"lifei, chatroomDidJoin, chatroomId is %@", chatroomId);
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
    NSLog(@"lifei, messageDidReceive conversationType is %d, conversationId is %@", message.conversation.conversationType, message.conversation.conversationId);
    JMessageContent *content = message.content;
    if ([content isKindOfClass:[JTextMessage class]]) {
        NSLog(@"lifei, text messageDidReceive, content is %@, extra is %@", ((JTextMessage *)content).content, ((JTextMessage *)content).extra);
    } else if ([content isKindOfClass:[JImageMessage class]]) {
        NSLog(@"lifei, image messageDidReceive, thumb is %@, url is %@, width is %d, height is %d, extra is %@", ((JImageMessage *)content).thumbnailUrl, ((JImageMessage *)content).url, ((JImageMessage *)content).width, ((JImageMessage *)content).height, ((JImageMessage *)content).extra);
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        NSLog(@"lifei, file messageDidReceive, name is %@, url is %@, size is %d, type is %@, extra is %@", ((JFileMessage *)content).name, ((JFileMessage *)content).url, ((JFileMessage *)content).size, ((JFileMessage *)content).type, ((JFileMessage *)content).extra);
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        NSLog(@"lifei, voice messageDidReceive, url is %@, duration is %d, extra is %@", ((JVoiceMessage *)content).url, ((JVoiceMessage *)content).duration, ((JVoiceMessage *)content).extra);
    }
    
    if ([content isKindOfClass:[JMediaMessageContent class]]) {
        [JIM.shared.messageManager downloadMediaMessage:message.messageId
                                               progress:^(JMessage *message, int progress) {
            NSLog(@"lifei, download progress %d", progress);
//            [JIM.shared.messageManager cancelDownloadMediaMessage:message.messageId];
        } success:^(JMessage *message) {
            NSLog(@"lifei, download success");
        } error:^(JErrorCode errorCode) {
            NSLog(@"lifei, download error code is %d", errorCode);
        }];
    }
}

- (void)messageDidRecall:(JMessage *)message {
    NSLog(@"lifei, messageDidRecall");
}

- (void)conversationInfoDidAdd:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidAdd, count is %d", conversationInfoList.count);
}

- (void)conversationInfoDidUpdate:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidUpdate, count is %d", conversationInfoList.count);
}

- (void)conversationInfoDidDelete:(NSArray<JConversationInfo *> *)conversationInfoList {
    NSLog(@"lifei, conversationInfoDidDelete, count is %d", conversationInfoList.count);
}

- (void)totalUnreadMessageCountDidUpdate:(int)count {
    NSLog(@"lifei, totalUnreadMessageCountDidUpdate, count is %d", count);
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




