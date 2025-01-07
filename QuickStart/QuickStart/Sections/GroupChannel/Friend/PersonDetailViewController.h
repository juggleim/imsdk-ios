//
//  PersonDetailViewController.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/7.
//

#import <UIKit/UIKit.h>
#import "QuickStart-Swift.h"

NS_ASSUME_NONNULL_BEGIN

@interface PersonDetailViewController : UIViewController
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, strong) JCUser *user;
@end

NS_ASSUME_NONNULL_END
