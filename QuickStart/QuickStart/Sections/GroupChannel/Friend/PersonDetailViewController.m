//
//  PersonDetailViewController.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/7.
//

#import "PersonDetailViewController.h"
#import "PersonInfoView.h"
#import <Masonry/Masonry.h>
#import "OCConst.h"
#import <JuggleIM/JCallSession.h>

@interface PersonDetailViewController () <UITableViewDelegate, UITableViewDataSource>
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) UIView *contentView;
@property (nonatomic, strong) PersonInfoView *infoView;
@property (nonatomic, strong) UITableView *tableView;

@property (nonatomic, strong) UIButton *conversationButton;
@property (nonatomic, strong) UIButton *addFriendButton;
@property (nonatomic, strong) UIButton *audioCallButton;
@property (nonatomic, strong) UIButton *videoCallButton;
@property (nonatomic, assign) BOOL inBlacklist;

@property (nonatomic, assign) CGFloat tableViewHeight;
@end

@implementation PersonDetailViewController

- (void)viewDidLoad {
    [super viewDidLoad];
    
    self.view.backgroundColor = ColorFromRGB(0xf5f6f9);

    [self setupSubviews];
    [self configureNavigationBar];
    [self getUserInfoData];
}

- (void)configureNavigationBar {
    self.navigationItem.title = @"用户信息";
    UIBarButtonItem *leftButton = [SBUBarButtonItem backButtonWithTarget:self selector:@selector(leftBarButtonItemPressed:)];
    self.navigationItem.leftBarButtonItem = leftButton;
}

- (void)leftBarButtonItemPressed:(id)sender {
    if (self.navigationController && [self.navigationController.viewControllers.lastObject isEqual:self]) {
        [self.navigationController popViewControllerAnimated:YES];
    }
}

- (void)reloadView {
    if (self.user) {
        if ([self isCurrentUser]) {
            self.tableView.hidden = YES;
            self.conversationButton.hidden = YES;
            self.addFriendButton.hidden = YES;
            self.audioCallButton.hidden = YES;
            self.videoCallButton.hidden = YES;
            [self.conversationButton mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.top.equalTo(self.tableView.mas_bottom).offset(15);
                make.left.right.equalTo(self.contentView).inset(10);
                make.height.offset(43);
            }];
        } else if (self.user.isFriend) {
            self.tableView.hidden = YES;//NO;
            self.conversationButton.hidden = NO;
            self.addFriendButton.hidden = YES;
            self.audioCallButton.hidden = NO;
            self.videoCallButton.hidden = NO;
            [self.conversationButton mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.top.equalTo(self.tableView.mas_bottom).offset(15);
                make.left.right.equalTo(self.contentView).inset(10);
                make.height.offset(43);
            }];
            [self.audioCallButton mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.top.equalTo(self.conversationButton.mas_bottom).offset(15);
                make.left.right.equalTo(self.contentView).inset(10);
                make.height.offset(43);
            }];
            [self.videoCallButton mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.top.equalTo(self.audioCallButton.mas_bottom).offset(15);
                make.left.right.equalTo(self.contentView).inset(10);
                make.height.offset(43);
                make.bottom.equalTo(self.contentView).offset(-15);
            }];
        } else {
            self.tableView.hidden = YES;
            self.conversationButton.hidden = YES;
            self.addFriendButton.hidden = NO;
            self.audioCallButton.hidden = YES;
            self.videoCallButton.hidden = YES;
            [self.addFriendButton mas_remakeConstraints:^(MASConstraintMaker *make) {
                make.top.equalTo(self.tableView.mas_bottom).offset(15);
                make.left.right.equalTo(self.contentView).inset(10);
                make.height.offset(43);
            }];
        }
    } else {
        self.tableView.hidden = YES;
        self.conversationButton.hidden = YES;
        self.addFriendButton.hidden = YES;
        self.audioCallButton.hidden = YES;
        self.videoCallButton.hidden = YES;
    }
}

