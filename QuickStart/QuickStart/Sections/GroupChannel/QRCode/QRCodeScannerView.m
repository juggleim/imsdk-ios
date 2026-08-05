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

#define Scanner_Width 206                                     /** Scanner width */
#define Scanner_X (self.frame.size.width - Scanner_Width) / 2 /** Scanner initial x position */
#define Scanner_Y 118                                         /** Scanner initial y position */

NSString *const ScannerLineAnmationKey = @"ScannerLineAnmationKey"; /** Scanner line animation key */
CGFloat const Scanner_BorderWidth = 1.0f;                           /** Scanner border width */
CGFloat const Scanner_CornerWidth = 3.0f;                           /** Scanner corner width */
CGFloat const Scanner_CornerLength = 20.0f;                         /** Scanner corner length */
CGFloat const Scanner_LineHeight = 2.0f;                            /** Scanner line height */

CGFloat const FlashlightBtn_Width = 20.0f;  /** Flashlight button width */
CGFloat const FlashlightLab_Height = 15.0f; /** Flashlight hint text height */
CGFloat const TipLab_Height = 50.0f;        /** Hint text height below the scanner */

static char FLASHLIGHT_ON; /** Flashlight state association key */

@interface QRCodeScannerView ()

@property (nonatomic, strong) UIImageView *scannerLine;                   /** Scanner line */
@property (nonatomic, strong) UIActivityIndicatorView *activityIndicator; /** Loading indicator */
@property (nonatomic, strong) UIButton *flashlightBtn;                    /** Flashlight button */
@property (nonatomic, strong) UILabel *tipLab;                            /** Hint text below the scanner */

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

/** Add the scanner line animation */
- (void)addScannerLineAnimation {

    // If the animation already exists, remove it first
    [self.scannerLine.layer removeAllAnimations];

    CABasicAnimation *lineAnimation = [CABasicAnimation animationWithKeyPath:@"transform"];
    lineAnimation.toValue =
        [NSValue valueWithCATransform3D:CATransform3DMakeTranslation(0, Scanner_Width - Scanner_LineHeight, 1)];
    lineAnimation.duration = 4;
    lineAnimation.repeatCount = HUGE;
    [self.scannerLine.layer addAnimation:lineAnimation forKey:ScannerLineAnmationKey];
    // Reset the animation speed to 1.0
    self.scannerLine.layer.speed = 1.0;
}

/** Pause the scanner animation */
- (void)pauseScannerLineAnimation {
    // Convert the current time to the paused animation time
    CFTimeInterval pauseTime = [self.scannerLine.layer convertTime:CACurrentMediaTime() fromLayer:nil];
    // Set the animation time offset so the animation freezes at that point
    self.scannerLine.layer.timeOffset = pauseTime;
    // Set the animation speed to 0; the default speed is 1.0
    self.scannerLine.layer.speed = 0;
}

/** Show the flashlight */
- (void)showFlashlightWithAnimated:(BOOL)animated {
    self.flashlightBtn.selected = YES;
}

/** Hide the flashlight */
- (void)hideFlashlightWithAnimated:(BOOL)animated {
    self.flashlightBtn.selected = NO;
}

/** Add the indicator */
- (void)addActivityIndicator {
    if (!self.activityIndicator) {
        self.activityIndicator =
        [[UIActivityIndicatorView alloc] initWithActivityIndicatorStyle:UIActivityIndicatorViewStyleLarge];
        self.activityIndicator.center = self.center;
        [self addSubview:self.activityIndicator];
    }
    [self.activityIndicator startAnimating];
}

/** Remove the indicator */
- (void)removeActivityIndicator {
    if (self.activityIndicator) {
        [self.activityIndicator removeFromSuperview];
        self.activityIndicator = nil;
    }
}

/** Set the flashlight switch */
- (void)setFlashlightOn:(BOOL)on {
    [self lightOn:on];
    self.flashlightBtn.selected = on;
    objc_setAssociatedObject(self, &FLASHLIGHT_ON, @(on), OBJC_ASSOCIATION_RETAIN_NONATOMIC);
}

/** Get the current flashlight state */
- (BOOL)flashlightOn {
    return [objc_getAssociatedObject(self, &FLASHLIGHT_ON) boolValue];
}

- (void)drawRect:(CGRect)rect {
    [super drawRect:rect];
    // Semi-transparent area
    [[UIColor colorWithWhite:0 alpha:0.7] setFill];
    UIRectFill(rect);

    // Transparent area
    CGRect scanner_rect = CGRectMake(Scanner_X, Scanner_Y, Scanner_Width, Scanner_Width);
    [[UIColor clearColor] setFill];
    UIRectFill(scanner_rect);

    // Border
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
        // Top-left corner
        case 0: {
            [tempPath moveToPoint:CGPointMake(Scanner_X + Scanner_CornerLength, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_CornerLength)];
        } break;
        // Top-right corner
        case 1: {
            [tempPath moveToPoint:CGPointMake(Scanner_X + Scanner_Width - Scanner_CornerLength, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width, Scanner_Y)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_Width, Scanner_Y + Scanner_CornerLength)];
        } break;
        // Bottom-left corner
        case 2: {
            [tempPath moveToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_Width - Scanner_CornerLength)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X, Scanner_Y + Scanner_Width)];
            [tempPath addLineToPoint:CGPointMake(Scanner_X + Scanner_CornerLength, Scanner_Y + Scanner_Width)];
        } break;
        // Bottom-right corner
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
