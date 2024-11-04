//
//  SingleCallViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/11/1.
//

#import "SingleCallViewController.h"
#import "CallTheme.h"
#import "CallCenter.h"

@interface SingleCallViewController () <JCallSessionDelegate>
@property (nonatomic, strong) id<JCallSession> callSession;
@property(nonatomic, strong) UIView *backgroundView;

/*!
 蒙层View
 */
@property(nonatomic, strong) UIVisualEffectView *blurView;

/*!
 对端的名字Label
 */
@property(nonatomic, strong) UILabel *remoteNameLabel;

/*!
 用户状态的view
 */
@property(nonatomic, strong) UILabel *statusView;

/*!
 提示Label
 */
@property(nonatomic, strong) UILabel *tipsLabel;

/*!
 最小化Button
 */
@property(nonatomic, strong) UIButton *minimizeButton;

/*!
 通话时长Label
 */
@property(nonatomic, strong) UILabel *timeLabel;

/*!
 静音Button
 */
@property(nonatomic, strong) UIButton *muteButton;


/*!
 扬声器Button
 */
@property(nonatomic, strong) UIButton *speakerButton;


/*!
 接听Button
 */
@property(nonatomic, strong) UIButton *acceptButton;


/*!
 挂断Button
 */
@property(nonatomic, strong) UIButton *hangupButton;


/*!
 关闭摄像头的Button
 */
@property(nonatomic, strong) UIButton *cameraCloseButton;


/*!
 切换前后摄像头的Button
 */
@property(nonatomic, strong) UIButton *cameraSwitchButton;

///*!
// 全屏的视频View
// */
//@property(nonatomic, strong) UIView *mainVideoView;
//
///*!
// 通话接通后，界面右上角的视频View
// */
//@property(nonatomic, strong) UIView *subVideoView;

@property(nonatomic, strong) UIView *topGradientView;
@property(nonatomic, strong) UIView *bottomGradientView;

@end

@implementation SingleCallViewController

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession {
    self = [super init];
    if (self) {
        self.callSession = callSession;
        [self.callSession addDelegate:self];
    }
    return self;
}

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession {
    self = [super init];
    if (self) {
        self.callSession = callSession;
        [self.callSession addDelegate:self];
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    // Do any additional setup after loading the view.
    
    self.backgroundView = [[UIView alloc] initWithFrame:self.view.bounds];
    self.backgroundView.backgroundColor = ColorFromRGB(0x262e42);
    [self.view addSubview:self.backgroundView];
    self.backgroundView.hidden = NO;
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];

    self.backgroundView.backgroundColor = ColorFromRGB(0x262e42);
    self.backgroundView.hidden = NO;
    self.topGradientView.hidden = YES;
    self.bottomGradientView.hidden = YES;


    self.blurView.hidden = NO;


    if (self.callSession.callStatus == JCallStatusConnected) {
        self.minimizeButton.frame = CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin + CallStatusBarHeight,
                                               CallButtonLength / 2, CallButtonLength / 2);
        self.minimizeButton.hidden = NO;
    } else if (self.callSession.callStatus != JCallStatusIdle) {
        self.minimizeButton.hidden = YES;
    }


