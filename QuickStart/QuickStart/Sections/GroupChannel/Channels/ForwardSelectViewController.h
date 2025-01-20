//
//  ForwardSelectViewController.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface ForwardSelectViewController : UIViewController

- (instancetype)initWithMessageContent:(JMessageContent *)content;

@end

NS_ASSUME_NONNULL_END
