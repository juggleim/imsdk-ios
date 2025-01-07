//
//  ScanQRCodeViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/20.
//

#import "ScanQRCodeViewController.h"
#import "QRCodeScannerView.h"
#import <AVFoundation/AVFoundation.h>
#import "QuickStart-Swift.h"
#import "PersonDetailViewController.h"
#import "GroupJoinViewController.h"

@interface ScanQRCodeViewController () <AVCaptureMetadataOutputObjectsDelegate>
@property (nonatomic, strong) QRCodeScannerView *scannerView;
@property (nonatomic, strong) AVCaptureSession *session;
@end

@implementation ScanQRCodeViewController
#pragma mark - life cycle
- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = [UIColor blackColor];
    [self.view addSubview:self.scannerView];
    [self setNavi];
    [self checkCameraAuthorizationStatus];

    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appDidBecomeActive:)
                                                 name:UIApplicationDidBecomeActiveNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(appWillResignActive:)
                                                 name:UIApplicationWillResignActiveNotification
                                               object:nil];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    [self resumeScanning];
}

- (void)viewWillDisappear:(BOOL)animated {
    [super viewWillDisappear:animated];
    [self.scannerView setFlashlightOn:NO];
    [self.scannerView hideFlashlightWithAnimated:YES];
}

- (void)dealloc {
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark-- AVCaptureMetadataOutputObjectsDelegate
- (void)captureOutput:(AVCaptureOutput *)output
    didOutputMetadataObjects:(NSArray<__kindof AVMetadataObject *> *)metadataObjects
              fromConnection:(AVCaptureConnection *)connection {
    // 获取扫一扫结果
    if (metadataObjects && metadataObjects.count > 0) {
        [self pauseScanning];
        AVMetadataMachineReadableCodeObject *metadataObject = metadataObjects[0];
        NSString *stringValue = metadataObject.stringValue;
        [self handleWithValue:stringValue];
    } else {
        [self showErrorAlertView];
    }
}

#pragma mark - private
- (void)showErrorAlertView {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIAlertController *alertController =
            [UIAlertController alertControllerWithTitle:nil message:@"无法识别的二维码" preferredStyle:UIAlertControllerStyleAlert];
        [alertController
            addAction:[UIAlertAction actionWithTitle:@"确认"
                                               style:UIAlertActionStyleDestructive
                                             handler:^(UIAlertAction *_Nonnull action){
            [self.navigationController popViewControllerAnimated:true];
                                             }]];
        [self presentViewController:alertController animated:true completion:nil];
    });
}

- (void)setNavi {
    self.navigationItem.title = @"Scan";
}

- (void)checkCameraAuthorizationStatus {
    [self checkCameraAuthorizationStatusWithGrand:^(BOOL granted) {
        if (granted) {
            dispatch_async(dispatch_get_main_queue(), ^{
                [self setupScanner];
            });
        }
    }];
}

/** 校验是否有相机权限 */
- (void)checkCameraAuthorizationStatusWithGrand:(void (^)(BOOL granted))permissionGranted {
    AVAuthorizationStatus videoAuthStatus = [AVCaptureDevice authorizationStatusForMediaType:AVMediaTypeVideo];

    switch (videoAuthStatus) {
    // 已授权
    case AVAuthorizationStatusAuthorized: {
        permissionGranted(YES);
    } break;
    // 未询问用户是否授权
    case AVAuthorizationStatusNotDetermined: {
        // 提示用户授权
        [AVCaptureDevice requestAccessForMediaType:AVMediaTypeVideo
                                 completionHandler:^(BOOL granted) {
                                     permissionGranted(granted);
                                 }];
    } break;
    // 用户拒绝授权或权限受限
    case AVAuthorizationStatusRestricted:
    case AVAuthorizationStatusDenied: {
        [self showAlertController:@"您没有照片访问权限，请前往“设置-隐私-照片”选项中，允许访问您的手机照片"
                      cancelTitle:@"确认"];
        permissionGranted(NO);
    } break;
    default:
        break;
    }
}

- (void)showAlertController:(NSString *)title cancelTitle:(NSString *)cancelTitle {
    dispatch_async(dispatch_get_main_queue(), ^{
        UIViewController *rootVC = [UIApplication sharedApplication].delegate.window.rootViewController;
        UIAlertController *alertController =
            [UIAlertController alertControllerWithTitle:title message:nil preferredStyle:UIAlertControllerStyleAlert];
        [alertController
            addAction:[UIAlertAction actionWithTitle:cancelTitle style:UIAlertActionStyleDefault handler:nil]];
        [rootVC presentViewController:alertController animated:YES completion:nil];
    });
}

