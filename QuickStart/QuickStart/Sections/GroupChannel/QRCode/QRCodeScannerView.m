//
//  QRCodeScannerView.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/20.
//

#import "QRCodeScannerView.h"
#import "objc/runtime.h"
#import <AVFoundation/AVFoundation.h>
#import "CallTheme.h"

#define Scanner_Width 206                                     /** 扫描器宽度 */
#define Scanner_X (self.frame.size.width - Scanner_Width) / 2 /** 扫描器初始x值 */
#define Scanner_Y 118                                         /** 扫描器初始y值 */

NSString *const ScannerLineAnmationKey = @"ScannerLineAnmationKey"; /** 扫描线条动画Key值 */
CGFloat const Scanner_BorderWidth = 1.0f;                           /** 扫描器边框宽度 */
CGFloat const Scanner_CornerWidth = 3.0f;                           /** 扫描器棱角宽度 */
CGFloat const Scanner_CornerLength = 20.0f;                         /** 扫描器棱角长度 */
CGFloat const Scanner_LineHeight = 2.0f;                            /** 扫描器线条高度 */

CGFloat const FlashlightBtn_Width = 20.0f;  /** 手电筒按钮宽度 */
CGFloat const FlashlightLab_Height = 15.0f; /** 手电筒提示文字高度 */
CGFloat const TipLab_Height = 50.0f;        /** 扫描器下方提示文字高度 */

static char FLASHLIGHT_ON; /** 手电筒开关状态绑定标识符 */

@interface QRCodeScannerView ()

@property (nonatomic, strong) UIImageView *scannerLine;                   /** 扫描线条 */
@property (nonatomic, strong) UIActivityIndicatorView *activityIndicator; /** 加载指示器 */
@property (nonatomic, strong) UIButton *flashlightBtn;                    /** 手电筒按钮 */
@property (nonatomic, strong) UILabel *tipLab;                            /** 扫描器下方提示文字 */

@end

@implementation QRCodeScannerView

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self setupUI];
    }
    return self;
}

- (void)setupUI {
    self.backgroundColor = [UIColor clearColor];
    [self addSubview:self.scannerLine];
    [self addScannerLineAnimation];
    [self addSubview:self.flashlightBtn];
    [self addSubview:self.tipLab];
    
    self.flashlightBtn.frame = CGRectMake(self.frame.origin.x, self.frame.origin.y+Scanner_Y + Scanner_Width+14, self.frame.size.width, 24);
    self.tipLab.frame = CGRectMake(self.frame.origin.x, self.flashlightBtn.frame.origin.y+self.flashlightBtn.frame.size.height+15, self.frame.size.width, 20);
}

- (void)flashlightClicked:(UIButton *)button {
    button.selected = !button.selected;
    [self setFlashlightOn:self.flashlightBtn.selected];
}

/** 添加扫描线条动画 */
- (void)addScannerLineAnimation {

    // 若已添加动画，则先移除动画再添加
    [self.scannerLine.layer removeAllAnimations];

    CABasicAnimation *lineAnimation = [CABasicAnimation animationWithKeyPath:@"transform"];
    lineAnimation.toValue =
        [NSValue valueWithCATransform3D:CATransform3DMakeTranslation(0, Scanner_Width - Scanner_LineHeight, 1)];
    lineAnimation.duration = 4;
    lineAnimation.repeatCount = HUGE;
    [self.scannerLine.layer addAnimation:lineAnimation forKey:ScannerLineAnmationKey];
    // 重置动画运行速度为1.0
    self.scannerLine.layer.speed = 1.0;
}

/** 暂停扫描器动画 */
- (void)pauseScannerLineAnimation {
    // 取出当前时间，转成动画暂停的时间
    CFTimeInterval pauseTime = [self.scannerLine.layer convertTime:CACurrentMediaTime() fromLayer:nil];
    // 设置动画的时间偏移量，指定时间偏移量的目的是让动画定格在该时间点的位置
    self.scannerLine.layer.timeOffset = pauseTime;
    // 将动画的运行速度设置为0， 默认的运行速度是1.0
    self.scannerLine.layer.speed = 0;
}

