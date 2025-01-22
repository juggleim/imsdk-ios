//
//  ForwardSelectViewController.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ForwardSelectViewControllerDelegate <NSObject>

- (void)messageWillForward:(JMessage *)message;
- (void)messageDidForward:(JMessage *)message;
- (void)messageDidForwardFail:(JMessage *)message
                    errorCode:(JErrorCode)code;

@end

@interface ForwardSelectViewController : UIViewController

- (instancetype)initWithMessageContent:(JMessageContent *)content;

@property (nonatomic, weak) id<ForwardSelectViewControllerDelegate> delegate;

@end

NS_ASSUME_NONNULL_END
