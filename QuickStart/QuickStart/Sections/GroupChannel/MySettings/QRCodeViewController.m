//
//  QRCodeViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/6.
//

#import "QRCodeViewController.h"
#import <SDWebImage/UIImageView+WebCache.h>
#import "OCConst.h"
#import <Masonry/Masonry.h>
#import "QuickStart-Swift.h"
#import "UIView+MBProgressHUD.h"
#import "ForwardSelectViewController.h"
#import "PortraitUtil.h"

@interface QRCodeViewController ()
@property (nonatomic, strong) UIView *qrBgView;
@property (nonatomic, strong) UIImageView *portraitImageView;
@property (nonatomic, strong) UILabel *nameLabel;
@property (nonatomic, strong) UILabel *countLabel;
@property (nonatomic, strong) UIImageView *qrCodeImageView;
@property (nonatomic, strong) UILabel *infoLabel;

@property (nonatomic, strong) UIView *shareBgView;
@property (nonatomic, strong) UIButton *saveButton;
@property (nonatomic, strong) UIButton *shareBtn;

@property (nonatomic, strong) JConversation *conversation;
@end

@implementation QRCodeViewController

#pragma mark - life cycle
- (instancetype)initWithConversation:(JConversation *)conversation {
    if (self = [super init]) {
        self.conversation = conversation;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    [self configData];
    [self setNaviItem];
    [self addSubViews];
}

#pragma mark - helper
- (void)configData {
    if (self.conversation.conversationType == JConversationTypeGroup) {
        self.countLabel.text = [NSString stringWithFormat:@"%ld 人", self.group.memberCount];
        if (self.group.portrait.length == 0) {
            self.portraitImageView.image = [PortraitUtil defaultPortraitImageWith:self.group.groupId name:self.group.groupName type:JConversationTypeGroup];
        } else {
            [self.portraitImageView sd_setImageWithURL:[NSURL URLWithString:self.group.portrait] placeholderImage:[UIImage imageNamed:@"iconUser"]];
        }
        self.nameLabel.text = self.group.groupName;
        self.infoLabel.text = @"扫一扫群二维码，立刻加入该群";
        [HttpManager.shared getGroupQRCodeWithGroupId:self.group.groupId
                                           completion:^(NSInteger code, NSString * _Nullable base64) {
            if (code != 0) {
                return;
            }
            NSData *d = [[NSData alloc] initWithBase64EncodedString:base64 options:NSDataBase64DecodingIgnoreUnknownCharacters];
            if (d) {
                UIImage *image = [UIImage imageWithData:d];
                dispatch_async(dispatch_get_main_queue(), ^{
                    self.qrCodeImageView.image = image;
                    self.qrCodeImageView.contentMode = UIViewContentModeScaleAspectFit;
                });
            }
        }];
    } else if (self.conversation.conversationType == JConversationTypePrivate) {
        JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:self.conversation.conversationId];
        if (userInfo.portrait.length == 0) {
            self.portraitImageView.image = [PortraitUtil defaultPortraitImageWith:self.conversation.conversationId name:userInfo.userName type:JConversationTypePrivate];
        } else {
            [self.portraitImageView sd_setImageWithURL:[NSURL URLWithString:userInfo.portrait] placeholderImage:[UIImage imageNamed:@"iconUser"]];
        }
        self.nameLabel.text = userInfo.userName;
        self.infoLabel.text = @"扫一扫二维码，加我为好友";
        [HttpManager.shared getUserQRCodeWithCompletion:^(NSInteger code, NSString * _Nullable base64) {
            if (code != 0) {
                return;
            }
            NSData *d = [[NSData alloc] initWithBase64EncodedString:base64 options:NSDataBase64DecodingIgnoreUnknownCharacters];
            if (d) {
                UIImage *image = [UIImage imageWithData:d];
                dispatch_async(dispatch_get_main_queue(), ^{
                    self.qrCodeImageView.image = image;
                    self.qrCodeImageView.contentMode = UIViewContentModeScaleAspectFit;
                });
            }
        }];
    }
}