/** 显示手电筒 */
- (void)showFlashlightWithAnimated:(BOOL)animated {
    self.flashlightBtn.selected = YES;
}

/** 隐藏手电筒 */
- (void)hideFlashlightWithAnimated:(BOOL)animated {
    self.flashlightBtn.selected = NO;
}

/** 添加指示器 */
- (void)addActivityIndicator {
    if (!self.activityIndicator) {
        self.activityIndicator =
        [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleLarge];
        self.activityIndicator.center = self.center;
        [self addSubview:self.activityIndicator];
    }
    [self.activityIndicator startAnimating];
}

/** 移除指示器 */
- (void)removeActivityIndicator {
    if (self.activityIndicator) {
        [self.activityIndicator removeFromSuperview];
        self.activityIndicator = nil;
    }
}

/** 设置手电筒开关 */
- (void)setFlashlightOn:(BOOL)on {
    [self lightOn:on];
    self.flashlightBtn.selected = on;
    objc_setAssociatedObject(self, &FLASHLIGHT_ON, @(on), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

/** 获取手电筒当前开关状态 */
- (BOOL)flashlightOn {
    return [objc_getAssociatedObject(self, &FLASHLIGHT_ON) boolValue];
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    // 半透明区域
    [[UIColor colorWithWhite:0 alpha:0.7] setFill];
    UIRectFill(rect);

    // 透明区域
    CGRect scanner_rect = CGRectMake(Scanner_X, Scanner_Y, Scanner_Width, Scanner_Width);
    [[UIColor clearColor] setFill];
    UIRectFill(scanner_rect);

    // 边框
    UIBezierPath *borderPath =
        [UIBezierPath bezierPathWithRect:CGRectMake(Scanner_X, Scanner_Y, Scanner_Width, Scanner_Width)];
    borderPath.lineCapStyle = kCGLineCapRound;
    borderPath.lineWidth = Scanner_BorderWidth;
    [[UIColor clearColor] set];
    [borderPath stroke];

    for (int index = 0; index < 4; ++index) {

        UIBezierPath *tempPath = [UIBezierPath bezierPath];
        tempPath.lineWidth = Scanner_CornerWidth;
        [ColorFromRGB(0x0099ff) set];

        switch (index) {
        // 左上角棱角
        case 0: {
            [tempPath moveToPoint:CGPointMake(Scanner_X + Scanner_CornerLength, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_CornerLength)];
        } break;
        // 右上角
        case 1: {
            [tempPath moveToPoint:CGPointMake(Scanner_X + Scanner_Width - Scanner_CornerLength, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width, Scanner_Y + Scanner_CornerLength)];
        } break;
        // 左下角
        case 2: {
            [tempPath moveToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_Width - Scanner_CornerLength)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_Width)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_CornerLength, Scanner_Y + Scanner_Width)];
        } break;
        // 右下角
        case 3: {
            [tempPath
                moveToPoint:CGPointMake(Scanner_X + Scanner_Width - Scanner_CornerLength, Scanner_Y + Scanner_Width)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width, Scanner_Y + Scanner_Width)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width,
                                                 Scanner_Y + Scanner_Width - Scanner_CornerLength)];
        } break;
        default:
            break;
        }
        [tempPath stroke];
    }
}

- (void)lightOn:(BOOL)on {
    AVCaptureDevice *captureDevice = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    if ([captureDevice hasTorch] && [captureDevice hasFlash]) {
        [captureDevice lockForConfiguration:nil];
        if (on) {
            [captureDevice setTorchMode:AVCaptureTorchModeOn];
            [captureDevice setFlashMode:AVCaptureFlashModeOn];
        } else {
            [captureDevice setTorchMode:AVCaptureTorchModeOff];
            [captureDevice setFlashMode:AVCaptureFlashModeOff];
        }
        [captureDevice unlockForConfiguration];
    }
}

- (CGFloat)scanner_x {
    return Scanner_X;
}

- (CGFloat)scanner_y {
    return Scanner_Y;
}

- (CGFloat)scanner_width {
    return Scanner_Width;
}
@end
