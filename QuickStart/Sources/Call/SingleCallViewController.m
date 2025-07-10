//
//  SingleCallViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/11/1.
//

#import "SingleCallViewController.h"
#import "CallTheme.h"
#import "CallCenter.h"

@interface SingleCallViewController ()

/*!
 对端的头像View
 */
@property(nonatomic, strong) UIImageView *remotePortraitView;

/*!
 对端的名字Label
 */
@property(nonatomic, strong) UILabel *mainNameLabel;

/*!
 用户状态的view
 */
@property(nonatomic, strong) UILabel *statusView;

/*!
 全屏的视频View
 */
@property(nonatomic, strong) UIView *mainVideoView;

/*!
 通话接通后，界面右上角的视频View
 */
@property(nonatomic, strong) UIView *subVideoView;

@property(nonatomic, strong) JUserInfo *remoteUserInfo;
/// 是否切换了自己和对方的视频 view（默认对方为 mainView，自己为 subView）
@property(nonatomic, assign) BOOL switchMainSubVideo;
@end

@implementation SingleCallViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

//    [self resetLayout];
}

- (void)resetLayout {
    [super resetLayout];
    UIImage *remoteHeaderImage = self.remotePortraitView.image;
    
    if (self.callSession.mediaType == JCallMediaTypeVoice) {
        self.remotePortraitView.frame = CGRectMake((self.view.frame.size.width - CallHeaderLength) / 2,
                                                   CallVerticalMargin * 3, CallHeaderLength, CallHeaderLength);
        self.remotePortraitView.image = remoteHeaderImage;
        self.remotePortraitView.hidden = NO;
        
        self.mainNameLabel.frame =
        CGRectMake(CallHorizontalMargin, CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin,
                   self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        self.mainNameLabel.hidden = NO;
        
        self.mainNameLabel.textAlignment = NSTextAlignmentCenter;
        self.tipsLabel.textAlignment = NSTextAlignmentCenter;
        self.statusView.frame = CGRectMake((self.view.frame.size.width - 17) / 2,
                                           CallVerticalMargin * 3 + (CallHeaderLength - 4) / 2, 17, 4);

        if (self.callSession.callStatus == JCallStatusOutgoing
            || self.callSession.callStatus == JCallStatusIncoming) {
            self.remotePortraitView.alpha = 0.5;
            self.statusView.hidden = NO;
        } else {
            self.statusView.hidden = YES;
            self.remotePortraitView.alpha = 1.0;
        }

        self.mainVideoView.hidden = YES;
        self.subVideoView.hidden = YES;
        [self resetRemoteUserInfoIfNeed];
    } else {
        if (self.callSession.callStatus == JCallStatusOutgoing) {
            self.mainVideoView.hidden = NO;
            [self.callSession startPreview:self.mainVideoView];
            self.blurView.hidden = YES;
        } else if (self.callSession.callStatus == JCallStatusIncoming) {
            [self.callSession startPreview:self.mainVideoView];
            self.mainVideoView.hidden = NO;
        } else if (self.callSession.callStatus == JCallStatusConnected) {
            self.mainVideoView.hidden = NO;
            if (self.callSession.members.count > 0) {
                JCallMember *member = self.callSession.members[0];
                [self.callSession setVideoView:self.mainVideoView forUserId:member.userInfo.userId];
            }
            self.blurView.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.remotePortraitView.hidden = YES;

            self.mainNameLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallStatusBarHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.mainNameLabel.hidden = NO;
            self.mainNameLabel.textAlignment = NSTextAlignmentCenter;
        } else if (self.callSession.callStatus == JCallStatusOutgoing) {
            self.remotePortraitView.frame =
                CGRectMake((self.view.frame.size.width - CallHeaderLength) / 2, CallVerticalMargin * 3,
                           CallHeaderLength, CallHeaderLength);
            self.remotePortraitView.image = remoteHeaderImage;
            self.remotePortraitView.hidden = NO;

            self.mainNameLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.mainNameLabel.hidden = NO;
            self.mainNameLabel.textAlignment = NSTextAlignmentCenter;
        } else if (self.callSession.callStatus == JCallStatusIncoming) {
            self.remotePortraitView.frame =
                CGRectMake((self.view.frame.size.width - CallHeaderLength) / 2, CallVerticalMargin * 3,
                           CallHeaderLength, CallHeaderLength);
            self.remotePortraitView.image = remoteHeaderImage;
            self.remotePortraitView.hidden = NO;

            self.mainNameLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.mainNameLabel.hidden = NO;
            self.mainNameLabel.textAlignment = NSTextAlignmentCenter;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
//            if ([RCCallKitUtility isLandscape] && [self isSupportOrientation:(UIInterfaceOrientation)[UIDevice currentDevice].orientation]) {
//                self.subVideoView.frame =
//                    CGRectMake(self.view.frame.size.width - RCCallHeaderLength - RCCallHorizontalMargin / 2,
//                               RCCallVerticalMargin, RCCallHeaderLength * 1.5, RCCallHeaderLength);
//            } else 
            {
                self.subVideoView.frame =
                    CGRectMake(self.view.frame.size.width - CallHeaderLength - CallHorizontalMargin / 2,
                               CallVerticalMargin + CallStatusBarHeight, CallHeaderLength, CallHeaderLength * 1.5);
            }
            [self.callSession setVideoView:self.subVideoView forUserId:JIM.shared.currentUserId];
            self.subVideoView.hidden = NO;
        } else {
            self.subVideoView.hidden = YES;
        }

        self.mainNameLabel.textAlignment = NSTextAlignmentCenter;
        self.statusView.frame = CGRectMake((self.view.frame.size.width - 17) / 2,
                                           CallVerticalMargin * 3 + (CallHeaderLength - 4) / 2, 17, 4);

        if (self.callSession.callStatus == JCallStatusOutgoing
            || self.callSession.callStatus == JCallStatusIncoming) {
            self.remotePortraitView.alpha = 0.5;
            self.statusView.hidden = NO;
        } else {
            self.statusView.hidden = YES;
            self.remotePortraitView.alpha = 1.0;
        }
    }
}

- (void)resetRemoteUserInfoIfNeed {
    if (self.callSession.members.count == 0) {
        return;
    }
    JCallMember *member = self.callSession.members[0];
    if (member.userInfo.userName.length > 0) {
        [self.mainNameLabel setText:member.userInfo.userName];
    } else {
        [self.mainNameLabel setText:member.userInfo.userId];
    }
}

- (void)layoutTextUnderImageButton:(UIButton *)button {
    [button.titleLabel setFont:[UIFont systemFontOfSize:12]];
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];


    button.titleEdgeInsets = UIEdgeInsetsMake(0, -button.imageView.frame.size.width,
                                              -button.imageView.frame.size.height - CallInsideMargin, 0);
    button.imageEdgeInsets = UIEdgeInsetsMake(-button.titleLabel.intrinsicContentSize.height - CallInsideMargin, 0, 0,
                                              -button.titleLabel.intrinsicContentSize.width);
}

