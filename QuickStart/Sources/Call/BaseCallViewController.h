//
//  BaseCallViewController.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseCallViewController : UIViewController
@property (nonatomic, strong) id<JCallSession> callSession;
@property(nonatomic, strong) UIView *backgroundView;

/*!
 蒙层View
 */
@property(nonatomic, strong) UIVisualEffectView *blurView;

/*!
 提示Label
 */
@property(nonatomic, strong) UILabel *tipsLabel;

/*!
 最小化Button
 */
@property(nonatomic, strong) UIButton *minimizeButton;

/*!
 加人Button
 */
@property(nonatomic, strong) UIButton *inviteUserButton;

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

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession;

- (void)resetLayout;
@end

NS_ASSUME_NONNULL_END
