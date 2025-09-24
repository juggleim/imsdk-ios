//
//  MultiVideoCallViewController.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

#import "MultiVideoCallViewController.h"
#import "MultiVideoCallUserCollectionLayout.h"
#import "MultiAudioCallUserCollectionLayout.h"
#import "MultiCallUserCell.h"
#import "QuickStart-Swift.h"
#import "CallTheme.h"

@interface MultiVideoCallViewController () <GroupMemberSelectVCDelegate, UICollectionViewDataSource, UICollectionViewDelegate>
@property (nonatomic, copy) NSString *groupId;

@property(nonatomic, strong) JCallMember *mainModel;
@property(nonatomic, strong) NSMutableArray <JCallMember *> *subUserModelList;

@property(nonatomic, assign) BOOL isFullScreen;
@end

@implementation MultiVideoCallViewController

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession groupId:(NSString *)groupId {
    self = [super initWithIncomingCall:callSession];
    if (self) {
        self.groupId = groupId;
    }
    return self;
}

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession groupId:(NSString *)groupId {
    self = [super initWithOutgoingCall:callSession];
    if (self) {
        self.groupId = groupId;
    }
    return self;
}

- (void)viewDidLoad {
    [super viewDidLoad];
    
    [self initAllUserModel];
    
    self.backgroundView.userInteractionEnabled = YES;
    [self.backgroundView
     addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self
                                                                  action:@selector(backgroundViewClicked)]];
    
    self.userCollectionView.userInteractionEnabled = YES;
    [self.userCollectionView
     addGestureRecognizer:[[UITapGestureRecognizer alloc] initWithTarget:self
                                                                  action:@selector(backgroundViewClicked)]];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
    self.isFullScreen = NO;
}

