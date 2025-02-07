//
//  ForwardAlertView.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@protocol ForwardAlertViewDelegate;

@interface ForwardAlertView : UIView

+ (instancetype)alertViewWithConversation:(JConversation *)conversation
                                  content:(JMessageContent *)content;

- (void)show;

@property (nonatomic, weak) id<ForwardAlertViewDelegate> delegate;

@end

@protocol ForwardAlertViewDelegate <NSObject>

/**
 点击某个按钮

 @param alertView alertView
 @param buttonIndex 0代表取消，1代表确定
 */
- (void)forwardAlertView:(ForwardAlertView *)alertView
            conversation:(JConversation *)conversation
    clickedButtonAtIndex:(NSInteger)buttonIndex;

@end

NS_ASSUME_NONNULL_END
