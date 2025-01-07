//
//  GroupJoinViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/7.
//

#import "GroupJoinViewController.h"
#import <SDWebImage/UIImageView+WebCache.h>
#import <Masonry/Masonry.h>
#import "OCConst.h"

@interface GroupJoinViewController ()
@property (nonatomic, strong) UIView *bgView;
@property (nonatomic, strong) UIImageView *portraitImageView;
@property (nonatomic, strong) UILabel *nameLabel;
@property (nonatomic, strong) UILabel *infoLabel;
@property (nonatomic, strong) UIButton *joinButton;
@end

@implementation GroupJoinViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);
    [self setNaviItem];
    [self addSubViews];
    [self setGroupInfo];
}

- (void)setGroupInfo {
    self.infoLabel.text = [NSString stringWithFormat:@"群内已有 %ld 人", self.group.memberCount];

    self.nameLabel.text = self.group.groupName;
    if ([self.group.portrait isEqualToString:@""]) {
        [self.portraitImageView sd_setImageWithURL:[NSURL URLWithString:self.group.portrait]
                                  placeholderImage:[UIImage imageNamed:@"iconMembers"]];
    }
}

- (void)setNaviItem {
    self.navigationItem.title = @"群信息";
    UIBarButtonItem *left = [SBUBarButtonItem backButtonWithTarget:self selector:@selector(leftBarButtonItemPressed)];
    self.navigationItem.leftBarButtonItem = left;
}

- (void)leftBarButtonItemPressed {
    if (self.navigationController && [self.navigationController.viewControllers.lastObject isEqual:self]) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)addSubViews {
    [self.view addSubview:self.bgView];
    [self.view addSubview:self.joinButton];
    [self.bgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.width.equalTo(self.view);
        make.height.offset(211);
        make.top.equalTo(self.view).offset(0);
    }];
    [self.joinButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.view);
        make.height.offset(44);
        make.left.equalTo(self.view).offset(32);
        make.top.equalTo(self.bgView.mas_bottom).offset(30);
    }];

    [self.bgView addSubview:self.portraitImageView];
    [self.bgView addSubview:self.nameLabel];
    [self.bgView addSubview:self.infoLabel];

    [self.portraitImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.bgView);
        make.top.equalTo(self.bgView).offset(60);
        make.width.height.offset(70);
    }];

    [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.bgView);
        make.top.equalTo(self.portraitImageView.mas_bottom).offset(9);
        make.height.offset(28);
        make.width.equalTo(self.bgView);
    }];

    [self.infoLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.centerX.equalTo(self.bgView);
        make.top.equalTo(self.nameLabel.mas_bottom).offset(2);
        make.height.offset(20);
        make.width.equalTo(self.bgView);
    }];
}

- (void)didClickJoinAction {
    __weak typeof(self) weakSelf = self;
    [HttpManager.shared groupApplyWithGroupId:self.group.groupId completion:^(NSInteger code) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (code == 0) {
                [weakSelf pushChatVC];
            } else {
                UIAlertController *alertController =
                    [UIAlertController alertControllerWithTitle:nil message:@"加入群组失败" preferredStyle:UIAlertControllerStyleAlert];
                [alertController
                    addAction:[UIAlertAction actionWithTitle:@"确认"
                                                       style:UIAlertActionStyleDestructive
                                                     handler:^(UIAlertAction *_Nonnull action){
                    [self.navigationController popViewControllerAnimated:true];
                                                     }]];
                [self presentViewController:alertController animated:true completion:nil];
            }
        });
    }];
}

- (void)pushChatVC {
    dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
        JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypeGroup conversationId:self.group.groupId];
        JConversationInfo *info = [JIM.shared.conversationManager getConversationInfo:conversation];
        ChannelViewController *vc = [[ChannelViewController alloc] initWithConversationInfo:info];
        [self.navigationController pushViewController:vc animated:YES];
    });
}

#pragma mark - getter
- (UIView *)bgView {
    if (!_bgView) {
        _bgView = [[UIView alloc] init];
        _bgView.backgroundColor = ColorFromRGB(0xffffff);
    }
    return _bgView;
}

- (UIImageView *)portraitImageView {
    if (!_portraitImageView) {
        _portraitImageView = [[UIImageView alloc] init];
        _portraitImageView.layer.masksToBounds = YES;
        _portraitImageView.layer.cornerRadius = 4;
    }
    return _portraitImageView;
}

- (UILabel *)nameLabel {
    if (!_nameLabel) {
        _nameLabel = [[UILabel alloc] init];
        _nameLabel.textColor = ColorFromRGB(0x262626);
        _nameLabel.font = [UIFont systemFontOfSize:20];
        _nameLabel.textAlignment = NSTextAlignmentCenter;
    }
    return _nameLabel;
}

- (UILabel *)infoLabel {
    if (!_infoLabel) {
        _infoLabel = [[UILabel alloc] init];
        _infoLabel.textColor = ColorFromRGB(0x939393);
        _infoLabel.font = [UIFont systemFontOfSize:14];
        _infoLabel.textAlignment = NSTextAlignmentCenter;
    }
    return _infoLabel;
}

- (UIButton *)joinButton {
    if (!_joinButton) {
        _joinButton = [[UIButton alloc] init];
        _joinButton.backgroundColor = ColorFromRGB(0x368ae8);
        [_joinButton setTitleColor:ColorFromRGB(0xffffff) forState:(UIControlStateNormal)];
        _joinButton.titleLabel.font = [UIFont systemFontOfSize:17];
        [_joinButton setTitle:@"加入该群" forState:(UIControlStateNormal)];
        [_joinButton addTarget:self
                        action:@selector(didClickJoinAction)
              forControlEvents:(UIControlEventTouchUpInside)];
    }
    return _joinButton;
}
@end
