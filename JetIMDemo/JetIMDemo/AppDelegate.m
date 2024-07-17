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

@interface AppDelegate () <JConnectionDelegate, JMessageDelegate, JMessageSyncDelegate, JConversationSyncDelegate, JConversationDelegate, JMessageReadReceiptDelegate, JMessageUploadProvider>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JIM.shared setServer:@[@"http://120.48.178.248:8083"]];
    [JIM.shared setConsoleLogLevel:JLogLevelVerbose];
    [JIM.shared initWithAppKey:@"appkey"];
    [JIM.shared.connectionManager connectWithToken:kToken2];
    [JIM.shared.connectionManager addDelegate:self];
    [JIM.shared.messageManager addDelegate:self];
    [JIM.shared.messageManager addSyncDelegate:self];
    [JIM.shared.conversationManager addSyncDelegate:self];
    [JIM.shared.conversationManager addDelegate:self];
    [JIM.shared.messageManager addReadReceiptDelegate:self];
    [JIM.shared.messageManager setMessageUploadProvider:self];
    
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
            JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
            NSArray *a = [JIM.shared.messageManager getMessagesFrom:conversation count:100 time:0 direction:JPullDirectionOlder];
            int i = 1;
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
        
        //register push token
//        [JIM.shared.connectionManager registerToken:@"ppppushtoken"];
        
        
        //send merge message
//        NSArray *messageIdList = @[@"nqsu7glz26cgrenb", @"nqsu7g4t26egrenb", @"nqsu7klq26ggrenb", @"nqsu7k4lg6ggrenb"];
//        NSMutableArray *previewList = [NSMutableArray array];
//
//        for (int i = 0; i < 4; i++) {
//            JMergeMessagePreviewUnit *unit = [[JMergeMessagePreviewUnit alloc] init];
//            unit.previewContent = [NSString stringWithFormat:@"previewContent%d", i];
//            JUserInfo *userInfo = [[JUserInfo alloc] init];
//            userInfo.userId = [NSString stringWithFormat:@"userId%d", i];
//            userInfo.userName = [NSString stringWithFormat:@"name%d", i];
//            userInfo.portrait = @"portait";
//            unit.sender = userInfo;
//            [previewList addObject:unit];
//        }
//        JMergeMessage *merge = [[JMergeMessage alloc] initWithTitle:@"title"
//                                                      MessageIdList:messageIdList
//                                                        previewList:previewList];
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
//        JMessage *m = [JIM.shared.messageManager sendMessage:merge
//                                                   inConversation:c
//                                                          success:^(JMessage *message) {
//            NSLog(@"lifei");
//        } error:^(JErrorCode errorCode, JMessage *message) {
//            NSLog(@"lifei");
//        }];
//        NSLog(@"lifei");

        //query merge message

//
//        [JIM.shared.messageManager getMergedMessageList:@"nqsu8pvcg6lgrenb"
//                                                success:^(NSArray<JMessage *> *mergedMessages) {
//            NSLog(@"lifei");
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei");
//        }];
        
        
        //get message by id
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"196691553488470016"];
//        [JIM.shared.messageManager getMessagesByMessageIds:@[@"nqfszuuysfce4xum", @"nqfszu682fee4xum", @"nqfszzcksfgfg38b", @"nqfsz8hngfnfg38b"]
//                                            inConversation:c
//                                                   success:^(NSArray<JMessage *> *messages) {
//            NSLog(@"lifei");
//        } error:^(JErrorCode errorCode) {
//            NSLog(@"lifei");
//        }];
        
        
        //userInfo
//        JUserInfo *user2 = [JIM.shared.userInfoManager getUserInfo:@"userid2"];
//        JGroupInfo *group = [JIM.shared.userInfoManager getGroupInfo:@"groupid1"];
//        JUserInfo *user10 = [JIM.shared.userInfoManager getUserInfo:@"userid10"];
//        JGroupInfo *group11 = [JIM.shared.userInfoManager getGroupInfo:@"groupid11"];
//
//        NSLog(@"lifei");
        
        //conversation mute
//        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//        [JIM.shared.conversationManager setMute:NO
//                                   conversation:conversation
//                                        success:^{
//            NSLog(@"lifei, conversation mute set success");
//            [JIM.shared.conversationManager setDraft:@"draft1" inConversation:conversation];
//            JConversationInfo *c = [JIM.shared.conversationManager getConversationInfo:conversation];
//            NSLog(@"lifei");
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei, conversation mute set error, code is %d", code);
//        }];
        
        
        //get group read detail