- (void)setupScanner {
    AVCaptureDevice *device = [AVCaptureDevice defaultDeviceWithMediaType:AVMediaTypeVideo];
    AVCaptureDeviceInput *deviceInput = [AVCaptureDeviceInput deviceInputWithDevice:device error:nil];

    AVCaptureMetadataOutput *metadataOutput = [[AVCaptureMetadataOutput alloc] init];
    [metadataOutput setMetadataObjectsDelegate:self queue:dispatch_get_main_queue()];
    metadataOutput.rectOfInterest = CGRectMake([self.scannerView scanner_y] / self.view.frame.size.height,
                                               [self.scannerView scanner_x] / self.view.frame.size.width,
                                               [self.scannerView scanner_width] / self.view.frame.size.height,
                                               [self.scannerView scanner_width] / self.view.frame.size.width);

    AVCaptureVideoDataOutput *videoDataOutput = [[AVCaptureVideoDataOutput alloc] init];

    self.session = [[AVCaptureSession alloc] init];
    [self.session setSessionPreset:AVCaptureSessionPresetHigh];
    if ([self.session canAddInput:deviceInput]) {
        [self.session addInput:deviceInput];
    }
    if ([self.session canAddOutput:metadataOutput]) {
        [self.session addOutput:metadataOutput];
    }
    if ([self.session canAddOutput:videoDataOutput]) {
        [self.session addOutput:videoDataOutput];
    }
#if TARGET_IPHONE_SIMULATOR
// 模拟器设置不了，会crash
#else
    if ([metadataOutput.availableMetadataObjectTypes containsObject:AVMetadataObjectTypeQRCode]) {
        metadataOutput.metadataObjectTypes = @[ AVMetadataObjectTypeQRCode ];
    }
#endif

    AVCaptureVideoPreviewLayer *videoPreviewLayer = [AVCaptureVideoPreviewLayer layerWithSession:_session];
    videoPreviewLayer.videoGravity = AVLayerVideoGravityResizeAspectFill;
    videoPreviewLayer.frame = self.view.layer.bounds;
    [self.view.layer insertSublayer:videoPreviewLayer atIndex:0];

    [self.session startRunning];
}

- (void)appDidBecomeActive:(NSNotification *)notify {
    [self resumeScanning];
}

- (void)appWillResignActive:(NSNotification *)notify {
    [self pauseScanning];
    [self.scannerView hideFlashlightWithAnimated:YES];
}

- (void)resumeScanning {
    if (self.session) {
        [self.session startRunning];
        [self.scannerView addScannerLineAnimation];
    }
}

- (void)pauseScanning {
    if (self.session) {
        [self.session stopRunning];
        [self.scannerView pauseScannerLineAnimation];
    }
}

- (void)handleWithValue:(NSString *)value {
    NSData *data = [value dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    NSString *action = json[@"action"];
    if ([action isEqualToString:@"login"]) {
        NSString *code = json[@"code"];
        [HttpManager.shared qrcodeConfirmWithQrcodeString:code completion:^(NSInteger code) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (code == 0) {
                    [self.navigationController popViewControllerAnimated:YES];
                }
            });
        }];
    } else if ([action isEqualToString:@"join_group"]) {
        NSString *groupId = json[@"group_id"];
//        NSString *userId = json[@"user_id"];
        
        [HttpManager.shared getGroupInfoWithGroupId:groupId completion:^(NSInteger code, JCGroupInfo * _Nullable group) {
            dispatch_async(dispatch_get_main_queue(), ^{
                if (code == 0) {
                    if (group.myRole == GroupRoleNotMember) {
                        GroupJoinViewController *vc = [[GroupJoinViewController alloc] init];
                        vc.group = group;
                        [self.navigationController pushViewController:vc animated:YES];
                    } else {
                        JConversation *conv = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:group.groupId];
                        JConversationInfo *info = [JIM.shared.conversationManager getConversationInfo:conv];
                        ChannelViewController *vc = [[ChannelViewController alloc] initWithConversationInfo:info];
                        [self.navigationController pushViewController:vc animated:YES];
                    }
                }
            });
        }];
        
    } else if ([action isEqualToString:@"add_friend"]) {
        NSString *userId = json[@"user_id"];
        PersonDetailViewController *vc = [[PersonDetailViewController alloc] init];
        vc.userId = userId;
        [self.navigationController pushViewController:vc animated:YES];
    } else {
        [self showErrorAlertView];
    }

}

#pragma mark - getter & setter
- (QRCodeScannerView *)scannerView {
    if (!_scannerView) {
        _scannerView = [[QRCodeScannerView alloc] initWithFrame:self.view.bounds];
//        _scannerView.delegate = self;
    }
    return _scannerView;
}

@end