- (void)initAllUserModel {
    if (self.callSession.callStatus == JCallStatusIncoming) {
        JCallMember *inviterMember = [self getInviterCallMember];
        if (inviterMember && ![self inviterHasHangup]) {
            self.mainModel = inviterMember;
        } else {
            self.mainModel = self.callSession.members[0];
        }
        [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
        self.subUserModelList = [[NSMutableArray alloc] init];
        for (JCallMember *callMember in self.callSession.members) {
            if (![callMember.userInfo.userId isEqualToString:self.mainModel.userInfo.userId]) {
                [self.subUserModelList addObject:callMember];
            }
        }
        JCallMember *current = self.callSession.currentCallMember;
        [self.subUserModelList addObject:current];
    } else if (self.callSession.callStatus == JCallStatusOutgoing) {
        self.mainModel = self.callSession.currentCallMember;
        [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
        self.subUserModelList = [self.callSession.members mutableCopy];
    } else if (self.callSession.callStatus == JCallStatusConnected || self.callSession.callStatus == JCallStatusConnecting) {
        if ([self.callSession.inviterId isEqualToString:JIM.shared.currentUserId] || [self inviterHasHangup]) {
            self.mainModel = self.callSession.currentCallMember;
            [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
            self.subUserModelList = [self.callSession.members mutableCopy];
        } else {
            self.mainModel = [self getInviterCallMember];
            [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
            self.subUserModelList = [[NSMutableArray alloc] init];
            for (JCallMember *callMember in self.callSession.members) {
                if (![callMember.userInfo.userId isEqualToString:self.mainModel.userInfo.userId]) {
                    [self.subUserModelList addObject:callMember];
                }
            }
            JCallMember *current = self.callSession.currentCallMember;
            [self.subUserModelList addObject:current];
        }
    }
}

- (BOOL)inviterHasHangup {
    if ([self.callSession.inviterId isEqualToString:JIM.shared.currentUserId]) {
        if (self.callSession.callStatus == JCallStatusIdle) {
            return YES;
        } else {
            return NO;
        }
    } else {
        for (JCallMember *member in self.callSession.members) {
            if ([member.userInfo.userId isEqualToString:self.callSession.inviterId] &&
                member.callStatus != JCallStatusIdle) {
                return NO;
            }
        }
        return YES;
    }
}

- (JCallMember *)getInviterCallMember {
    for (JCallMember *member in self.callSession.members) {
        if ([member.userInfo.userId isEqualToString:self.callSession.inviterId]) {
            return member;
        }
    }
    return nil;
}

- (JCallMember *)getCallMember:(NSString *)userId {
    for (JCallMember *member in self.callSession.members) {
        if ([member.userInfo.userId isEqualToString:userId]) {
            return member;
        }
    }
    return nil;
}

- (NSArray *)getAllUserIdInSubUserModel {
    NSMutableArray *userIdList = [[NSMutableArray alloc] init];
    for (JCallMember *member in self.subUserModelList) {
        [userIdList addObject:member.userInfo.userId];
    }
    if (userIdList.count > 0) {
        return [userIdList copy];
    } else {
        return nil;
    }
}

- (JCallMember *)getModelInSubUserModelList:(NSString *)userId {
    for (JCallMember *model in self.subUserModelList) {
        if ([model.userInfo.userId isEqualToString:userId]) {
            return model;
        }
    }
    return nil;
}

- (void)updateSubUserLayout:(JCallMember *)member {
    if (member) {
        NSInteger index = [self.subUserModelList indexOfObject:member];
        if (index != NSNotFound) {
            NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
            [self.userCollectionView reloadItemsAtIndexPaths:@[ indexPath ]];
        }
    }
}

- (void)updateAllSubUserLayout {
    [self.userCollectionView reloadData];
}

- (void)addSubUserModel:(JCallMember *)model {
    if (model.userInfo.userId && ![[self getAllUserIdInSubUserModel] containsObject:model.userInfo.userId]) {
        NSInteger index = self.subUserModelList.count;
        NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
        [self.subUserModelList addObject:model];
        [self.userCollectionView insertItemsAtIndexPaths:[NSArray arrayWithObject:indexPath]];
    }
}

- (void)removeSubUserModel:(JCallMember *)model {
    if (model) {
        NSInteger index = [self.subUserModelList indexOfObject:model];
        if (index != NSNotFound) {
            NSIndexPath *indexPath = [NSIndexPath indexPathForItem:index inSection:0];
            [self.subUserModelList removeObject:model];
            [self.userCollectionView deleteItemsAtIndexPaths:[NSArray arrayWithObject:indexPath]];
        }
    }
}

- (void)backgroundViewClicked {
    if (self.callSession.callStatus == JCallStatusConnected) {
        self.isFullScreen = !self.isFullScreen;
        
        [self resetLayout];
    }
}

- (UIImageView *)inviterPortraitView {
    if (!_inviterPortraitView) {
        _inviterPortraitView = [[UIImageView alloc] init];
        
        [self.view addSubview:_inviterPortraitView];
        _inviterPortraitView.hidden = YES;
        //        [_inviterPortraitView setPlaceholderImage:[RCCallKitUtility getDefaultPortraitImage]];
        _inviterPortraitView.layer.cornerRadius = 4;
        _inviterPortraitView.layer.masksToBounds = YES;
    }
    return _inviterPortraitView;
}

- (UILabel *)inviterNameLabel {
    if (!_inviterNameLabel) {
        _inviterNameLabel = [[UILabel alloc] init];
        _inviterNameLabel.backgroundColor = [UIColor clearColor];
        _inviterNameLabel.textColor = [UIColor whiteColor];
        _inviterNameLabel.font = [UIFont systemFontOfSize:18];
        _inviterNameLabel.textAlignment = NSTextAlignmentCenter;
        
        [self.view addSubview:_inviterNameLabel];
        _inviterNameLabel.hidden = YES;
    }
    return _inviterNameLabel;
}

- (UILabel *)mainNameLabel {
    if (!_mainNameLabel) {
        _mainNameLabel = [[UILabel alloc] init];
        _mainNameLabel.backgroundColor = [UIColor clearColor];
        _mainNameLabel.textColor = [UIColor whiteColor];
        _mainNameLabel.font = [UIFont systemFontOfSize:18];
        _mainNameLabel.textAlignment = NSTextAlignmentCenter;
        _mainNameLabel.text = self.mainModel.userInfo.userName;
        
        [self.view addSubview:_mainNameLabel];
        _mainNameLabel.hidden = YES;
    }
    return _mainNameLabel;
}

- (UILabel *)userCollectionTitleLabel {
    if (!_userCollectionTitleLabel) {
        _userCollectionTitleLabel = [[UILabel alloc] init];
        _userCollectionTitleLabel.backgroundColor = [UIColor clearColor];
        _userCollectionTitleLabel.textColor = [UIColor whiteColor];
        _userCollectionTitleLabel.font = [UIFont systemFontOfSize:16];
        _userCollectionTitleLabel.textAlignment = NSTextAlignmentCenter;
        _userCollectionTitleLabel.text =
        @"Members";
        
        [self.view addSubview:_userCollectionTitleLabel];
        _userCollectionTitleLabel.hidden = YES;
    }
    return _userCollectionTitleLabel;
}

- (UICollectionView *)userCollectionView {
    if (!_userCollectionView) {
        if (self.userCollectionViewLayout) {
            _userCollectionView =
            [[UICollectionView alloc] initWithFrame:CGRectZero collectionViewLayout:self.userCollectionViewLayout];
        } else {
            MultiVideoCallUserCollectionLayout *userCollectionViewLayout =
            [[MultiVideoCallUserCollectionLayout alloc] initWithItemMargin:5];
            _userCollectionView =
            [[UICollectionView alloc] initWithFrame:CGRectZero collectionViewLayout:userCollectionViewLayout];
        }
        _userCollectionView.backgroundColor = [UIColor clearColor];
        
        [self.view addSubview:_userCollectionView];
        _userCollectionView.hidden = YES;
        _userCollectionView.dataSource = self;
        _userCollectionView.delegate = self;
        [_userCollectionView registerClass:[MultiCallUserCell class]
                forCellWithReuseIdentifier:@"MultiVideoCellReuseID"];
    }
    return _userCollectionView;
}

- (void)inviteUserButtonClicked {
    if (self.callSession.isMultiCall) {
        [self inviteNewUser];
    }
}

- (void)inviteNewUser {
    NSMutableArray *existUserIdList = [[NSMutableArray alloc] init];
    for (JCallMember *member in self.callSession.members) {
        [existUserIdList addObject:member.userInfo.userId];
    }
    
    GroupMemberSelectViewController *vc = [[GroupMemberSelectViewController alloc] init];
    vc.type = GroupMemberSelectTypeVideoCall;
    vc.groupId = self.groupId;
    vc.existedUsers = existUserIdList;
    vc.delegate = self;
    
    UINavigationController *root = [[UINavigationController alloc] initWithRootViewController:vc];
    
    [self presentViewController:root animated:true completion:nil];
}

- (void)resetLayout {
    [super resetLayout];
    
    if (self.callSession.callStatus == JCallStatusIncoming) {
        //        [self.inviterPortraitView setImageURL:[NSURL URLWithString:self.mainModel.userInfo.portraitUri]];
        self.inviterPortraitView.frame = CGRectMake((self.view.frame.size.width - CallHeaderLength) / 2,
                                                    CallVerticalMargin * 2, CallHeaderLength, CallHeaderLength);
        self.inviterPortraitView.hidden = NO;
        
        self.inviterNameLabel.text = self.mainModel.userInfo.userName;
        self.inviterNameLabel.frame =
        CGRectMake(CallHorizontalMargin, CallVerticalMargin * 2 + CallHeaderLength + CallInsideMargin,
                   self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        self.inviterNameLabel.hidden = NO;
    } else {
        self.inviterNameLabel.hidden = YES;
        self.inviterPortraitView.hidden = YES;
    }
    
    if (self.callSession.callStatus == JCallStatusConnected) {
        self.mainNameLabel.frame =
        CGRectMake(CallHorizontalMargin, CallVerticalMargin + CallStatusBarHeight,
                   self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        self.mainNameLabel.hidden = NO;
    } else {
        self.mainNameLabel.hidden = YES;
    }
    
    CGFloat titleY = MAX(CallVerticalMargin * 2 + CallHeaderLength + CallInsideMargin * 3 + CallLabelHeight * 2,
                         (self.view.frame.size.height - CallLabelHeight) / 2);
    if (self.callSession.callStatus == JCallStatusIncoming) {
        self.userCollectionTitleLabel.frame = CGRectMake(
                                                         CallHorizontalMargin, titleY - CallExtraSpace, self.view.frame.size.width - CallHorizontalMargin * 2, CallLabelHeight);
        self.userCollectionTitleLabel.hidden = NO;
    } else {
        self.userCollectionTitleLabel.hidden = YES;
    }
    
    if (self.callSession.callStatus == JCallStatusIncoming) {
        self.userCollectionView.frame = CGRectMake(
                                                   CallHorizontalMargin * 2.5, titleY + CallLabelHeight + CallInsideMargin - CallExtraSpace,
                                                   self.view.frame.size.width - CallHorizontalMargin * 5,
                                                   self.view.frame.size.height - CallVerticalMargin - CallButtonLength - CallInsideMargin * 4 -
                                                   CallLabelHeight - (titleY + CallLabelHeight + CallInsideMargin));
        
        if (self.userCollectionViewLayout) {
            self.userCollectionView.collectionViewLayout = self.userCollectionViewLayout;
        } else {
            MultiAudioCallUserCollectionLayout *userCollectionViewLayout =
            [[MultiAudioCallUserCollectionLayout alloc] initWithItemMargin:5 buttomPadding:10];
            [self.userCollectionView setCollectionViewLayout:userCollectionViewLayout animated:YES];
        }
        self.userCollectionView.hidden = NO;
    } else if (self.callSession.callStatus == JCallStatusOutgoing || (self.callSession.callStatus == JCallStatusConnected && !self.isFullScreen)) {
        self.userCollectionView.frame = CGRectMake(
                                                   0, self.view.frame.size.height - CallVerticalMargin - CallButtonLength * 3.5 - CallInsideMargin * 2 - CallExtraSpace,
                                                   self.view.frame.size.width, CallButtonLength * 2.5);
        
        if (self.userCollectionViewLayout) {
            self.userCollectionView.collectionViewLayout = self.userCollectionViewLayout;
        } else {
            MultiVideoCallUserCollectionLayout *userCollectionViewLayout =
            [[MultiVideoCallUserCollectionLayout alloc] initWithItemMargin:5];
            [self.userCollectionView setCollectionViewLayout:userCollectionViewLayout animated:YES];
        }
        self.userCollectionView.hidden = NO;
    } else if (self.callSession.callStatus == JCallStatusConnected && self.isFullScreen) {
        self.userCollectionView.frame =
        CGRectMake(0, self.view.frame.size.height - CallInsideMargin - CallButtonLength * 2.5 - CallExtraSpace,
                   self.view.frame.size.width, CallButtonLength * 2.5);
        self.userCollectionView.hidden = NO;
    }
    
    if (self.callSession.callStatus == JCallStatusConnected) {
        self.minimizeButton.hidden = self.isFullScreen;
        self.cameraSwitchButton.hidden = self.isFullScreen;
        self.inviteUserButton.hidden = self.isFullScreen;
        self.muteButton.hidden = self.isFullScreen;
        self.hangupButton.hidden = self.isFullScreen;
        self.cameraCloseButton.hidden = self.isFullScreen;
    }
}

#pragma mark - JCallSessionDelegate
- (void)callDidConnect {
    [self.userCollectionView removeFromSuperview];
    _userCollectionView = nil;
    [self userCollectionView];
    [self updateAllSubUserLayout];
    [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
    [self resetLayout];
}

- (void)usersDidInvite:(NSArray<NSString *> *)userIdList
             inviterId:(NSString *)inviterId {
    for (NSString *userId in userIdList) {
        JCallMember *model = [self getCallMember:userId];
        if (!model) {
            continue;
        }
        [self addSubUserModel:model];
    }

//    NSString *userName = model.userInfo.userName;
//    if (userName) {
//        NSString *inviteTips = [NSString
//            stringWithFormat:@"%@ was invited the call", userName];
//        self.tipsLabel.text = inviteTips;
//
//        __weak typeof(self) weakSelf = self;
//        dispatch_after(dispatch_time(DISPATCH_TIME_NOW, (int64_t)(0.5 * NSEC_PER_SEC)), dispatch_get_main_queue(), ^{
//            if ([weakSelf.tipsLabel.text isEqualToString:inviteTips]) {
//                weakSelf.tipsLabel.text = @"";
//            }
//        });
//    }
}

- (void)usersDidConnect:(NSArray<NSString *> *)userIdList {
    for (NSString *userId in userIdList) {
        if (![userId isEqualToString:self.mainModel.userInfo.userId]) {
            JCallMember *model = [self getModelInSubUserModelList:userId];
            if (!model) {
                [self addSubUserModel:[self getCallMember:userId]];
            } else {
                [self updateSubUserLayout:model];
            }
        }
    }
}

- (void)usersDidLeave:(NSArray<NSString *> *)userIdList {
    for (NSString *userId in userIdList) {
        if ([userId isEqualToString:self.mainModel.userInfo.userId]) {
            if (self.callSession.callStatus == JCallStatusIncoming ||
                self.callSession.callStatus == JCallStatusConnected) {
                JCallMember *tempModel = self.subUserModelList[0];
                self.mainModel = tempModel;
                [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
                self.mainNameLabel.text = tempModel.userInfo.userName;
                if (self.callSession.callStatus == JCallStatusIncoming) {
                    self.inviterNameLabel.text = tempModel.userInfo.userName;
                }

                [self.subUserModelList removeObject:tempModel];
                [self updateAllSubUserLayout];
            }
        } else {
            for (JCallMember *userModel in self.subUserModelList) {
                if ([userId isEqualToString:userModel.userInfo.userId]) {
                    [self removeSubUserModel:userModel];
                    break;
                }
            }
        }
    }
}

- (void)userCamaraDidChange:(BOOL)enable
                     userId:(NSString *)userId {
    if ([userId isEqualToString:self.mainModel.userInfo.userId]) {
        if (enable) {
            [self.callSession setVideoView:self.backgroundView forUserId:userId];
        } else {
            [self.callSession setVideoView:nil forUserId:userId];
        }
    } else {
        [self updateSubUserLayout:[self getModelInSubUserModelList:userId]];
    }
}

#pragma mark - UICollectionViewDataSource
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    return self.subUserModelList.count;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView
                  cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    MultiCallUserCell *cell =
        [collectionView dequeueReusableCellWithReuseIdentifier:@"MultiVideoCellReuseID" forIndexPath:indexPath];
    JCallMember *userModel = self.subUserModelList[indexPath.row];
    [cell setModel:userModel status:self.callSession.callStatus];
    [self.callSession setVideoView:(UIView *)cell.headerImageView forUserId:userModel.userInfo.userId];
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    JCallMember *tempModel = self.mainModel;
    JCallMember *selectedModel = self.subUserModelList[indexPath.row];

    self.mainModel = selectedModel;
    [self.callSession setVideoView:self.backgroundView forUserId:self.mainModel.userInfo.userId];
    self.mainNameLabel.text = selectedModel.userInfo.userName;
    [self.subUserModelList removeObject:selectedModel];
    [self.subUserModelList insertObject:tempModel atIndex:indexPath.row];

    [self updateSubUserLayout:tempModel];
}


#pragma mark - GroupMemberSelectVCDelegate
- (void)membersDidSelectWithType:(enum GroupMemberSelectType)type members:(NSArray<JUserInfo *> * _Nonnull)members {
    NSMutableArray *userIdList = [[NSMutableArray alloc] init];
    for (JUserInfo *userInfo in members) {
        [userIdList addObject:userInfo.userId];
    }
    
    [self.callSession inviteUsers:userIdList];
}

@end
