//
//  EditUserNameViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/22.
//

#import "EditUserNameViewController.h"
#import "OCConst.h"
#import "QuickStart-Swift.h"
#import <MBProgressHUD.h>

@interface EditUserNameViewController ()
@property (nonatomic, strong) UIView *bgView;
@property (nonatomic, strong) UITextField *userNameTextField;
@property (nonatomic, strong) NSDictionary *subViews;
@property (nonatomic, strong) UIBarButtonItem *rightButton;
@property (nonatomic, strong) MBProgressHUD *hud;
@end

@implementation EditUserNameViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    
    [self initUI];
    [self configureNavigationBar];
}

- (void)initUI {
    self.bgView = [UIView new];
    self.bgView.backgroundColor = ColorFromRGB(0xffffff);
    self.bgView.translatesAutoresizingMaskIntoConstraints = NO;
    [self.view addSubview:self.bgView];

    UITapGestureRecognizer *clickbgView =
        [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(beginEditNickname)];
    [self.bgView addGestureRecognizer:clickbgView];

    self.userNameTextField = [UITextField new];
    self.userNameTextField.borderStyle = UITextBorderStyleNone;
    self.userNameTextField.clearButtonMode = UITextFieldViewModeAlways;
    self.userNameTextField.font = [UIFont systemFontOfSize:16.f];
    self.userNameTextField.textColor = ColorFromRGB(0x000000);
    self.userNameTextField.translatesAutoresizingMaskIntoConstraints = NO;
    self.userNameTextField.text = ProfileManager.shared.currentUserInfo.userName;
    [self.userNameTextField addTarget:self
                               action:@selector(textFieldDidChange:)
                     forControlEvents:UIControlEventEditingChanged];
    [self.bgView addSubview:self.userNameTextField];

    self.subViews = NSDictionaryOfVariableBindings(_bgView, _userNameTextField);

    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|-15-[_bgView(44)]"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:self.subViews]];

    [self.view addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_bgView]|"
                                                                      options:0
                                                                      metrics:nil
                                                                        views:self.subViews]];

    [self.bgView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-9-[_userNameTextField]-3-|"
                                                                        options:0
                                                                        metrics:nil
                                                                          views:self.subViews]];

    [self.bgView addConstraint:[NSLayoutConstraint constraintWithItem:_userNameTextField
                                                            attribute:NSLayoutAttributeCenterY
                                                            relatedBy:NSLayoutRelationEqual
                                                               toItem:self.bgView
                                                            attribute:NSLayoutAttributeCenterY
                                                           multiplier:1
                                                             constant:0]];
}

- (void)configureNavigationBar {
    self.navigationItem.title = @"修改昵称";
    UIBarButtonItem *leftButton = [SBUBarButtonItem backButtonWithTarget:self selector:@selector(leftBarButtonItemPressed:)];
    self.navigationItem.leftBarButtonItem = leftButton;
    self.rightButton = [[UIBarButtonItem alloc] initWithTitle:@"保存"
                                                                    style:(UIBarButtonItemStylePlain)
                                                                   target:self
                                                                   action:@selector(clickRightBtn)];
}

- (void)leftBarButtonItemPressed:(id)sender {
    if (self.navigationController && [self.navigationController.viewControllers.lastObject isEqual:self]) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)clickRightBtn {
    [self.hud showAnimated:YES];
    NSString *name = self.userNameTextField.text;
    [HttpManager.shared updateUserInfoWithUserId:ProfileManager.shared.currentUserInfo.userId name:name portrait:nil completion:^(NSInteger code) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (code == 0) {
                ProfileManager.shared.currentUserInfo.userName = name;
                [self.navigationController popViewControllerAnimated:YES];
            }
        });
    }];
}

- (void)beginEditNickname {
    [self.userNameTextField becomeFirstResponder];
}

- (void)textFieldDidChange:(UITextField *)textField {
    NSString *toBeString = textField.text;
    if (![toBeString isEqualToString:ProfileManager.shared.currentUserInfo.userName]) {
        self.navigationItem.rightBarButtonItem = self.rightButton;
    } else {
        self.navigationItem.rightBarButtonItem = nil;
    }
}


- (MBProgressHUD *)hud {
    if (!_hud) {
        MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
        hud.label.text = @"修改中...";
        _hud = hud;
    }
    return _hud;
}

@end
