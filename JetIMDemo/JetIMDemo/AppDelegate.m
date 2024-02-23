//
//  AppDelegate.m
//  JetIMDemo
//
//  Created by Nathan on 2023/11/27.
//

#import "AppDelegate.h"
#import <JetIM/JetIM.h>

#define kToken1 @"CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"
#define kToken2 @"CgZhcHBrZXkaINodQgLnbhTbt0SzC8b/JFwjgUAdIfUZTEFK8DvDLgM1"
#define kToken3 @"CgZhcHBrZXkaINMDzs7BBTTZTwjKtM10zyxL4DBWFuZL6Z/OAU0Iajpv"
#define kToken4 @"CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"
#define kToken5 @"CgZhcHBrZXkaIOx2upLCsmsefp8U/KNb52UGnAEu/xf+im3QaUd0HTC2"

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate, JMessageSyncDelegate, JConversationSyncDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
//    [JIM.shared setServer:@"https://nav.gxjipei.com"];
    [JIM.shared initWithAppKey:@"appkey"];
    [JIM.shared.connectionManager connectWithToken:kToken1];
    [JIM.shared.connectionManager setDelegate:self];
    [JIM.shared.messageManager setDelegate:self];
    [JIM.shared.messageManager setSyncDelegate:self];
    [JIM.shared.conversationManager setSyncDelegate:self];
    
    return YES;
}

- (void)conversationSyncDidComplete {
    NSLog(@"lifei, conversationSyncDidComplete");
}

- (void)messageSyncDidComplete {
    NSLog(@"lifei, messageSyncDidComplete");
}

- (void)dbDidOpen {
    NSLog(@"lifei, dbDidOpen");
}

- (void)connectionStatusDidChange:(JConnectionStatus)status errorCode:(JErrorCode)code {
    NSLog(@"lifei, connectionStatusDidChange status is %d, code is %d", status, code);
    if (JConnectionStatusConnected == status) {
        //getConversationList
        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid5"];
            [JIM.shared.conversationManager clearUnreadCountByConversation:c];
            NSArray *array = [JIM.shared.conversationManager getConversationInfoList];
            NSLog(@"conversation count is %d", array.count);
        });
        
        
        //delete conversation
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(15 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//            [JIM.shared.conversationManager deleteConversationInfoBy:c];
//        });
        
        
//
//        array = [JIM.shared.conversationManager getConversationInfoListWithTypes:@[@(JConversationTypePrivate), @(JConversationTypeGroup)] count:100 timestamp:0 direction:JPullDirectionNewer];
//
//        array = [JIM.shared.conversationManager getConversationInfoListByCount:100
//                                                                     timestamp:0
//                                                                     direction:JPullDirectionOlder];
        
        //recall message
//        sleep(2);
//        [JIM.shared.messageManager recallMessage:@"npqarw3waq8e43gq"
//                                         success:^(JMessage *message) {
//            NSLog(@"lifei, recallMessage onsuccess messageId is %@", message.messageId);
//        } error:^(JErrorCode errorCode) {
//            NSLog(@"lifei, recallMessage onError errorCode is %d", errorCode);
//        }];
//
        //getConversation
//        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid3"];
//        JConversationInfo *conversationInfo = [JIM.shared.conversationManager getConversationInfo:conversation];
//        NSLog(@"lifei, getConversationInfo");
//
//        [JIM.shared.conversationManager setDraft:@"draft" inConversation:conversation];
//        conversationInfo = [JIM.shared.conversationManager getConversationInfo:conversation];
//
//        [JIM.shared.conversationManager clearDraftInConversation:conversation];
//        conversationInfo = [JIM.shared.conversationManager getConversationInfo:conversation];
        
//        [JIM.shared.conversationManager deleteConversationInfoBy:conversation];
        
        //getMessages
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid5"];
//        sleep(5);
//        NSArray *messages = [JIM.shared.messageManager getMessagesFrom:c
//                                                                 count:100
//                                                                  time:0
//                                                             direction:JPullDirectionOlder];
//        NSLog(@"lifei");
//        NSArray *messages = [JIM.shared.messageManager getMessagesFrom:conversation
//                                                                      count:100
//                                                                       time:0
//                                                                  direction:JPullDirectionOlder];
//        NSLog(@"lifei, getMessagesFrom count is %d", messages.count);
        
//        [JIM.shared.messageManager clearMessagesIn:conversation];
        
//        NSArray *messageIds = @[@"npgkkc69gaegv6fp", @"npgkkcpmaacgv6fp", @"npg3n8gb26lgrenb"];
//        NSArray *messages1 = [JIM.shared.messageManager getMessagesByMessageIds:messageIds];
//        NSLog(@"lifei, getMessagesByMessageIds count is %d", messages1.count);
//
//        NSArray *clientMsgNos = @[@(3), @(2), @(6)];
//        NSArray *messages2 = [JIM.shared.messageManager getMessagesByClientMsgNos:clientMsgNos];
//        NSLog(@"lifei, getMessagesByMessageIds count is %d", messages2.count);
        
//        NSArray *contentTypes = @[[JImageMessage contentType]];
//        NSArray *messages3 = [JIM.shared.messageManager getMessagesFrom:conversation
//                                                                  count:100
//                                                                   time:0
//                                                              direction:JPullDirectionOlder
//                                                           contentTypes:contentTypes];
//        NSLog(@"lifei, getMessagesFrom count is %d", messages3.count);
        
        //get remote message
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate
//                                                            conversationId:@"userid5"];
//        [JIM.shared.messageManager getRemoteMessagesFrom:c
//                                               startTime:0
//                                                   count:100
//                                               direction:JPullDirectionNewer
//                                                 success:^(NSArray *messages) {
//            NSLog(@"lifei, getRemoteMessagesFrom count is %d", messages.count);
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei, getRemoteMessagesFrom error, code is ", code);
//
//        }];
//        

//        [JetIM.shared.messageManager deleteMessageByMessageId:@"nnx9lhzgaqeg7sb5"];
//        [JetIM.shared.messageManager deleteMessageByClientMsgNo:550];
        
        //send message
//        [self sendMessage];
        
        //disconnect
//        [JetIM.shared.connectionManager disconnect:NO];
        
        //get remote msgs
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
//        [JetIM.shared.messageManager getRemoteMessagesFrom:c
//                                                    startTime:[[NSDate date] timeIntervalSince1970]*1000
//                                                        count:20
//                                                    direction:JPullDirectionNewToOld
//                                                      success:nil
//                                                        error:nil];
        
    }
}

- (void)sendMessage {
    JTextMessage *text = [[JTextMessage alloc] initWithContent:@"testSendMessage"];
    text.extra = @"extra";
    JImageMessage *image = [[JImageMessage alloc] init];
    image.thumbnailUrl = @"http://sadfasdf.com";
    image.url = @"http://fsadfasdf.dd";
    image.width = 640;
    image.height = 480;
    image.extra = @"extra";
    JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid5"];

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

    JMessage *m = [JIM.shared.messageManager sendMessage:video
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
    NSLog(@"after send video, msgNo is %lld", m.clientMsgNo);
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
}

- (void)messageDidRecall:(JMessage *)message {
    NSLog(@"lifei, messageDidRecall");
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


- (void)connectionStatusDidChange:(JConnectionStatus)status {
    NSLog(@"connectionStatusDidChange status is %d", status);
}

@end


