//
//  AppDelegate.m
//  JetIMDemo
//
//  Created by Nathan on 2023/11/27.
//

#import "AppDelegate.h"
#import <JetIM/JetIM.h>
#import <JetIM/JTextMessage.h>

#define kToken1 @"CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"
#define kToken2 @"CgZhcHBrZXkaINodQgLnbhTbt0SzC8b/JFwjgUAdIfUZTEFK8DvDLgM1"
#define kToken3 @"CgZhcHBrZXkaINMDzs7BBTTZTwjKtM10zyxL4DBWFuZL6Z/OAU0Iajpv"
#define kToken4 @"CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"
#define kToken5 @"CgZhcHBrZXkaIOx2upLCsmsefp8U/KNb52UGnAEu/xf+im3QaUd0HTC2"

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JIM.shared setServer:@"https://nav.gxjipei.com"];
    [JIM.shared initWithAppKey:@"appkey"];
    [JIM.shared.connectionManager connectWithToken:kToken3];
    [JIM.shared.connectionManager setDelegate:self];
    [JIM.shared.messageManager setDelegate:self];
    
    return YES;
}

- (void)dbDidOpen {
    NSLog(@"lifei, dbDidOpen");
}

- (void)connectionStatusDidChange:(JConnectionStatus)status errorCode:(JErrorCode)code {
    NSLog(@"lifei, connectionStatusDidChange status is %d, code is %d", status, code);
    if (JConnectionStatusConnected == status) {
        //getConversationList
//        sleep(10);
//        NSArray *array = [JetIM.shared.conversationManager getConversationInfoList];
//        NSLog(@"conversation count is %d", array.count);
        
        //getConversation
        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
        JConversationInfo *conversationInfo = [JIM.shared.conversationManager getConversationInfo:conversation];
        NSLog(@"lifei, getConversationInfo");
        
        //getMessages
        NSArray *messages = [JIM.shared.messageManager getMessagesFrom:conversation
                                                                      count:100
                                                                       time:0
                                                                  direction:JPullDirectionOlder];
        NSLog(@"lifei, getMessagesFrom count is %d", messages.count);
        
        NSArray *messageIds = @[@"npgkkc69gaegv6fp", @"npgkkcpmaacgv6fp", @"npg3n8gb26lgrenb"];
        NSArray *messages1 = [JIM.shared.messageManager getMessagesByMessageIds:messageIds];
        NSLog(@"lifei, getMessagesByMessageIds count is %d", messages1.count);

        NSArray *clientMsgNos = @[@(3), @(6), @(2)];
        NSArray *messages2 = [JIM.shared.messageManager getMessagesByClientMsgNos:clientMsgNos];
        NSLog(@"lifei, getMessagesByMessageIds count is %d", messages2.count);

//        [JetIM.shared.messageManager deleteMessageByMessageId:@"nnx9lhzgaqeg7sb5"];
//        [JetIM.shared.messageManager deleteMessageByClientMsgNo:550];
        
        //send message
        sleep(2);
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
    JTextMessage *text = [[JTextMessage alloc] initWithContent:@"tttt"];
    JImageMessage *image = [[JImageMessage alloc] init];
    image.thumbnailUrl = @"http://sadfasdf.com";
    image.url = @"http://fsadfasdf.dd";
    image.width = 640;
    image.height = 480;
    JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid3"];

    JFileMessage *file = [[JFileMessage alloc] init];
    file.name = @"fileName";
    file.url = @"fsafasdf";
    file.size = 1000;
    file.type = @"exe";

    JVoiceMessage *voice = [[JVoiceMessage alloc] init];
    voice.url = @"voiceURL";
    voice.duration = 60;

    JMessage *m = [JIM.shared.messageManager sendMessage:text
                                 inConversation:conversation
                                        success:^(long long clientMsgNo) {
        NSLog(@"lifei, sendMessage success, ");
    } error:^(JErrorCode errorCode, long long clientMsgNo) {
        NSLog(@"lifei, sendMessage error");
    }];
    NSLog(@"after send text, msgNo is %lld", m.clientMsgNo);
    sleep(2);
//    [JIM.shared.messageManager sendMessage:image
//                                 inConversation:conversation
//                                        success:^(long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:file
//                                 inConversation:conversation
//                                        success:^(long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage error");
//    }];
//    sleep(2);
//    [JIM.shared.messageManager sendMessage:voice
//                                 inConversation:conversation
//                                        success:^(long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage success, ");
//    } error:^(JErrorCode errorCode, long long clientMsgNo) {
//        NSLog(@"lifei, sendMessage error");
//    }];
}

- (void)messageDidReceive:(JMessage *)message {
    NSLog(@"lifei, messageDidReceive conversationType is %d, conversationId is %@", message.conversation.conversationType, message.conversation.conversationId);
    JMessageContent *content = message.content;
    if ([content isKindOfClass:[JTextMessage class]]) {
        NSLog(@"lifei, text messageDidReceive, content is %@", ((JTextMessage *)content).content);
    } else if ([content isKindOfClass:[JImageMessage class]]) {
        NSLog(@"lifei, image messageDidReceive, thumb is %@, url is %@, width is %d, height is %d", ((JImageMessage *)content).thumbnailUrl, ((JImageMessage *)content).url, ((JImageMessage *)content).width, ((JImageMessage *)content).height);
    } else if ([content isKindOfClass:[JFileMessage class]]) {
        NSLog(@"lifei, file messageDidReceive, name is %@, url is %@, size is %d, type is %@", ((JFileMessage *)content).name, ((JFileMessage *)content).url, ((JFileMessage *)content).size, ((JFileMessage *)content).type);
    } else if ([content isKindOfClass:[JVoiceMessage class]]) {
        NSLog(@"lifei, voice messageDidReceive, url is %@, duration is %d", ((JVoiceMessage *)content).url, ((JVoiceMessage *)content).duration);
    }
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