#pragma mark - getter
- (UILabel *)mainNameLabel {
    if (!_mainNameLabel) {
        _mainNameLabel = [[UILabel alloc] init];
        _mainNameLabel.backgroundColor = [UIColor clearColor];
        _mainNameLabel.textColor = [UIColor whiteColor];
        _mainNameLabel.font = [UIFont systemFontOfSize:18];
        _mainNameLabel.textAlignment = NSTextAlignmentCenter;

        [self.view addSubview:_mainNameLabel];
        _mainNameLabel.hidden = YES;
    }
    return _mainNameLabel;
}

- (UIImageView *)remotePortraitView {
    if (!_remotePortraitView) {
        _remotePortraitView = [[UIImageView alloc] init];


        [self.view addSubview:_remotePortraitView];
        _remotePortraitView.hidden = YES;
//        [_remotePortraitView setPlaceholderImage:[RCCallKitUtility getDefaultPortraitImage]];
        _remotePortraitView.layer.cornerRadius = 4;
        _remotePortraitView.layer.masksToBounds = YES;
    }
    return _remotePortraitView;
}

- (UILabel *)statusView {
    if (!_statusView) {
        _statusView = [[UILabel alloc] init];
        _statusView.backgroundColor = [UIColor clearColor];
        _statusView.textColor = [UIColor whiteColor];
        _statusView.font = [UIFont systemFontOfSize:18];
        _statusView.textAlignment = NSTextAlignmentCenter;
        
        [self.view addSubview:_statusView];
        _statusView.hidden = YES;
    }
    return _statusView;
}

- (UIView *)mainVideoView {
    if (!_mainVideoView) {
        _mainVideoView = [[UIView alloc] initWithFrame:self.backgroundView.frame];
        _mainVideoView.backgroundColor = ColorFromRGB(0x262e42);

        [self.backgroundView addSubview:_mainVideoView];
        _mainVideoView.hidden = YES;
    }
    return _mainVideoView;
}

- (UIView *)subVideoView {
    if (!_subVideoView) {
        _subVideoView = [[UIView alloc] init];
        _subVideoView.backgroundColor = [UIColor blackColor];
        _subVideoView.layer.borderWidth = 1;
        _subVideoView.layer.borderColor = [[UIColor whiteColor] CGColor];

        [self.view addSubview:_subVideoView];
        _subVideoView.hidden = YES;

        UITapGestureRecognizer *tap =
            [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(subVideoViewClicked)];
        [_subVideoView addGestureRecognizer:tap];
    }
    return _subVideoView;
}




- (void)subVideoViewClicked {
    NSString *currentId = JIM.shared.currentUserId;
    JUserInfo *currentUserInfo = [JIM.shared.userInfoManager getUserInfo:currentId];
    JCallMember *member = self.callSession.members[0];
    JUserInfo *remoteUserInfo = member.userInfo;
    
    JUserInfo *mainUser;
    JUserInfo *subUser;
    if (self.switchMainSubVideo) {
        mainUser = remoteUserInfo;
        subUser = currentUserInfo;
    } else {
        mainUser = currentUserInfo;
        subUser = remoteUserInfo;
    }
    self.switchMainSubVideo = !self.switchMainSubVideo;
    
    NSString *name = mainUser.userName;
    if (name.length == 0) {
        name = mainUser.userId;
    }
    [self.mainNameLabel setText:name];
//    [self.remotePortraitView setImageURL:[NSURL URLWithString:mainUser.portrait]];
    
    [self.callSession setVideoView:self.mainVideoView forUserId:mainUser.userId];
    [self.callSession setVideoView:self.subVideoView forUserId:subUser.userId];
}

@end