- (void)setupSubviews {
    [self.view addSubview:self.scrollView];
    [self.scrollView addSubview:self.contentView];
    [self.contentView addSubview:self.infoView];
    [self.contentView addSubview:self.tableView];
    [self.contentView addSubview:self.conversationButton];
    [self.contentView addSubview:self.addFriendButton];
    [self.contentView addSubview:self.audioCallButton];
    [self.contentView addSubview:self.videoCallButton];

    [self.scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.center.width.height.equalTo(self.view);
    }];

    [self.contentView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.edges.equalTo(self.scrollView);
        make.width.equalTo(self.scrollView);
    }];

    [self.infoView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.left.right.equalTo(self.contentView);
        make.height.offset(85);
    }];
    
    [self.tableView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.infoView.mas_bottom);
        make.left.right.equalTo(self.contentView);
//        make.height.offset(self.tableView.contentSize.height - 30);
    }];
    
    [self.conversationButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.tableView.mas_bottom).offset(15);
        make.left.right.equalTo(self.contentView).inset(10);
        make.height.offset(43);
    }];
    [self.addFriendButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.conversationButton.mas_bottom).offset(15);
        make.left.right.equalTo(self.contentView).inset(10);
        make.height.offset(43);
    }];
    [self.audioCallButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.addFriendButton.mas_bottom).offset(15);
        make.left.right.equalTo(self.contentView).inset(10);
        make.height.offset(43);
    }];
    [self.videoCallButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.audioCallButton.mas_bottom).offset(15);
        make.left.right.equalTo(self.contentView).inset(10);
        make.height.offset(43);
        make.bottom.equalTo(self.contentView).offset(-15);
    }];
    [self reloadView];
}

- (void)getUserInfoData {
    [HttpManager.shared getUserInfoWithUserId:self.userId completion:^(NSInteger code, JCUser * _Nullable user) {
        if (code == 0) {
            self.user = user;
            dispatch_async(dispatch_get_main_queue(), ^{
                [self.infoView setUserInfo:user];
                [self reloadView];
            });
        }
    }];
}

- (BOOL)isCurrentUser {
    return [self.userId isEqualToString:JIM.shared.currentUserId];
}

- (void)startChat {
    JConversation *conversation = [[JConversation alloc] initWithConversationType:JConversationTypePrivate conversationId:self.userId];
    JConversationInfo *info = [JIM.shared.conversationManager getConversationInfo:conversation];
    ChannelViewController *vc = [[ChannelViewController alloc] initWithConversationInfo:info];
    [self.navigationController pushViewController:vc animated:YES];
}

- (void)addFriend {
    [HttpManager.shared applyFriendWithUserId:self.userId completion:^(NSInteger code) {
        dispatch_async(dispatch_get_main_queue(), ^{
            if (code == 0) {
                UIAlertController *alertController = [UIAlertController alertControllerWithTitle:nil message:@"已发送好友申请" preferredStyle:UIAlertControllerStyleAlert];
                [alertController
                    addAction:[UIAlertAction actionWithTitle:@"确认"
                                                       style:UIAlertActionStyleDestructive
                                                     handler:^(UIAlertAction *_Nonnull action){
                }]];
                [self presentViewController:alertController animated:true completion:nil];
            } else {
                UIAlertController *alertController = [UIAlertController alertControllerWithTitle:nil message:@"发送好友申请失败" preferredStyle:UIAlertControllerStyleAlert];
                [alertController
                    addAction:[UIAlertAction actionWithTitle:@"确认"
                                                       style:UIAlertActionStyleDestructive
                                                     handler:^(UIAlertAction *_Nonnull action){
                }]];
                [self presentViewController:alertController animated:true completion:nil];
            }
        });
    }];
}

- (void)audioCall {
    id<JCallSession> session = [JIM.shared.callManager startSingleCall:self.userId
                                                             mediaType:JCallMediaTypeVoice
                                                                 extra:@"extra"
                                                              delegate:nil];
    [CallCenter.shared startSingleCall:session];
}

- (void)videoCall {
    id<JCallSession> session = [JIM.shared.callManager startSingleCall:self.userId
                                                             mediaType:JCallMediaTypeVideo
                                                              delegate:nil];
    [CallCenter.shared startSingleCall:session];
}

#pragma mark - UITableViewDelegate / UITableViewDataSource
- (NSInteger)tableView:(UITableView *)tableView numberOfRowsInSection:(NSInteger)section {
    return 1;
}

- (UITableViewCell *)tableView:(UITableView *)tableView cellForRowAtIndexPath:(NSIndexPath *)indexPath {
    BaseSettingTableViewCell *cell = [[BaseSettingTableViewCell alloc] init];
    [cell setCellStyle:DefaultStyle];
    cell.leftLabel.text = @"设置备注";
    return cell;
}

