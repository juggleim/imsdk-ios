//
//  GroupAnnouncementViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/25.
//

#import "GroupAnnouncementViewController.h"
#import <MBProgressHUD/MBProgressHUD.h>
#import "UITextViewAndPlaceholder.h"
#import "UIView+MBProgressHUD.h"
#import "OCConst.h"
#import "QuickStart-Swift.h"
#import "NormalAlertView.h"
#import "Masonry.h"

#define MaxLength 100

@interface GroupAnnouncementViewController () <UITextViewDelegate>
@property (nonatomic, strong) UITextViewAndPlaceholder *announcementContent;
@property (nonatomic, assign) CGFloat textViewOriginalHeight;
@property (nonatomic, strong) UILabel *guideLabel;
@property (nonatomic, strong) UILabel *updateTime;
@property (nonatomic, copy) NSString *announcementString;
@end

@implementation GroupAnnouncementViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.navigationItem.title = NSLocalizedString(@"Group Announcement", @"");
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    [self setNaviItem];
    [self registerNotification];
    [self setData];
    [self setupView];

    UITapGestureRecognizer *tap = [[UITapGestureRecognizer alloc] initWithTarget:self action:@selector(didTap)];
    [self.view addGestureRecognizer:tap];
}

- (void)dealloc {
    NSLog(@"%s", __func__);
    [[NSNotificationCenter defaultCenter] removeObserver:self];
}

#pragma mark - helper
- (void)sendAnnouncement {
    //Show a spinning progress indicator while publishing
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.yOffset = -46.f;
    hud.minSize = CGSizeMake(120, 120);
    hud.color = [ColorFromRGB(0x343637) colorWithAlphaComponent:0.5];
    hud.margin = 0;
    [hud show:YES];
    //Use a custom image after successful publishing
    NSString *txt = self.announcementContent.text;
    //trim leading and trailing spaces
    txt = [txt stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    //trim leading and trailing newlines
    txt = [txt stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceAndNewlineCharacterSet]];
    
    [HttpManager.shared setGroupAnnouncementWithGroupId:self.groupId content:txt completion:^(NSInteger code) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (code == 0) {
                hud.mode = MBProgressHUDModeCustomView;
                UIImageView *customView =
                    [[UIImageView alloc] initWithImage:[UIImage imageNamed:@"complete"]];
                customView.frame = CGRectMake(0, 0, 80, 80);
                hud.customView = customView;
                dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)),
                               dispatch_get_main_queue(), ^{
                                   //Return after showing the success image
                                   [self.navigationController popViewControllerAnimated:YES];
                               });
            } else {
                [hud hide:YES];
                [NormalAlertView
                    showAlertWithMessage:NSLocalizedString(@"Failed to send group announcement", @"")
                           highlightText:nil
                               leftTitle:nil
                              rightTitle:NSLocalizedString(@"Confirm", @"")
                                  cancel:nil
                                 confirm:nil];
            }
        });
    }];
}

- (void)registerNotification {
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillShow:)
                                                 name:UIKeyboardWillShowNotification
                                               object:nil];
    [[NSNotificationCenter defaultCenter] addObserver:self
                                             selector:@selector(keyboardWillHide:)
                                                 name:UIKeyboardWillHideNotification
                                               object:nil];
}

//Keyboard will show
- (void)keyboardWillShow:(NSNotification *)aNotification {
    CGRect keyboardRect = [[[aNotification userInfo] objectForKey:UIKeyboardBoundsUserInfoKey] CGRectValue];
    [self.announcementContent mas_remakeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(14);
        make.right.equalTo(self.view).offset(-14);
        make.top.equalTo(self.updateTime.mas_bottom).offset(5);
        if (@available(iOS 11.0, *)) {
            make.bottom.equalTo(self.view.mas_safeAreaLayoutGuideBottom).offset(-keyboardRect.size.height);
        } else {
            make.bottom.equalTo(self.view).offset(-keyboardRect.size.height);
        }
    }];
}

//Keyboard will hide
- (void)keyboardWillHide:(NSNotification *)aNotification {
    [self.announcementContent mas_remakeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(14);
        make.right.equalTo(self.view).offset(-14);
        make.top.equalTo(self.updateTime.mas_bottom).offset(5);
        if (@available(iOS 11.0, *)) {
            make.bottom.equalTo(self.view.mas_safeAreaLayoutGuideBottom);
        } else {
            make.bottom.equalTo(self.view);
        }
    }];
}

