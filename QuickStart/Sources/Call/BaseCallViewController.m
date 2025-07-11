//
//  BaseCallViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

#import "BaseCallViewController.h"
#import "CallTheme.h"
#import "CallCenter.h"

@interface BaseCallViewController () <JCallSessionDelegate>
@property(nonatomic, strong) UIView *topGradientView;
@property(nonatomic, strong) UIView *bottomGradientView;
@end

@implementation BaseCallViewController

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
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    
    [self resetLayout];
}

- (void)resetLayout {
    if (self.callSession.mediaType == JCallMediaTypeVoice && !self.callSession.isMultiCall) {
        
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
        self.inviteUserButton.hidden = YES;
        
        
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
    } else if (self.callSession.mediaType == JCallMediaTypeVideo && !self.callSession.isMultiCall) {
        self.backgroundView.hidden = NO;
        
        self.blurView.hidden = YES;
        
        if (self.callSession.callStatus == JCallStatusConnected) {
            self.minimizeButton.frame = CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin + CallStatusBarHeight,
                                                   CallButtonLength / 2, CallButtonLength / 2);
            self.minimizeButton.hidden = NO;
            self.topGradientView.hidden = NO;
            self.bottomGradientView.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.minimizeButton.hidden = YES;
            self.topGradientView.hidden = YES;
            self.bottomGradientView.hidden = YES;
        }
        
        self.inviteUserButton.hidden = YES;
        
        if (self.callSession.callStatus == JCallStatusConnected) {
            self.timeLabel.frame =
            CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallInsideMargin + CallLabelHeight + CallStatusBarHeight,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.timeLabel.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.timeLabel.hidden = YES;
        }
        
        if (self.callSession.callStatus == JCallStatusConnected) {
            self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       MAX((self.view.frame.size.height - CallLabelHeight) / 2,
                           CallVerticalMargin + CallHeaderLength * 1.5 + CallInsideMargin * 3),
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusOutgoing) {
            self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusIncoming) {
            self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       CallVerticalMargin * 3 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusIdle) {
            self.tipsLabel.frame = CGRectMake(
                                              CallHorizontalMargin,
                                              self.view.frame.size.height - CallVerticalMargin - CallButtonLength * 2 - CallInsideMargin * 8 - CallExtraSpace,
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
        
        self.speakerButton.hidden = YES;
        
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
        
        if (self.callSession.callStatus == JCallStatusConnected) {
            self.cameraSwitchButton.frame =
            CGRectMake(self.view.frame.size.width - CallHorizontalMargin - CallButtonLength,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                       CallButtonLength);
            [self layoutTextUnderImageButton:self.cameraSwitchButton];
            self.cameraSwitchButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.cameraSwitchButton.hidden = YES;
        }
        
        if (self.callSession.callStatus == JCallStatusConnected) {
            self.cameraCloseButton.frame = CGRectMake(
                                                      self.view.frame.size.width - CallHorizontalMargin - CallButtonLength,
                                                      self.view.frame.size.height - CallVerticalMargin - CallButtonLength * 2 - CallInsideMargin * 5 - CallExtraSpace,
                                                      CallButtonLength, CallButtonLength);
            [self layoutTextUnderImageButton:self.cameraCloseButton];
            self.cameraCloseButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.cameraCloseButton.hidden = YES;
        }
    } else if (self.callSession.mediaType == JCallMediaTypeVoice && self.callSession.isMultiCall) {
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

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.inviteUserButton.frame =
                CGRectMake(self.view.frame.size.width - CallHorizontalMargin / 2 - CallButtonLength / 2,
                           CallVerticalMargin + CallStatusBarHeight, CallButtonLength / 2, CallButtonLength / 2);
            self.inviteUserButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.inviteUserButton.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.timeLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallStatusBarHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.timeLabel.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.timeLabel.hidden = YES;
        }

        // header orgin y = RCCallVerticalMargin * 2
        if (self.callSession.callStatus == JCallStatusIncoming) {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin,
                           CallVerticalMargin * 2 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusOutgoing) {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallStatusBarHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusIdle) {
            self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength -
                       CallInsideMargin * 3 - CallLabelHeight - CallExtraSpace,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin,
                           self.view.frame.size.height - CallVerticalMargin - CallButtonLength -
                               CallInsideMargin * 3 - CallLabelHeight,
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
        } else if (self.callSession.callStatus == JCallStatusConnected || self.callSession.callStatus == JCallStatusConnecting) {
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

    } else if (self.callSession.mediaType == JCallMediaTypeVideo && self.callSession.isMultiCall) {
        self.backgroundView.hidden = NO;

        self.blurView.hidden = YES;

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.minimizeButton.frame = CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin + CallStatusBarHeight,
                                                   CallButtonLength / 2, CallButtonLength / 2);
            self.minimizeButton.hidden = NO;
            self.topGradientView.hidden = NO;
            self.bottomGradientView.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.minimizeButton.hidden = YES;
            self.topGradientView.hidden = YES;
            self.bottomGradientView.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.cameraSwitchButton.frame = CGRectMake(
                self.view.frame.size.width - CallHorizontalMargin / 2 - CallButtonLength - CallInsideMargin,
                CallVerticalMargin + CallStatusBarHeight, CallButtonLength / 2, CallButtonLength / 2);
            self.cameraSwitchButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.cameraSwitchButton.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.inviteUserButton.frame =
                CGRectMake(self.view.frame.size.width - CallHorizontalMargin / 2 - CallButtonLength / 2,
                           CallVerticalMargin + CallStatusBarHeight, CallButtonLength / 2, CallButtonLength / 2);
            self.inviteUserButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.inviteUserButton.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.timeLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallInsideMargin + CallLabelHeight + CallStatusBarHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
            self.timeLabel.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.timeLabel.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusIncoming) {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin,
                           CallVerticalMargin * 2 + CallHeaderLength + CallInsideMargin * 2 + CallLabelHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusOutgoing) {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallStatusBarHeight,
                           self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        } else if (self.callSession.callStatus == JCallStatusIdle) {
            self.tipsLabel.frame =
            CGRectMake(CallHorizontalMargin,
                       self.view.frame.size.height - CallVerticalMargin - CallButtonLength * 3.5 -
                       CallInsideMargin * 5 - CallLabelHeight - CallExtraSpace,
                       self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        }  else {
            self.tipsLabel.frame =
                CGRectMake(CallHorizontalMargin,
                           self.view.frame.size.height - CallVerticalMargin - CallButtonLength * 3.5 -
                               CallInsideMargin * 5 - CallLabelHeight,
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

        self.speakerButton.hidden = YES;

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
        } else if (self.callSession.callStatus == JCallStatusConnected || self.callSession.callStatus == JCallStatusConnecting) {
            self.hangupButton.frame =
                CGRectMake((self.view.frame.size.width - CallButtonLength) / 2,
                           self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                           CallButtonLength);
            [self layoutTextUnderImageButton:self.hangupButton];
            self.hangupButton.hidden = NO;

            self.acceptButton.hidden = YES;
        }

        if (self.callSession.callStatus == JCallStatusConnected) {
            self.cameraCloseButton.frame =
                CGRectMake(self.view.frame.size.width - CallHorizontalMargin - CallButtonLength,
                           self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallExtraSpace, CallButtonLength,
                           CallButtonLength);
            [self layoutTextUnderImageButton:self.cameraCloseButton];
            self.cameraCloseButton.hidden = NO;
        } else if (self.callSession.callStatus != JCallStatusIdle) {
            self.cameraCloseButton.hidden = YES;
        }
    }
}

- (void)layoutTextUnderImageButton:(UIButton *)button {
    [button.titleLabel setFont:[UIFont systemFontOfSize:12]];
    [button setTitleColor:[UIColor whiteColor] forState:UIControlStateNormal];

    button.titleEdgeInsets = UIEdgeInsetsMake(0, -button.imageView.frame.size.width,
                                              -button.imageView.frame.size.height - CallInsideMargin, 0);
    button.imageEdgeInsets = UIEdgeInsetsMake(-button.titleLabel.intrinsicContentSize.height -CallInsideMargin, 0, 0,
                                              -button.titleLabel.intrinsicContentSize.width);
}

#pragma mark - JCallSessionDelegate
/// 通话已接通
- (void)callDidConnect {
    [self resetLayout];
}

/// 通话已结束
/// - Parameter finishReason: 结束原因
- (void)callDidFinish:(JCallFinishReason)finishReason {
    NSLog(@"call finish, reason is %ld", finishReason);
    [CallCenter.shared dismissCallViewController:self];
}

/// 用户被邀请
/// - Parameter userId: 被邀请的用户 id
- (void)usersDidInvite:(NSArray<NSString *> *)userIdList
             inviterId:(NSString *)inviterId {
    [self resetLayout];
}

/// 用户加入通话
/// - Parameter userId: 用户 id
- (void)usersDidConnect:(NSArray<NSString *> *)userIdList {
    
}

/// 用户退出通话
/// - Parameter userId: 用户 id
- (void)usersDidLeave:(NSArray<NSString *> *)userIdList {
    
}

/// 用户开启/关闭摄像头
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId {
    [self resetLayout];
}

/// 用户开启/关闭麦克风
/// - Parameters:
///   - enable: 是否开启
///   - userId: 用户 id
- (void)userMicrophoneDidChange:(BOOL)enable
                         userId:(NSString *)userId {
    
}

- (void)soundLevelDidUpdate:(NSDictionary<NSString *,NSNumber *> *)soundLevels {
    NSLog(@"soundLevelDidUpdate start");
    [soundLevels enumerateKeysAndObjectsUsingBlock:^(NSString * _Nonnull key, NSNumber * _Nonnull obj, BOOL * _Nonnull stop) {
        NSLog(@"userId is %@, level is %@", key, obj);
    }];
    
    NSLog(@"soundLevelDidUpdate end");
}

/// 通话中的错误回调
/// - Parameter errorCode: 错误码
- (void)errorDidOccur:(JCallErrorCode)errorCode {
    
}

#pragma mark - getter
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

- (UIView *)backgroundView {
    if (!_backgroundView) {
        _backgroundView = [[UIView alloc] initWithFrame:self.view.bounds];
        _backgroundView.backgroundColor = ColorFromRGB(0x262e42);
        [self.view addSubview:_backgroundView];
        _backgroundView.hidden = NO;
    }
    return _backgroundView;
}

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

- (UIButton *)inviteUserButton {
    if (!_inviteUserButton) {
        _inviteUserButton =
            [[UIButton alloc] initWithFrame:CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin / 2,
                                                       CallButtonLength, CallButtonLength)];
        [_inviteUserButton setImage:[UIImage imageNamed:@"callAdd"] forState:UIControlStateNormal];
        [_inviteUserButton setImage:[UIImage imageNamed:@"callAdd"]
                           forState:UIControlStateHighlighted];

        [_inviteUserButton addTarget:self
                              action:@selector(inviteUserButtonClicked)
                    forControlEvents:UIControlEventTouchUpInside];

        [self.view addSubview:_inviteUserButton];
        _inviteUserButton.hidden = YES;
    }
    return _inviteUserButton;
}