- (void)tableView:(UITableView *)tableView didSelectRowAtIndexPath:(NSIndexPath *)indexPath {
    
}

#pragma mark - Setter && Getter
- (UIScrollView *)scrollView {
    if (!_scrollView) {
        _scrollView = [[UIScrollView alloc] init];
    }
    return _scrollView;
}

- (UIView *)contentView {
    if (!_contentView) {
        _contentView = [[UIView alloc] init];
    }
    return _contentView;
}

- (PersonInfoView *)infoView {
    if (!_infoView) {
        _infoView = [[PersonInfoView alloc] init];
    }
    return _infoView;
}

- (UITableView *)tableView {
    if (!_tableView) {
        _tableView = [[UITableView alloc] init];
        _tableView.delegate = self;
        _tableView.dataSource = self;
        _tableView.scrollEnabled = NO;
        _tableView.backgroundColor = ColorFromRGB(0xf0f0f6);
    }
    return _tableView;
}

- (UIButton *)conversationButton {
    if (!_conversationButton) {
        _conversationButton = [[UIButton alloc] init];
        _conversationButton.backgroundColor = ColorFromRGB(0x0099ff);
        _conversationButton.translatesAutoresizingMaskIntoConstraints = NO;
        [_conversationButton setTitle:@"发起会话" forState:UIControlStateNormal];
        [_conversationButton addTarget:self action:@selector(startChat) forControlEvents:UIControlEventTouchUpInside];
        _conversationButton.layer.masksToBounds = YES;
        _conversationButton.layer.cornerRadius = 5.f;
        _conversationButton.layer.borderWidth = 0.5;
        _conversationButton.layer.borderColor = ColorFromRGB(0x0181dd).CGColor;
    }
    return _conversationButton;
}

- (UIButton *)addFriendButton {
    if (!_addFriendButton) {
        _addFriendButton = [[UIButton alloc] init];
        _addFriendButton.backgroundColor = ColorFromRGB(0x0099ff);
        _addFriendButton.translatesAutoresizingMaskIntoConstraints = NO;
        [_addFriendButton setTitle:@"添加好友" forState:UIControlStateNormal];
        [_addFriendButton addTarget:self action:@selector(addFriend) forControlEvents:UIControlEventTouchUpInside];
        _addFriendButton.layer.masksToBounds = YES;
        _addFriendButton.layer.cornerRadius = 5.f;
        _addFriendButton.layer.borderWidth = 0.5;
        _addFriendButton.layer.borderColor = ColorFromRGB(0x0181dd).CGColor;
    }
    return _addFriendButton;
}

- (UIButton *)audioCallButton {
    if (!_audioCallButton) {
        _audioCallButton = [[UIButton alloc] init];
        _audioCallButton.backgroundColor = ColorFromRGB(0xffffff);
        [_audioCallButton setTitle:@"语音通话" forState:UIControlStateNormal];
        [_audioCallButton setTitleColor:ColorFromRGB(0x000000) forState:UIControlStateNormal];
        [_audioCallButton addTarget:self action:@selector(audioCall) forControlEvents:UIControlEventTouchUpInside];
        _audioCallButton.translatesAutoresizingMaskIntoConstraints = NO;
        _audioCallButton.layer.masksToBounds = YES;
        _audioCallButton.layer.cornerRadius = 5.f;
    }
    return _audioCallButton;
}

- (UIButton *)videoCallButton {
    if (!_videoCallButton) {
        _videoCallButton = [[UIButton alloc] init];
        _videoCallButton.backgroundColor = ColorFromRGB(0xffffff);
        [_videoCallButton setTitle:@"视频通话" forState:UIControlStateNormal];
        [_videoCallButton setTitleColor:ColorFromRGB(0x000000) forState:UIControlStateNormal];
        [_videoCallButton addTarget:self action:@selector(videoCall) forControlEvents:UIControlEventTouchUpInside];
        _videoCallButton.translatesAutoresizingMaskIntoConstraints = NO;
        _videoCallButton.layer.masksToBounds = YES;
        _videoCallButton.layer.cornerRadius = 5.f;
    }
    return _videoCallButton;
}
@end
