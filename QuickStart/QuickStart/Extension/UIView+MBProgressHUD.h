#import <UIKit/UIKit.h>
#import <MBProgressHUD/MBProgressHUD.h>

NS_ASSUME_NONNULL_BEGIN

@interface UIView (MBProgressHUD)

@property (weak, nonatomic) MBProgressHUD *HUD;
@property (nonatomic, strong) MBProgressHUD *loadingView;
- (void)showHUDMessage:(NSString *)message;
- (void)showLoading;
- (void)hideLoading;
@end

NS_ASSUME_NONNULL_END
