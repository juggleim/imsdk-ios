//
//  AppDelegate.m
//  JuggleDemo
//
//  Created by Nathan on 2023/11/27.
//

#import "AppDelegate.h"
#import <Juggle/Juggle.h>

@interface AppDelegate () <JConnectionDelegate>

@end

@implementation AppDelegate


- (BOOL)application:(UIApplication *)application didFinishLaunchingWithOptions:(NSDictionary *)launchOptions {
    // Override point for customization after application launch.
    [JuggleIM.shared initWithAppKey:@"appkey"];
    [JuggleIM.shared.connectionManager connectWithToken:@"CgZhcHBrZXkaIDAr072n8uOcw5YBeKCcQ+QCw4m6YWhgt99U787/dEJS"];
    [JuggleIM.shared.connectionManager setDelegate:self];
    
    return YES;
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