- (void)inviteUserButtonClicked {
}

- (UIButton *)minimizeButton {
    if (!_minimizeButton) {
        _minimizeButton =
            [[UIButton alloc] initWithFrame:CGRectMake(CallHorizontalMargin / 2, CallVerticalMargin / 2,
                                                       CallButtonLength, CallButtonLength)];
        [_minimizeButton setImage:[UIImage imageNamed:@"callMinimize"]
                         forState:UIControlStateNormal];
        [_minimizeButton setImage:[UIImage imageNamed:@"callMinimize"]
                         forState:UIControlStateHighlighted];
        [_minimizeButton setTitle:@"Minimize" forState:UIControlStateNormal];


//        [_minimizeButton addTarget:self
//                            action:@selector(minimizeButtonClicked)
//                  forControlEvents:UIControlEventTouchUpInside];


//        [self.view addSubview:_minimizeButton];
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
        
        [_muteButton setImage:[UIImage imageNamed:@"callMute"] forState:UIControlStateNormal];
        [_muteButton setImage:[UIImage imageNamed:@"callMuteHover"] forState:UIControlStateHighlighted];
        [_muteButton setImage:[UIImage imageNamed:@"callMuteHover"] forState:UIControlStateSelected];
        [_muteButton setTitle:@"Mute"
                     forState:UIControlStateNormal];
        [_muteButton setTitle:@"Unmute" forState:UIControlStateSelected];
//        [_muteButton setSelected:self.callSession.isMuted];
        [_muteButton addTarget:self action:@selector(muteButtonClicked:) forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_muteButton];
        _muteButton.hidden = YES;
    }
    return _muteButton;
}


