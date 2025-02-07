//
//  QRCodeScannerView.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/20.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface QRCodeScannerView : UIView

- (void)addScannerLineAnimation;
- (void)pauseScannerLineAnimation;
- (void)addActivityIndicator;
- (void)removeActivityIndicator;

- (CGFloat)scanner_x;
- (CGFloat)scanner_y;
- (CGFloat)scanner_width;

- (void)showFlashlightWithAnimated:(BOOL)animated;
- (void)hideFlashlightWithAnimated:(BOOL)animated;
- (void)setFlashlightOn:(BOOL)on;
- (BOOL)flashlightOn;

@end

NS_ASSUME_NONNULL_END