//        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//        [JIM.shared.messageManager getGroupMessageReadDetail:@"nqel4yrhaa4k5g4v"
//                                              inConversation:conversation success:^(NSArray<JUserInfo *> *readMembers, NSArray<JUserInfo *> *unreadMembers) {
//            NSLog(@"lifei, getGroupMessageReadDetail success");
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei, getGroupMessageReadDetail error");
//        }];
        
        //getConversationList
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//            [JIM.shared.conversationManager clearUnreadCountByConversation:c];
//            NSArray *array = [JIM.shared.conversationManager getConversationInfoList];
//            NSLog(@"conversation count is %d", array.count);
//        });
        
        
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
        //draft
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//        [JIM.shared.conversationManager setDraft:@"draft" inConversation:c];
//
//        [JIM.shared.conversationManager clearDraftInConversation:c];
        
//        [JIM.shared.conversationManager deleteConversationInfoBy:conversation];
        
        //getMessages
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
////        NSArray *messages = [JIM.shared.messageManager getMessagesFrom:c
////                                                                 count:100
////                                                                  time:1712600893903
////                                                             direction:JPullDirectionNewer];
//
////        [JIM.shared.messageManager deleteMessageByMessageIds:@"nq2crdhwaagk5g4v"];
//        [JIM.shared.messageManager getLocalAndRemoteMessagesFrom:c
//                                                       startTime:0
//                                                           count:100
//                                                       direction:JPullDirectionOlder success:^(NSArray *messages) {
//            NSLog(@"lifei");
//
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei");
//
//        }];
//
//        NSLog(@"lifei");
        
        //read receipt
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//        NSArray *messageIds = @[@"nqe4ddt6abgk5g4v", @"nqe4dfu5sbgk5g4v"];
//        [JIM.shared.messageManager sendReadReceipt:messageIds
//                                    inConversation:c
//                                           success:^{
//            NSLog(@"lifei, sendReadReceipt success");
//        } error:^(JErrorCode code) {
//            NSLog(@"lifei, sendReadReceipt error, code is %d", code);
//        }];
////
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

        //delete messages
//        [JIM.shared.messageManager deleteMessageByMessageIds:@"nqel4yrhaa4k5g4v"];
//        [JIM.shared.messageManager deleteMessageByMessageIds:@"nqe4ddt6abgk5g4v"];
////        [JetIM.shared.messageManager deleteMessageByClientMsgNo:550];
//        NSArray *ids = @[@"nqel6ahx2a6k5g4v", @"nqel4yrhaa4k5g4v", @"nqe4dfu5sbgk5g4v", @"nqe4ddt6abgk5g4v"];
//        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup
//                                                            conversationId:@"groupid1"];
//        [JIM.shared.messageManager getMessagesByMessageIds:ids
//                                            inConversation:c
//                                                   success:^(NSArray<JMessage *> *messages) {
//            NSLog(@"lifei, getMessagesByMessageIds count is %d", messages.count);
//        } error:^(JErrorCode errorCode) {
//            NSLog(@"lifei, getMessagesByMessageIds error code is %d", errorCode);
//        }];
        
        //send message
//        [self sendMessage];
        
        //send mention message
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            JTextMessage *text = [[JTextMessage alloc] initWithContent:@"testMention"];
//            JMessageMentionInfo *mentionInfo = [[JMessageMentionInfo alloc] init];
//            mentionInfo.type = JMentionTypeSomeOne;
//            JUserInfo *user1 = [[JUserInfo alloc] init];
//            user1.userId = @"userid1";
//            JUserInfo *user2 = [[JUserInfo alloc] init];
//            user2.userId = @"userid2";
//            mentionInfo.targetUsers = @[user1, user2];
////            text.mentionInfo = mentionInfo;
//            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:@"groupid1"];
//            [JIM.shared.messageManager sendMessage:text inConversation:c
//                                           success:^(JMessage *message) {
//                NSLog(@"1111");
//                JConversationInfo *conversationInfo = [JIM.shared.conversationManager getConversationInfo:c];
//                JMessage *m = [JIM.shared.messageManager getMessagesByMessageIds:@[message.messageId]];
//                NSLog(@"1111");
//            } error:^(JErrorCode errorCode, JMessage *message) {
//                NSLog(@"222");
//            }];
//        });
        
        
        //save message
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            JTextMessage *text = [[JTextMessage alloc] initWithContent:@"save_message"];
//            JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid11"];
//            JMessage *m = [JIM.shared.messageManager saveMessage:text inConversation:c];
//            NSLog(@"lifei");
//            [JIM.shared.messageManager setMessageState:JMessageStateUploading withClientMsgNo:m.clientMsgNo];
//            m = [JIM.shared.messageManager getMessagesByClientMsgNos:@[@(m.clientMsgNo)]][0];
//            NSLog(@"lifei");
//            [JIM.shared.messageManager setMessageState:JMessageStateFail withClientMsgNo:m.clientMsgNo];
//            m = [JIM.shared.messageManager getMessagesByClientMsgNos:@[@(m.clientMsgNo)]][0];
//            NSLog(@"lifei");
//        });
        
        
        
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

- (void)dbDidClose {
    
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