- (void)muteButtonClicked:(UIButton *)sender {
    [self.callSession muteMicrophone:!sender.selected];
    [self.muteButton setSelected:!sender.selected];
    
//    [self didTapMuteButton];
//
//
//    [self.callSession setMuted:!self.callSession.isMuted];
//    [self.muteButton setSelected:self.callSession.isMuted];
}


- (UIButton *)speakerButton {
    if (!_speakerButton) {
        _speakerButton = [[UIButton alloc] init];
        [_speakerButton setImage:[UIImage imageNamed:@"callHandFree"]
                        forState:UIControlStateNormal];
        [_speakerButton setImage:[UIImage imageNamed:@"callHandFreeHover"]
                        forState:UIControlStateHighlighted];
        [_speakerButton setImage:[UIImage imageNamed:@"callHandFreeHover"]
                        forState:UIControlStateSelected];
        [_speakerButton setTitle:@"MuteSpeaker"
                        forState:UIControlStateNormal];
        [_speakerButton setTitle:@"UnmuteSpeaker"
                        forState:UIControlStateSelected];
//        [_speakerButton setSelected:self.callSession.speakerEnabled];


        [_speakerButton addTarget:self
                           action:@selector(speakerButtonClicked:)
                 forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_speakerButton];
        _speakerButton.hidden = YES;
    }
    return _speakerButton;
}