- (void)setNaviItem {
    if (self.conversation.conversationType == JConversationTypeGroup) {
        self.navigationItem.title = @"群二维码";
    } else {
        self.navigationItem.title = @"我的二维码";
    }
    UIBarButtonItem *leftButton = [SBUBarButtonItem backButtonWithTarget:self selector:@selector(clickBackBtn)];
    self.navigationItem.leftBarButtonItem = leftButton;
}

- (void)clickBackBtn {
    [self.navigationController popViewControllerAnimated:YES];
}

- (void)didClickSaveAction {
    [self saveImageToPhotos:[self captureCurrentView:self.qrBgView]];
}

- (void)didShareAction {
    UIImage *image = [self captureCurrentView:self.qrBgView];
    JImageMessage *msg = [[JImageMessage alloc] initWithImage:image];
    ForwardSelectViewController *vc = [[ForwardSelectViewController alloc] initWithMessageContent:msg];
    UINavigationController *navi = [[UINavigationController alloc] initWithRootViewController:vc];
    navi.modalPresentationStyle = UIModalPresentationFullScreen;
    [self.navigationController presentViewController:navi animated:YES completion:nil];
}

- (UIImage *)captureCurrentView:(UIView *)view {
    CGRect frame = view.frame;
    UIGraphicsBeginImageContextWithOptions(frame.size, NO, [UIScreen mainScreen].scale);
    CGContextRef contextRef = UIGraphicsGetCurrentContext();
    [view.layer renderInContext:contextRef];
    UIImage *image = UIGraphicsGetImageFromCurrentImageContext();
    UIGraphicsEndImageContext();
    return image;
}

- (void)saveImageToPhotos:(UIImage *)image {
    [self savePhotosAlbumWithImage:image authorizationStatusBlock:^{
        UIAlertController *alertController =
            [UIAlertController alertControllerWithTitle:nil message:@"没有访问权限，请前往“设置-隐私-照片”选项中，允许访问您的照片" preferredStyle:UIAlertControllerStyleAlert];
        [alertController
            addAction:[UIAlertAction actionWithTitle:@"确认"
                                               style:UIAlertActionStyleDestructive
                                             handler:^(UIAlertAction *_Nonnull action){
            [self.navigationController popViewControllerAnimated:true];
                                             }]];
        [self presentViewController:alertController animated:true completion:nil];
    } resultBlock:^(BOOL success) {
        [self showHUDWithSuccess:success];
    }];
}

- (void)savePhotosAlbumWithImage:(UIImage *)image authorizationStatusBlock:(nullable dispatch_block_t)authorizationStatusBlock resultBlock:(nullable void (^)(BOOL success))resultBlock {
    PHAuthorizationStatus status = [PHPhotoLibrary authorizationStatus];
    if (PHAuthorizationStatusRestricted == status || PHAuthorizationStatusDenied == status) {
        if (authorizationStatusBlock) {
            authorizationStatusBlock();
        }
        return;
    }
    
    [[PHPhotoLibrary sharedPhotoLibrary] performChanges:^{
        [PHAssetChangeRequest creationRequestForAssetFromImage:image];
    } completionHandler:^(BOOL success, NSError * _Nullable error) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (resultBlock) {
                resultBlock(nil == error);
            }
        });
    }];
}

- (void)showHUDWithSuccess:(BOOL)success {
    if (success) {
        [self.view showHUDMessage:@"图片保存成功"];
    } else {
        [self.view showHUDMessage:@"图片保存失败"];
    }
}

- (void)addSubViews {
    [self.view addSubview:self.qrBgView];
    [self.view addSubview:self.shareBgView];
    UIView *lineView = [[UIView alloc] init];
    lineView.backgroundColor = ColorFromRGB(0xd8d8d8);
    [self.view addSubview:lineView];
    [self.qrBgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.width.offset(320);
        make.height.offset(440);
        make.top.equalTo(self.view).offset(58.5);
    }];
    [lineView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.width.equalTo(self.qrBgView);
        make.height.offset(0.5);
        make.bottom.equalTo(self.qrBgView.mas_bottom).offset(-46);
    }];
    [self.shareBgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.width.equalTo(self.qrBgView);
        make.height.offset(46);
        make.top.equalTo(lineView.mas_bottom);
    }];

    [self addQrBgViewSubviews];
    [self addShareBgViewSubviews];
}

