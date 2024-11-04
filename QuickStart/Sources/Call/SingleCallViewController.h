//
//  SingleCallViewController.h
//  QuickStart
//
//  Created by Fei Li on 2024/11/1.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>

@interface SingleCallViewController : UIViewController

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession;

@end