- (void)speakerButtonClicked:(UIButton *)sender {
    [self.callSession setSpeakerEnable:sender.isSelected];
//    [self.callSession muteSpeaker:!sender.selected];
    [self.speakerButton setSelected:!sender.selected];
//    [self didTapSpeakerButton];
//
//
//    [self.callSession setSpeakerEnabled:!self.callSession.speakerEnabled];
//    [self.speakerButton setSelected:self.callSession.speakerEnabled];
}


- (UIButton *)acceptButton {
    if (!_acceptButton) {
        _acceptButton = [[UIButton alloc] init];
        [_acceptButton setImage:[UIImage imageNamed:@"callAnswer"]
                       forState:UIControlStateNormal];
        [_acceptButton setImage:[UIImage imageNamed:@"callAnswerHover"]
                       forState:UIControlStateHighlighted];
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
    [self.callSession accept];
}


- (UIButton *)hangupButton {
    if (!_hangupButton) {
        _hangupButton = [[UIButton alloc] init];
        [_hangupButton setImage:[UIImage imageNamed:@"callHangup"]
                       forState:UIControlStateNormal];
        [_hangupButton setImage:[UIImage imageNamed:@"callHangupHover"]
                       forState:UIControlStateHighlighted];
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
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraClose"]
                                forState:UIControlStateNormal];
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraCloseHover"]
                                forState:UIControlStateHighlighted];
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraCloseHover"] forState:UIControlStateSelected];
            [_cameraCloseButton
                setTitle:@"Off"
                forState:UIControlStateNormal];
            [_cameraCloseButton
                setTitle:@"On"
             forState:UIControlStateSelected];
        } else {
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraClose"]
                                forState:UIControlStateNormal];
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraCloseHover"]
                                forState:UIControlStateHighlighted];
            [_cameraCloseButton setImage:[UIImage imageNamed:@"callCameraCloseHover"] forState:UIControlStateSelected];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video.png"]
//                                forState:UIControlStateNormal];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video.png"]
//                                forState:UIControlStateHighlighted];
//            [_cameraCloseButton setImage:[RCCallKitUtility imageFromVoIPBundle:@"voip/video_hover_.png"]
//                                forState:UIControlStateSelected];
            [_cameraCloseButton setTitle:@"Off"
                                forState:UIControlStateNormal];
            [_cameraCloseButton setTitle:@"On"
                                forState:UIControlStateSelected];
        }


//        [_cameraCloseButton setSelected:!self.callSession.cameraEnabled];
        [_cameraCloseButton addTarget:self
                               action:@selector(cameraCloseButtonClicked:)
                     forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_cameraCloseButton];
        _cameraCloseButton.hidden = YES;
    }
    return _cameraCloseButton;
}


- (void)cameraCloseButtonClicked:(UIButton *)sender {
    [self.callSession enableCamera:sender.isSelected];
    [self.cameraCloseButton setSelected:!sender.selected];

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
            [_cameraSwitchButton setImage:[UIImage imageNamed:@"callCameraSwitch"]
                                 forState:UIControlStateNormal];
            [_cameraSwitchButton setImage:[UIImage imageNamed:@"callCameraSwitchHover"]
                                 forState:UIControlStateHighlighted];
            [_cameraSwitchButton setImage:[UIImage imageNamed:@"callCameraSwitchHover"]
                                 forState:UIControlStateSelected];
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
                                action:@selector(cameraSwitchButtonClicked:)
                      forControlEvents:UIControlEventTouchUpInside];


        [self.view addSubview:_cameraSwitchButton];
        _cameraSwitchButton.hidden = YES;
    }
    return _cameraSwitchButton;
}


- (void)cameraSwitchButtonClicked:(UIButton *)sender {
    [self.callSession useFrontCamera:sender.isSelected];
    [self.cameraSwitchButton setSelected:!sender.selected];

//    [self didTapCameraSwitchButton];
//
//
//    [self.callSession switchCameraMode];
}

@end