- (void)addShareBgViewSubviews {
    [self.shareBgView addSubview:self.saveButton];
    [self.shareBgView addSubview:self.shareBtn];
    UIView *lineView1 = [[UIView alloc] init];
    lineView1.backgroundColor = ColorFromRGB(0xd8d8d8);
    [self.shareBgView addSubview:lineView1];
    UIView *lineView2 = [[UIView alloc] init];
    lineView2.backgroundColor = ColorFromRGB(0xd8d8d8);
    [self.shareBgView addSubview:lineView2];

    [self.saveButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.bottom.equalTo(self.shareBgView);
        make.width.offset(320 / 2);
    }];
    [lineView1 mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.equalTo(self.shareBgView);
        make.left.equalTo(self.saveButton.mas_right).offset(-0.5);
        make.width.offset(0.5);
    }];
    [self.shareBtn mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.equalTo(self.shareBgView);
        make.left.equalTo(self.saveButton.mas_right);
        make.right.equalTo(self.shareBgView);
    }];
}

- (void)addQrBgViewSubviews {
    [self.qrBgView addSubview:self.portraitImageView];
    [self.qrBgView addSubview:self.nameLabel];
    UIView *lineView = [[UIView alloc] init];
    lineView.backgroundColor = ColorFromRGB(0xd8d8d8);
    [self.qrBgView addSubview:lineView];

    [self.portraitImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.top.equalTo(self.qrBgView).offset(16);
        make.width.height.offset(50);
    }];

    if (self.conversation.conversationType == JConversationTypeGroup) {
        [self.qrBgView addSubview:self.qrCodeImageView];
        [self.qrBgView addSubview:self.countLabel];
        [self.qrBgView addSubview:self.infoLabel];
        [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(self.portraitImageView.mas_right).offset(12);
            make.right.equalTo(self.qrBgView.mas_right).offset(-12);
            make.top.equalTo(self.qrBgView).offset(17);
            make.height.offset(24);
        }];

        [self.countLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(self.portraitImageView.mas_right).offset(12);
            make.right.equalTo(self.qrBgView.mas_right).offset(-12);
            make.bottom.equalTo(self.portraitImageView.mas_bottom);
            make.height.offset(20);
        }];
    } else {
        [self.qrBgView addSubview:self.qrCodeImageView];
        [self.qrBgView addSubview:self.infoLabel];
        [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(self.portraitImageView.mas_right).offset(12);
            make.right.equalTo(self.qrBgView.mas_right).offset(-12);
            make.centerY.equalTo(self.portraitImageView);
            make.height.offset(24);
        }];
    }

    [lineView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.qrBgView);
        make.top.equalTo(self.qrBgView).offset(90);
        make.width.offset(280);
        make.height.offset(0.5);
    }];
    [self.qrCodeImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.qrBgView);
        make.top.equalTo(self.qrBgView).offset(76);
        make.width.height.offset(260);
    }];
    [self.infoLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.qrBgView);
        make.bottom.equalTo(self.qrBgView).offset(-72);
        make.height.offset(20);
        make.width.equalTo(self.qrBgView);
    }];
//    [self setQrImageBorderView];
}

