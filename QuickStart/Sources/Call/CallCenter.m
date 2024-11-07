//
//  CallCenter.m
//  QuickStart
//
//  Created by Fei Li on 2024/11/4.
//

#import "CallCenter.h"
#import "SingleCallViewController.h"

@interface CallCenter () <JCallReceiveDelegate>
@property(nonatomic, strong) NSMutableArray *callWindows;
@end

@implementation CallCenter

static CallCenter *_instance;

+ (instancetype)shared {
    static dispatch_once_t onceToken;
    dispatch_once(&onceToken, ^{
        _instance = [[self alloc] init];
        _instance.callWindows = [NSMutableArray array];
        [JIM.shared.callManager addReceiveDelegate:_instance];
    });
    return _instance;
}

- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign {
    [JIM.shared.callManager initZegoEngineWith:appId
                                       appSign:appSign];
}

- (void)startSingleCall:(id<JCallSession>)callSession {
    SingleCallViewController *vc = [[SingleCallViewController alloc] initWithOutgoingCall:callSession];
    [self presentCallViewController:vc];
}

- (void)dismissCallViewController:(UIViewController *)vc {
    for (UIWindow *window in self.callWindows) {
        if (window.rootViewController == vc) {
            [window resignKeyWindow];
            window.hidden = YES;
            [self.callWindows removeObject:window];
            break;
        }
    }
    [vc dismissViewControllerAnimated:YES completion:nil];
}


- (void)presentCallViewController:(UIViewController *)viewController {
    //后台回前台的瞬间 activationState 是 UISceneActivationStateForegroundInactive
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        UIWindowScene *scene = nil;
        for (UIWindowScene *s in UIApplication.sharedApplication.connectedScenes) {
            if (s.activationState == UISceneActivationStateForegroundActive) {
                scene = s;
                break;
            }
        }
        if (!scene) {
            return;
        }
        
        UIWindow *activityWindow = [[UIWindow alloc] initWithWindowScene:scene];
        activityWindow.frame = [[UIScreen mainScreen] bounds];
        activityWindow.windowLevel = UIWindowLevelAlert + 1;
        activityWindow.rootViewController = viewController;
        [activityWindow makeKeyAndVisible];
        CATransition *animation = [CATransition animation];
        [animation setDuration:0.3];
        animation.type = kCATransitionMoveIn;     //可更改为其他方式
        animation.subtype = kCATransitionFromTop; //可更改为其他方式
        [[activityWindow layer] addAnimation:animation forKey:nil];
        [self.callWindows addObject:activityWindow];
    });
}

#pragma mark - JCallReceiveDelegate
- (void)callDidReceive:(id<JCallSession>)callSession {
    SingleCallViewController *vc = [[SingleCallViewController alloc] initWithIncomingCall:callSession];
    [self presentCallViewController:vc];
}

@end
