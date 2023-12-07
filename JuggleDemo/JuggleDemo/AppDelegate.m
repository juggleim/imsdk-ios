//
//  AppDelegate.m
//  JuggleDemo
//
//  Created by Nathan on 2023/11/27.
//

#import "AppDelegate.h"
#import <Juggle/Juggle.h>
#import <Juggle/JTextMessage.h>

#define kToken1 @"CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"
#define kToken2 @"CgZhcHBrZXkaINodQgLnbhTbt0SzC8b/JFwjgUAdIfUZTEFK8DvDLgM1"
#define kToken3 @"CgZhcHBrZXkaINMDzs7BBTTZTwjKtM10zyxL4DBWFuZL6Z/OAU0Iajpv"
#define kToken4 @"CgZhcHBrZXkaIDHZwzfny4j4GiJye8y8ehU5fpJ+wVOGI3dCsBMfyLQv"
#define kToken5 @"CgZhcHBrZXkaIOx2upLCsmsefp8U/KNb52UGnAEu/xf+im3QaUd0HTC2"

@interface AppDelegate () <JConnectionDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JuggleIM.shared initWithAppKey:@"appkey"];
    [JuggleIM.shared.connectionManager connectWithToken:kToken5];
    [JuggleIM.shared.connectionManager setDelegate:self];
    
    return YES;
}

- (void)connectionStatusDidChange:(JConnectionStatus)status errorCode:(JErrorCode)code {
    if (JConnectionStatusConnected == status) {
        //send message
//        JTextMessage *text = [[JTextMessage alloc] initWithContent:@"text1"];
//        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
//        [JuggleIM.shared.messageManager sendMessage:text inConversation:conversation];
        
        //disconnect
//        [JuggleIM.shared.connectionManager disconnect:NO];
        
        //get remote msgs
        JConversation *c = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:@"userid1"];
        [JuggleIM.shared.messageManager getRemoteMessagesFrom:c
                                                    startTime:[[NSDate date] timeIntervalSince1970]*1000
                                                        count:20
                                                    direction:JPullDirectionNewToOld
                                                      success:nil
                                                        error:nil];
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