- (void)setQrImageBorderView{
    UIView *topView = [self getQrImageBorderView];
    UIView *bottomView = [self getQrImageBorderView];
    UIView *leftView = [self getQrImageBorderView];
    UIView *rightView = [self getQrImageBorderView];
    [self.qrCodeImageView addSubview:topView];
    [self.qrCodeImageView addSubview:bottomView];
    [self.qrCodeImageView addSubview:leftView];
    [self.qrCodeImageView addSubview:rightView];
    [topView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.equalTo(self.qrCodeImageView);
        make.height.offset(15);
    }];
    [bottomView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.bottom.left.right.equalTo(self.qrCodeImageView);
        make.height.offset(15);
    }];
    [leftView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.bottom.equalTo(self.qrCodeImageView);
        make.width.offset(15);
    }];
    [rightView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.right.equalTo(self.qrCodeImageView);
        make.width.offset(15);
    }];
}

- (UIView *)getQrImageBorderView{
    UIView *view = [[UIView alloc] init];
    view.backgroundColor = ColorFromRGB(0xffffff);
    return view;
}

#pragma mark - getter
- (UIView *)qrBgView {
    if (!_qrBgView) {
        _qrBgView = [[UIView alloc] init];
        _qrBgView.backgroundColor = ColorFromRGB(0xffffff);
        _qrBgView.layer.masksToBounds = YES;
        _qrBgView.layer.cornerRadius = 8;
    }
    return _qrBgView;
}

- (UIImageView *)portraitImageView {
    if (!_portraitImageView) {
        _portraitImageView = [[UIImageView alloc] init];
        _portraitImageView.layer.masksToBounds = YES;
        _portraitImageView.layer.cornerRadius = 25;
    }
    return _portraitImageView;
}

- (UILabel *)nameLabel {
    if (!_nameLabel) {
        _nameLabel = [[UILabel alloc] init];
        _nameLabel.textColor = ColorFromRGB(0x111f2c);
        _nameLabel.font = [UIFont boldSystemFontOfSize:17];
    }
    return _nameLabel;
}

- (UILabel *)countLabel {
    if (!_countLabel) {
        _countLabel = [[UILabel alloc] init];
        _countLabel.textColor = ColorFromRGB(0xA0A5AB);
        _countLabel.font = [UIFont systemFontOfSize:14];
    }
    return _countLabel;
}

- (UIImageView *)qrCodeImageView {
    if (!_qrCodeImageView) {
        _qrCodeImageView = [[UIImageView alloc] init];
    }
    return _qrCodeImageView;
}

- (UILabel *)infoLabel {
    if (!_infoLabel) {
        _infoLabel = [[UILabel alloc] init];
        _infoLabel.textColor = ColorFromRGB(0x939393);
        _infoLabel.font = [UIFont systemFontOfSize:13];
        _infoLabel.textAlignment = NSTextAlignmentCenter;
    }
    return _infoLabel;
}

- (UIView *)shareBgView {
    if (!_shareBgView) {
        _shareBgView = [[UIView alloc] init];
        _shareBgView.backgroundColor = ColorFromRGB(0xffffff);
        _shareBgView.layer.masksToBounds = YES;
        _shareBgView.layer.cornerRadius = 8;
    }
    return _shareBgView;
}

- (UIButton *)saveButton {
    if (!_saveButton) {
        _saveButton = [[UIButton alloc] init];
        [_saveButton setTitleColor:ColorFromRGB(0x0099ff) forState:(UIControlStateNormal)];
        _saveButton.titleLabel.font = [UIFont systemFontOfSize:13];
        [_saveButton setTitle:@"保存图片" forState:(UIControlStateNormal)];
        [_saveButton addTarget:self
                        action:@selector(didClickSaveAction)
              forControlEvents:(UIControlEventTouchUpInside)];
    }
    return _saveButton;
}

- (UIButton *)shareBtn {
    if (!_shareBtn) {
        _shareBtn = [[UIButton alloc] init];
        [_shareBtn setTitleColor:ColorFromRGB(0x0099ff) forState:(UIControlStateNormal)];
        _shareBtn.titleLabel.font = [UIFont systemFontOfSize:13];
        [_shareBtn setTitle:@"分享至 JuggleChat" forState:(UIControlStateNormal)];
        [_shareBtn addTarget:self
                      action:@selector(didShareAction)
            forControlEvents:(UIControlEventTouchUpInside)];
    }
    return _shareBtn;
}

@end
