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
    
    self.navigationItem.title = @"群公告";
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
    //发布中的时候显示转圈的进度
    MBProgressHUD *hud = [MBProgressHUD showHUDAddedTo:self.view animated:YES];
    hud.yOffset = -46.f;
    hud.minSize = CGSizeMake(120, 120);
    hud.color = [ColorFromRGB(0x343637) colorWithAlphaComponent:0.5];
    hud.margin = 0;
    [hud show:YES];
    //发布成功后，使用自定义图片
    NSString *txt = self.announcementContent.text;
    //去除收尾的空格
    txt = [txt stringByTrimmingCharactersInSet:[NSCharacterSet whitespaceCharacterSet]];
    //去除收尾的换行
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
                                   //显示成功的图片后返回
                                   [self.navigationController popViewControllerAnimated:YES];
                               });
            } else {
                [hud hide:YES];
                [NormalAlertView
                    showAlertWithMessage:@"群公告发送失败"
                           highlightText:nil
                               leftTitle:nil
                              rightTitle:@"确定"
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

//键盘将要弹出
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

//键盘将要隐藏
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
        [NormalAlertView showAlertWithMessage:@"退出本次编辑"
            highlightText:nil
            leftTitle:@"继续编辑"
            rightTitle:@"退出"
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
        [NormalAlertView showAlertWithMessage:@"确定清空群公告？"
            highlightText:@""
            leftTitle:@"取消"
            rightTitle:@"确定"
            cancel:^{

            }
            confirm:^{
                [self sendAnnouncement];
            }];
    } else {
        [NormalAlertView
            showAlertWithMessage:@"该公告会通知全部群成员，是否发布？"
            highlightText:@""
            leftTitle:@"取消"
            rightTitle:@"发布"
            cancel:^{

            }
            confirm:^{
                [self sendAnnouncement];
            }];
    }
}

- (void)setNaviItem {
    UIBarButtonItem *rightButton = [[UIBarButtonItem alloc] initWithTitle:@"完成"
                                                                    style:(UIBarButtonItemStylePlain)
                                                                   target:self
                                                                   action:@selector(clickRightBtn:)];
    self.navigationItem.rightBarButtonItem = rightButton;
    self.navigationItem.rightBarButtonItem.enabled = NO;

    UIBarButtonItem *leftButton = [[UIBarButtonItem alloc] initWithTitle:@"取消"
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
                stringWithFormat:@"发布时间：00:00"];
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
        [self.view showHUDMessage:@"字数已超限"];
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
        _announcementContent.myPlaceholder = @"请编辑群公告";
    }
    return _announcementContent;
}

- (UILabel *)guideLabel {
    if (!_guideLabel) {
        _guideLabel = [[UILabel alloc] init];
        _guideLabel.textColor = ColorFromRGB(0x939393);
        _guideLabel.font = [UIFont systemFontOfSize:13];
        _guideLabel.text = @"发布后将以系统消息发送群聊，全员成员可见";
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
