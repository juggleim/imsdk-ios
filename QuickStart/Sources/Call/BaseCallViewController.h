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
 Overlay view
 */
@property(nonatomic, strong) UIVisualEffectView *blurView;

/*!
 Hint label
 */
@property(nonatomic, strong) UILabel *tipsLabel;

/*!
 Minimize button
 */
@property(nonatomic, strong) UIButton *minimizeButton;

/*!
 Add people button
 */
@property(nonatomic, strong) UIButton *inviteUserButton;

/*!
 Call duration label
 */
@property(nonatomic, strong) UILabel *timeLabel;

/*!
 Mute button
 */
@property(nonatomic, strong) UIButton *muteButton;


/*!
 Speaker button
 */
@property(nonatomic, strong) UIButton *speakerButton;


/*!
 Answer button
 */
@property(nonatomic, strong) UIButton *acceptButton;


/*!
 Hang up button
 */
@property(nonatomic, strong) UIButton *hangupButton;


/*!
 Camera off button
 */
@property(nonatomic, strong) UIButton *cameraCloseButton;


/*!
 Switch camera button
 */
@property(nonatomic, strong) UIButton *cameraSwitchButton;

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession;

- (void)resetLayout;
@end

NS_ASSUME_NONNULL_END
