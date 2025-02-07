#import "UIView+MBProgressHUD.h"
#import <objc/runtime.h>
#import "OCConst.h"

#define MBPHUD_EXECUTE(...)                                                                                            \
    __weak typeof(self) weakself = self;                                                                               \
    [self hideHUDCompletion:^{                                                                                         \
        [weakself.HUD removeFromSuperview];                                                                            \
        __VA_ARGS__                                                                                                    \
    }];

CGFloat const MBPHUDFontSize = 12;
CGFloat const MBPHUDShowTime = 2.0f;

@implementation UIView (MBProgressHUD)

@dynamic HUD;

- (MBProgressHUD *)loadingView {
    return objc_getAssociatedObject(self, @selector(loadingView));
}

- (void)setLoadingView:(MBProgressHUD *)loadingView {
    objc_setAssociatedObject(self, @selector(loadingView), loadingView, OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

- (MBProgressHUD *)HUD {
    return [MBProgressHUD HUDForView:self];
}

- (MBProgressHUD *)instanceHUD {
    MBProgressHUD *HUD = [[MBProgressHUD alloc] initWithView:self];
    return HUD;
}

- (void)showHUDMessage:(NSString *)message {
    MBPHUD_EXECUTE({
        MBProgressHUD *HUD = [weakself instanceHUD];
        HUD.bezelView.backgroundColor = [ColorFromRGB(0x000000) colorWithAlphaComponent:0.4];
        [[UIApplication sharedApplication].keyWindow addSubview:HUD];
        [[UIApplication sharedApplication].keyWindow bringSubviewToFront:HUD];
        HUD.mode = MBProgressHUDModeText;
        HUD.label.text = message;
        HUD.label.textColor = ColorFromRGB(0xffffff);
        HUD.removeFromSuperViewOnHide = YES;
        [HUD showAnimated:YES];
        HUD.userInteractionEnabled = NO;
        [HUD hideAnimated:YES afterDelay:MBPHUDShowTime];
    })
}

- (void)hideHUDCompletion:(nullable void (^)(void))completion {
    if (!self.HUD) {
        if (completion)
            completion();
        return;
    }
    self.HUD.completionBlock = completion;
    [self.HUD hideAnimated:YES];
}

- (void)showLoading {
    if (self.loadingView) {
        return;
    }
   
    dispatch_async(dispatch_get_main_queue(), ^{
        MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self animated:YES];
        hud.mode = MBProgressHUDModeAnnularDeterminate;
        hud.label.text = @"Loading...";
        hud.removeFromSuperViewOnHide = YES;
        self.loadingView = hud;
    });
}

- (void)hideLoading {
    dispatch_async(dispatch_get_main_queue(), ^{
        if (self.loadingView) {
            [self.loadingView hideAnimated:YES];
            self.loadingView = nil;
        }
    });
}
@end