- (void)clickLeftBtn:(id)sender {
    [self.announcementContent resignFirstResponder];
    if (self.announcementString && ![self.announcementContent.text isEqualToString:self.announcementString]) {
        [NormalAlertView showAlertWithMessage:NSLocalizedString(@"Exit this edit", @"")
            highlightText:nil
            leftTitle:NSLocalizedString(@"Continue editing", @"")
            rightTitle:NSLocalizedString(@"Exit", @"")
            cancel:^{

            }
            confirm:^{
                [self.navigationController popViewControllerAnimated:YES];
            }];
    } else {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)clickRightBtn:(id)sender {
    [self.announcementContent resignFirstResponder];
    if (self.announcementString && self.announcementContent.text.length == 0) {
        [NormalAlertView showAlertWithMessage:NSLocalizedString(@"Clear the group announcement?", @"")
            highlightText:@""
            leftTitle:NSLocalizedString(@"Cancel", @"")
            rightTitle:NSLocalizedString(@"Confirm", @"")
            cancel:^{

            }
            confirm:^{
                [self sendAnnouncement];
            }];
    } else {
        [NormalAlertView
            showAlertWithMessage:NSLocalizedString(@"This announcement will notify all group members. Publish it?", @"")
            highlightText:@""
            leftTitle:NSLocalizedString(@"Cancel", @"")
            rightTitle:NSLocalizedString(@"Publish", @"")
            cancel:^{

            }
            confirm:^{
                [self sendAnnouncement];
            }];
    }
}

- (void)setNaviItem {
    UIBarButtonItem *rightButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Done", @"")
                                                                    style:(UIBarButtonItemStylePlain)
                                                                   target:self
                                                                   action:@selector(clickRightBtn:)];
    self.navigationItem.rightBarButtonItem = rightButton;
    self.navigationItem.rightBarButtonItem.enabled = NO;

    UIBarButtonItem *leftButton = [[UIBarButtonItem alloc] initWithTitle:NSLocalizedString(@"Cancel", @"")
                                                                   style:(UIBarButtonItemStylePlain)
                                                                  target:self
                                                                  action:@selector(clickLeftBtn:)];
    self.navigationItem.leftBarButtonItem = leftButton;
}

- (void)didTap {
    [self.announcementContent resignFirstResponder];
}

- (void)setData {
    __weak typeof(self) weakSelf = self;
    [HttpManager.shared getGroupAnnouncementWithGroupId:self.groupId completion:^(NSInteger code, NSString * _Nonnull content) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (code != 0) {
                return;
            }
            weakSelf.announcementString = content;
            if (content.length > 0) {
                weakSelf.announcementContent.text = content;
            }
            weakSelf.updateTime.text = [NSString
                stringWithFormat:NSLocalizedString(@"Publish time: %@", @""), @"00:00"];
        });
    }];
}

- (void)setupView {
    [self.view addSubview:self.guideLabel];
    [self.view addSubview:self.updateTime];
    [self.view addSubview:self.announcementContent];
    [self.guideLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(18);
        make.right.equalTo(self.view).offset(-18);
        make.top.equalTo(self.view).offset(12);
    }];
    [self.updateTime mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(18);
        make.right.equalTo(self.view).offset(-18);
        make.top.equalTo(self.guideLabel.mas_bottom).offset(5);
    }];
    [self.announcementContent mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(self.view).offset(14);
        make.right.equalTo(self.view).offset(-14);
        make.top.equalTo(self.updateTime.mas_bottom).offset(5);
        if (@available(iOS 11.0, *)) {
            make.bottom.equalTo(self.view.mas_safeAreaLayoutGuideBottom);
        } else {
            make.bottom.equalTo(self.view);
        }
    }];
    [self.view updateConstraintsIfNeeded];
    [self.view layoutIfNeeded];
    self.textViewOriginalHeight = self.announcementContent.frame.size.height;
}

#pragma mark - UITextViewDelegate
- (void)textViewDidChange:(UITextView *)textView {
    if (textView.text.length > MaxLength) {
        textView.text = [textView.text substringToIndex:MaxLength];
        [self.view showHUDMessage:NSLocalizedString(@"Character limit exceeded", @"")];
    }
    if ([textView.text isEqualToString:self.announcementString]) {
        self.navigationItem.rightBarButtonItem.enabled = NO;
    } else {
        self.navigationItem.rightBarButtonItem.enabled = YES;
    }
}


#pragma mark - geter & setter
- (UITextViewAndPlaceholder *)announcementContent {
    if (!_announcementContent) {
        _announcementContent = [[UITextViewAndPlaceholder alloc] initWithFrame:CGRectZero];
        _announcementContent.delegate = self;
        _announcementContent.font = [UIFont systemFontOfSize:14.f];
        _announcementContent.textColor = [UIColor blackColor];
        _announcementContent.myPlaceholder = NSLocalizedString(@"Please edit the group announcement", @"");
    }
    return _announcementContent;
}

- (UILabel *)guideLabel {
    if (!_guideLabel) {
        _guideLabel = [[UILabel alloc] init];
        _guideLabel.textColor = ColorFromRGB(0x939393);
        _guideLabel.font = [UIFont systemFontOfSize:13];
        _guideLabel.text = NSLocalizedString(@"After publishing, it will be sent to the group chat as a system message and visible to all members", @"");
    }
    return _guideLabel;
}

- (UILabel *)updateTime {
    if (!_updateTime) {
        _updateTime = [[UILabel alloc] init];
        _updateTime.textColor = ColorFromRGB(0x939393);
        _updateTime.font = [UIFont systemFontOfSize:13];
    }
    return _updateTime;
}

@end