//    self.inviteUserButton.hidden = YES;


    // header orgin y = RCCallVerticalMargin * 3
    if (self.callSession.callStatus == JCallStatusConnected) {
        self.timeLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        self.timeLabel.hidden = NO;
    } else if (self.callSession.callStatus != JCallStatusIdle) {
        self.timeLabel.hidden = YES;
    }


    if (self.callSession.callStatus == JCallStatusIdle) {
        self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength -
                           CallInsideMargin * 3 - CallLabelHeight - CallExtraSpace,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
    } else if (self.callSession.callStatus == JCallStatusConnected) {
        self.tipsLabel.frame = CGRectMake(
            CallHorizontalMargin,
            MAX((self.view.frame.size.height - CallLabelHeight) / 2,
                CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin * 3 + CallLabelHeight * 2),
            self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
    } else {
        self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
    }
    self.tipsLabel.hidden = NO;

    if (self.callSession.callStatus == JCallStatusConnected) {
        self.muteButton.frame = CGRectMake(CallHorizontalMargin,
                                           self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace,
                                           CallButtonLength, CallButtonLength);
        [self layoutTextUnderImageButton:self.muteButton];
        self.muteButton.hidden = NO;
    } else if (self.callSession.callStatus != JCallStatusIdle) {
        self.muteButton.hidden = YES;
    }


    if (self.callSession.callStatus == JCallStatusConnected) {
        self.speakerButton.frame =
            CGRectMake(self.view.frame.size.width - CallHorizontalMargin - CallButtonLength,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                       CallButtonLength);
        [self layoutTextUnderImageButton:self.speakerButton];
        self.speakerButton.hidden = NO;
//        [self.speakerButton setSelected:self.callSession.speakerEnabled];
    } else if (self.callSession.callStatus != JCallStatusIdle) {
        self.speakerButton.hidden = YES;
    }


    if (self.callSession.callStatus == JCallStatusOutgoing) {
        self.hangupButton.frame =
            CGRectMake((self.view.frame.size.width - CallButtonLength) / 2,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                       CallButtonLength);
        [self layoutTextUnderImageButton:self.hangupButton];
        self.hangupButton.hidden = NO;


        self.acceptButton.hidden = YES;
    } else if (self.callSession.callStatus == JCallStatusIncoming) {
        self.hangupButton.frame = CGRectMake(
            CallHorizontalMargin, self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace,
            CallButtonLength, CallButtonLength);
        [self layoutTextUnderImageButton:self.hangupButton];
        self.hangupButton.hidden = NO;


        self.acceptButton.frame =
            CGRectMake(self.view.frame.size.width - CallHorizontalMargin - CallButtonLength,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                       CallButtonLength);
        [self layoutTextUnderImageButton:self.acceptButton];
        self.acceptButton.hidden = NO;
    } else if (self.callSession.callStatus == JCallStatusConnected) {
        self.hangupButton.frame =
            CGRectMake((self.view.frame.size.width - CallButtonLength) / 2,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                       CallButtonLength);
        [self layoutTextUnderImageButton:self.hangupButton];
        self.hangupButton.hidden = NO;


        self.acceptButton.hidden = YES;
    }


    self.cameraCloseButton.hidden = YES;
    self.cameraSwitchButton.hidden = YES;
    
    
    
    
    
    self.remoteNameLabel.frame =
    CGRectMake(CallHorizontalMargin, CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin,
               self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
    self.remoteNameLabel.hidden = NO;
    
    self.remoteNameLabel.textAlignment = NSTextAlignmentCenter;
    self.tipsLabel.textAlignment = NSTextAlignmentCenter;
    
    self.statusView.frame = CGRectMake((self.view.frame.size.width - 17) / 2,
                                       CallVerticalMargin * 3 + (CallHeaderLength - 4) / 2, 17, 4);
    
    if (self.callSession.callStatus == JCallStatusOutgoing
        || self.callSession.callStatus == JCallStatusIncoming) {
        self.statusView.hidden = NO;
    } else {
        self.statusView.hidden = YES;
    }
    [self resetRemoteUserInfoIfNeed];
}

- (void)resetRemoteUserInfoIfNeed {
    JCallMember *member = self.callSession.members[0];
    if (member.userInfo.userName.length > 0) {
        [self.remoteNameLabel setText:member.userInfo.userName];
    } else {
        [self.remoteNameLabel setText:member.userInfo.userId];
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

#pragma mark - JCallSessionDelegate
/// 通话已接通
- (void)callDidConnect {
    
}

/// 通话已结束
/// - Parameter finishReason: 结束原因
- (void)callDidFinish:(JCallFinishReason)finishReason {
    [CallCenter.shared dismissCallViewController:self];
}

/// 用户被邀请
/// - Parameter userId: 被邀请的用户 id
- (void)userDidInvite:(NSString *)userId {
    
}

/// 用户加入通话
/// - Parameter userId: 用户 id
- (void)userDidConnect:(NSString *)userId {
    
}

/// 用户退出通话
/// - Parameter userId: 用户 id
- (void)userDidLeave:(NSString *)userId {
    
}

/// 用户开启/关闭摄像头
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId {
    
}

/// 用户开启/关闭麦克风
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userMicrophoneDidChange:(BOOL)enable
                         userId:(NSString *)userId {
    
}

/// 通话中的错误回调
/// - Parameter errorCode: 错误码
- (void)errorDidOccur:(JCallErrorCode)errorCode {
    
}

#pragma mark - getter
- (UILabel *)remoteNameLabel {
    if (!_remoteNameLabel) {
        _remoteNameLabel = [[UILabel alloc] init];
        _remoteNameLabel.backgroundColor = [UIColor clearColor];
        _remoteNameLabel.textColor = [UIColor whiteColor];
        _remoteNameLabel.font = [UIFont systemFontOfSize:18];
        _remoteNameLabel.textAlignment = NSTextAlignmentCenter;

        [self.view addSubview:_remoteNameLabel];
        _remoteNameLabel.hidden = YES;
    }
    return _remoteNameLabel;
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

- (UILabel *)tipsLabel {
    if (!_tipsLabel) {
        _tipsLabel = [[UILabel alloc] init];
        _tipsLabel.backgroundColor = [UIColor clearColor];
        _tipsLabel.textColor = [UIColor whiteColor];
        _tipsLabel.font = [UIFont systemFontOfSize:18];
        _tipsLabel.textAlignment = NSTextAlignmentCenter;

        [self.view addSubview:_tipsLabel];
        _tipsLabel.hidden = YES;
    }
    return _tipsLabel;
}

//- (UIView *)mainVideoView {
//    if (!_mainVideoView) {
//        _mainVideoView = [[UIView alloc] initWithFrame:self.backgroundView.frame];
//        _mainVideoView.backgroundColor = RongVoIPUIColorFromRGB(0x262e42);
//
//        [self.backgroundView addSubview:_mainVideoView];
//        _mainVideoView.hidden = YES;
//    }
//    return _mainVideoView;
//}
//
//- (UIView *)subVideoView {
//    if (!_subVideoView) {
//        _subVideoView = [[UIView alloc] init];
//        _subVideoView.backgroundColor = [UIColor blackColor];
//        _subVideoView.layer.borderWidth = 1;
//        _subVideoView.layer.borderColor = [[UIColor whiteColor] CGColor];
//
//        [self.view addSubview:_subVideoView];
//        _subVideoView.hidden = YES;
//
//        UITapGestureRecognizer *tap =
//            [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(subVideoViewClicked)];
//        [_subVideoView addGestureRecognizer:tap];
//    }
//    return _subVideoView;
//}


- (UIView *)topGradientView {
    if (!_topGradientView) {
        _topGradientView = [[UIView alloc] initWithFrame:CGRectMake(0, 0, self.view.frame.size.width, CallTopGGradientHeight)];
        _topGradientView.backgroundColor = [UIColor clearColor];
        [_topGradientView.layer insertSublayer:[self getGradientLayerWithFrame:CGRectMake(0, 0, self.view.frame.size.width, CallTopGGradientHeight) direction:YES] atIndex:0];

        [self.view insertSubview:_topGradientView aboveSubview:self.backgroundView];
        _topGradientView.hidden = YES;
    }
    return _topGradientView;
}


- (UIView *)bottomGradientView {
    if (!_bottomGradientView) {
        _bottomGradientView = [[UIView alloc] initWithFrame:CGRectMake(0, self.view.frame.size.height - CallBottomGradientHeight, self.view.frame.size.width, CallBottomGradientHeight)];
        _bottomGradientView.backgroundColor = [UIColor clearColor];
        [_bottomGradientView.layer insertSublayer:[self getGradientLayerWithFrame:CGRectMake(0, 0, self.view.frame.size.width, CallBottomGradientHeight) direction:NO] atIndex:0];

        [self.view insertSubview:_bottomGradientView aboveSubview:self.backgroundView];
        _bottomGradientView.hidden = YES;
    }
    return _bottomGradientView;
}


- (CAGradientLayer *)getGradientLayerWithFrame:(CGRect)frame direction:(BOOL)isUpToDown{

    //为透明度设置渐变效果
    UIColor *colorBegin = [UIColor colorWithRed:(0/255.0)  green:(0/255.0)  blue:(0/255.0)  alpha:0.3];
    UIColor *colorEnd = [UIColor colorWithRed:(0/255.0)  green:(0/255.0)  blue:(0/255.0)  alpha:0.0];
    NSArray *colors = [NSArray arrayWithObjects:(id)colorBegin.CGColor, (id)colorEnd.CGColor, nil];
    CAGradientLayer *gradient = [CAGradientLayer layer];
    //设置开始和结束位置(设置渐变的方向)
    if (isUpToDown) {
        gradient.startPoint = CGPointMake(0, 0);
        gradient.endPoint = CGPointMake(0, 1);
    }else {
        gradient.startPoint = CGPointMake(0, 1);
        gradient.endPoint = CGPointMake(0, 0);
    }
    gradient.colors = colors;
    gradient.frame = frame;
    return gradient;
}

- (UIButton *)minimizeButton {
    if (!_minimizeButton) {
        _minimizeButton =
            [[UIButton alloc] initWithFrame:CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin / 2,
                                                       CallButtonLength, CallButtonLength)];
//        [_minimizeButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/minimize.png"]
//                         forState:UIControlStateNormal];
//        [_minimizeButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/minimize.png"]
//                         forState:UIControlStateHighlighted];
        [_minimizeButton setTitle:@"Minimize" forState:UIControlStateNormal];


//        [_minimizeButton addTarget:self
//                            action:@selector(minimizeButtonClicked)
//                  forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_minimizeButton];
        _minimizeButton.hidden = YES;
    }
    return _minimizeButton;
}

- (UILabel *)timeLabel {
    if (!_timeLabel) {
        _timeLabel = [[UILabel alloc] init];
        _timeLabel.backgroundColor = [UIColor clearColor];
        _timeLabel.textColor = [UIColor whiteColor];
        _timeLabel.font = [UIFont systemFontOfSize:18];
        _timeLabel.textAlignment = NSTextAlignmentCenter;


        [self.view addSubview:_timeLabel];
        _timeLabel.hidden = YES;
    }
    return _timeLabel;
}

- (UIButton *)muteButton {
    if (!_muteButton) {
        _muteButton = [[UIButton alloc] init];
//        [_muteButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/mute.png"] forState:UIControlStateNormal];
//        [_muteButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/mute_hover.png"]
//                     forState:UIControlStateHighlighted];
//        [_muteButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/mute_hover.png"]
//                     forState:UIControlStateSelected];
        [_muteButton setTitle:@"Mute"
                     forState:UIControlStateNormal];
//        [_muteButton setSelected:self.callSession.isMuted];
        [_muteButton addTarget:self action:@selector(muteButtonClicked) forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_muteButton];
        _muteButton.hidden = YES;
    }
    return _muteButton;
}


- (void)muteButtonClicked {
//    [self didTapMuteButton];
//
//
//    [self.callSession setMuted:!self.callSession.isMuted];
//    [self.muteButton setSelected:self.callSession.isMuted];
}


- (UIButton *)speakerButton {
    if (!_speakerButton) {
        _speakerButton = [[UIButton alloc] init];
//        [_speakerButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/handfree.png"]
//                        forState:UIControlStateNormal];
//        [_speakerButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/handfree_hover.png"]
//                        forState:UIControlStateHighlighted];
//        [_speakerButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/handfree_hover.png"]
//                        forState:UIControlStateSelected];
        [_speakerButton setTitle:@"Speaker"
                        forState:UIControlStateNormal];
//        [_speakerButton setSelected:self.callSession.speakerEnabled];


        [_speakerButton addTarget:self
                           action:@selector(speakerButtonClicked)
                 forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_speakerButton];
        _speakerButton.hidden = YES;
    }
    return _speakerButton;
}


- (void)speakerButtonClicked {
//    [self didTapSpeakerButton];
//
//
//    [self.callSession setSpeakerEnabled:!self.callSession.speakerEnabled];
//    [self.speakerButton setSelected:self.callSession.speakerEnabled];
}


- (UIButton *)acceptButton {
    if (!_acceptButton) {
        _acceptButton = [[UIButton alloc] init];
//        [_acceptButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/answer.png"]
//                       forState:UIControlStateNormal];
//        [_acceptButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/answer_hover.png"]
//                       forState:UIControlStateHighlighted];
        [_acceptButton setTitle:@"Accept"
                       forState:UIControlStateNormal];


        [_acceptButton addTarget:self
                          action:@selector(acceptButtonClicked)
                forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_acceptButton];
        _acceptButton.hidden = YES;
    }
    return _acceptButton;
}


- (void)acceptButtonClicked {
//    [self didTapAcceptButton];
//
//
//    if (!self.callSession) {
//        [self callDidDisconnect];
//    } else {
//        [self.callSession accept:self.callSession.mediaType];
//    }
}


- (UIButton *)hangupButton {
    if (!_hangupButton) {
        _hangupButton = [[UIButton alloc] init];
//        [_hangupButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/hang_up.png"]
//                       forState:UIControlStateNormal];
//        [_hangupButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/hang_up_hover.png"]
//                       forState:UIControlStateHighlighted];
        [_hangupButton setTitle:@"Hang Up"
                       forState:UIControlStateNormal];


        [_hangupButton addTarget:self
                          action:@selector(hangupButtonClicked)
                forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_hangupButton];
        _hangupButton.hidden = YES;
    }
    return _hangupButton;
}


- (void)hangupButtonClicked {
    [self.callSession hangup];
}


- (UIButton *)cameraCloseButton {
    if (!_cameraCloseButton) {
        _cameraCloseButton = [[UIButton alloc] init];


        if (!self.callSession.isMultiCall) {
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/audio.png"]
//                                forState:UIControlStateNormal];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/audio.png"]
//                                forState:UIControlStateHighlighted];
            [_cameraCloseButton
                setTitle:@"Switch to voice"
                forState:UIControlStateNormal];
        } else {
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video.png"]
//                                forState:UIControlStateNormal];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video.png"]
//                                forState:UIControlStateHighlighted];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video_hover_.png"]
//                                forState:UIControlStateSelected];
            [_cameraCloseButton setTitle:@"Turn off camera"
                                forState:UIControlStateNormal];
            [_cameraCloseButton setTitle:@"Turn on camera"
                                forState:UIControlStateSelected];
        }


//        [_cameraCloseButton setSelected:!self.callSession.cameraEnabled];
        [_cameraCloseButton addTarget:self
                               action:@selector(cameraCloseButtonClicked)
                     forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_cameraCloseButton];
        _cameraCloseButton.hidden = YES;
    }
    return _cameraCloseButton;
}


- (void)cameraCloseButtonClicked {
//    [self didTapCameraCloseButton];
//
//
//    if (!self.callSession.isMultiCall) {
//        [self.callSession setVideoView:nil userId:[RCIMClient sharedRCIMClient].currentUserInfo.userId];
//        [self.callSession setVideoView:nil userId:self.callSession.targetId];
//
//
//        if (self.callSession.callStatus == RCCallIncoming || self.callSession.callStatus == RCCallRinging) {
//            [self.callSession accept:RCCallMediaAudio];
//        }
//
//
//        if ([self.callSession changeMediaType:RCCallMediaAudio]) {
//            [self resetLayout:self.callSession.isMultiCall
//                    mediaType:RCCallMediaAudio
//                   callStatus:self.callSession.callStatus];
//        }
//    } else {
//        [self.callSession setCameraEnabled:!self.callSession.cameraEnabled];
//        [self.cameraCloseButton setSelected:!self.callSession.cameraEnabled];
//    }
}


- (UIButton *)cameraSwitchButton {
    if (!_cameraSwitchButton) {
        _cameraSwitchButton = [[UIButton alloc] init];


        if (!self.callSession.isMultiCall) {
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/camera.png"]
//                                 forState:UIControlStateNormal];
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/camera_hover.png"]
//                                 forState:UIControlStateHighlighted];
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/camera_hover.png"]
//                                 forState:UIControlStateSelected];
            [_cameraSwitchButton setTitle:@"Camera"
                                 forState:UIControlStateNormal];
            [_cameraSwitchButton setTitle:@"Camera"
                                 forState:UIControlStateSelected];
        } else {
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/change.png"]
//                                 forState:UIControlStateNormal];
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/change.png"]
//                                 forState:UIControlStateHighlighted];
//            [_cameraSwitchButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/change.png"]
//                                 forState:UIControlStateSelected];
        }


        [_cameraSwitchButton addTarget:self
                                action:@selector(cameraSwitchButtonClicked)
                      forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_cameraSwitchButton];
        _cameraSwitchButton.hidden = YES;
    }
    return _cameraSwitchButton;
}


- (void)cameraSwitchButtonClicked {
//    [self didTapCameraSwitchButton];
//
//
//    [self.callSession switchCameraMode];
}

@end
