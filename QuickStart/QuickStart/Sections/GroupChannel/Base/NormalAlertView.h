#import <UIKit/UIKit.h>

typedef void (^ButtonBlock)(void);

@interface NormalAlertView : UIView

+ (void)showAlertWithTitle:(NSString *)title
                   message:(NSString *)message
             describeTitle:(NSString *)describeTitle
              confirmTitle:(NSString *)confirmTitle
                   confirm:(ButtonBlock)confirm;

+ (void)showAlertWithMessage:(NSString *)Message
               highlightText:(NSString *)highlightText
                   leftTitle:(NSString *)leftTitle
                  rightTitle:(NSString *)rightTitle
                      cancel:(ButtonBlock)cancel
                     confirm:(ButtonBlock)confirm;

+ (void)showAlertWithTitle:(NSString *)title
                   message:(NSString *)message
             highlightText:(NSString *)highlightText
             describeTitle:(NSString *)describeTitle
                 leftTitle:(NSString *)leftTitle
                rightTitle:(NSString *)rightTitle
                    cancel:(ButtonBlock)cancel
                   confirm:(ButtonBlock)confirm;

@end
