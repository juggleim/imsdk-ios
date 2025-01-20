//
//  ForwardAlertView.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import "ForwardAlertView.h"
#import "OCConst.h"
#import <Masonry/Masonry.h>
#import <SDWebImage/UIImageView+WebCache.h>

#define itemHeight 40

@interface ForwardAlertView ()
@property (nonatomic, strong) UIView *contentView;
@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UIScrollView *scrollView;
@property (nonatomic, strong) UILabel *nameLabel;
@property (nonatomic, strong) UILabel *forwardContentLabel;
@property (nonatomic, strong) UIButton *cancelButton;
@property (nonatomic, strong) UIButton *confirmButton;

@property (nonatomic, strong) UIImage *imageData;
@property (nonatomic, copy) NSString *titleName;

@property (nonatomic, strong) JMessageContent *messageContent;
@property (nonatomic, strong) JConversation *conversation;
@end

@implementation ForwardAlertView

+ (instancetype)alertViewWithConversation:(JConversation *)conversation
                                  content:(nonnull JMessageContent *)content {
    if (!conversation) {
        return nil;
    }
    UIWindow *keyWindow = [[UIApplication sharedApplication] keyWindow];
    ForwardAlertView *alertV = [[self alloc] initWithFrame:keyWindow.bounds];
    alertV.messageContent = content;
    alertV.conversation = conversation;
    [alertV setModel:conversation];
    return alertV;
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        [self loadSubviews];
    }
    return self;
}

- (void)show {
    UIWindow *keyWindow = [[UIApplication sharedApplication] keyWindow];
    [keyWindow addSubview:self];
    [self updateUI];
}

#pragma mark - Target Action
- (void)confirmButtonEvent {
    [self dealDelegate:1];
}

- (void)cancelButtonEvent {
    [self dealDelegate:0];
}

#pragma mark - Private Method
- (void)loadSubviews {
    self.backgroundColor = ColorFromRGB(0x000000);
    [self addSubview:self.contentView];
    [self.contentView addSubview:self.cancelButton];
    [self.contentView addSubview:self.confirmButton];
    [self.contentView addSubview:self.scrollView];
    [self.contentView addSubview:self.titleLabel];
    [self.contentView addSubview:self.forwardContentLabel];

    [self.contentView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.trailing.equalTo(self).inset(27);
        make.center.equalTo(self);
    }];
    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.contentView).offset(20);
        make.leading.trailing.equalTo(self.contentView).inset(18);
        make.height.offset(25);
    }];
    [self.scrollView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.trailing.equalTo(self.contentView).inset(12);
        make.top.equalTo(self.titleLabel.mas_bottom).offset(14);
        make.height.offset(55);
    }];
    UIView *firstLine = [[UIView alloc] init];
    firstLine.backgroundColor = ColorFromRGB(0xE5E5E5);
    [self.contentView addSubview:firstLine];
    [firstLine mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.trailing.equalTo(self.contentView);
        make.top.equalTo(self.scrollView.mas_bottom);
        make.height.offset(1);
    }];
    [self.forwardContentLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.trailing.equalTo(self.contentView).inset(18);
        make.top.equalTo(self.scrollView.mas_bottom);
        make.height.offset(44);
    }];
    UIView *secondLine = [[UIView alloc] init];
    secondLine.backgroundColor = ColorFromRGB(0xE5E5E5);
    [self.contentView addSubview:secondLine];
    [secondLine mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.trailing.equalTo(self.contentView);
        make.top.equalTo(self.forwardContentLabel.mas_bottom);
        make.height.offset(1);
    }];
    UIView *separateLine = [[UIView alloc] init];
    separateLine.backgroundColor = ColorFromRGB(0xE5E5E5);
    [self.contentView addSubview:separateLine];
    [self.confirmButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.trailing.equalTo(self.contentView);
        make.top.equalTo(self.forwardContentLabel.mas_bottom);
        make.leading.equalTo(separateLine.mas_trailing);
        make.height.offset(44);
        make.bottom.equalTo(self.contentView);
    }];
    [separateLine mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.height.equalTo(self.confirmButton);
        make.width.offset(1);
        make.centerX.equalTo(self.contentView);
    }];
    [self.cancelButton mas_makeConstraints:^(MASConstraintMaker *make) {
        make.trailing.equalTo(separateLine.mas_leading);
        make.top.equalTo(self.forwardContentLabel.mas_bottom);
        make.leading.equalTo(self.contentView);
        make.height.offset(44);
        make.bottom.equalTo(self.contentView);
    }];
}

- (void)setModel:(JConversation *)model {
    CGFloat originX = 18;
    UIImageView *imageView = [[UIImageView alloc] initWithFrame:CGRectMake(originX, 7.5, itemHeight, itemHeight)];
    [self loadImageWithModel:model forImageView:imageView];
    imageView.layer.cornerRadius = 2;
    imageView.layer.masksToBounds = YES;
    [self.scrollView addSubview:imageView];
    [self.scrollView addSubview:self.nameLabel];

    [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(imageView.mas_trailing).offset(10);
        make.centerY.height.equalTo(imageView);
        make.trailing.equalTo(self.scrollView).offset(-10);
    }];

    if (model.conversationType == JConversationTypePrivate) {
        JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:model.conversationId];
        self.nameLabel.text = userInfo.userName;
    } else if (model.conversationType == JConversationTypeGroup) {
        JGroupInfo *groupInfo = [JIM.shared.userInfoManager getGroupInfo:model.conversationId];
        self.nameLabel.text = groupInfo.groupName;
    }
}

- (void)updateUI {
    self.titleLabel.text = @"发送给";
    NSString *displayString = self.messageContent.conversationDigest;
    if (displayString.length > 10) {
        displayString = [[displayString substringToIndex:10] stringByAppendingString:@"..."];
    }
    self.forwardContentLabel.text = displayString;
}

- (void)loadImageWithModel:(JConversation *)model forImageView:(UIImageView *)imageView {
    NSString *imageUrl = @"";
    NSString *name;
    if (model.conversationType == JConversationTypePrivate) {
        JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:model.conversationId];
        imageUrl = userInfo.portrait;
        name = userInfo.userName;
    } else if (model.conversationType == JConversationTypeGroup) {
        JGroupInfo *groupInfo = [JIM.shared.userInfoManager getGroupInfo:model.conversationId];
        imageUrl = groupInfo.portrait;
        name = groupInfo.groupName;
    }
    if (imageUrl.length > 0) {
        [imageView sd_setImageWithURL:[NSURL URLWithString:imageUrl]
                     placeholderImage:[UIImage imageNamed:@"iconPerson"]];
    } else {
        imageView.image = [UIImage imageNamed:@"iconPerson"];
    }
}

- (void)dealDelegate:(NSUInteger)event {
    if (self.delegate && [self.delegate respondsToSelector:@selector(forwardAlertView:conversation:clickedButtonAtIndex:)]) {
        [self.delegate forwardAlertView:self
                           conversation:self.conversation
                   clickedButtonAtIndex:event];
    }
    [self removeFromSuperview];
}

#pragma mark - Getter && Setter
- (UIView *)contentView {
    if (!_contentView) {
        _contentView = [[UIView alloc] init];
        _contentView.backgroundColor = ColorFromRGB(0xffffff);
        _contentView.layer.cornerRadius = 5;
        _contentView.layer.masksToBounds = YES;
        _contentView.userInteractionEnabled = YES;
    }
    return _contentView;
}

- (UIButton *)cancelButton {
    if (!_cancelButton) {
        _cancelButton = [[UIButton alloc] init];
        [_cancelButton setTitle:@"取消" forState:UIControlStateNormal];
        [_cancelButton setTitleColor:ColorFromRGB(0x262626) forState:UIControlStateNormal];
        _cancelButton.titleLabel.font = [UIFont systemFontOfSize:18];
        [_cancelButton addTarget:self action:@selector(cancelButtonEvent) forControlEvents:UIControlEventTouchUpInside];
    }
    return _cancelButton;
}

- (UIButton *)confirmButton {
    if (!_confirmButton) {
        _confirmButton = [[UIButton alloc] init];
        [_confirmButton setTitle:@"发送" forState:UIControlStateNormal];
        [_confirmButton setTitleColor:ColorFromRGB(0x3A91F3) forState:UIControlStateNormal];
        _confirmButton.titleLabel.font = [UIFont systemFontOfSize:18];
        [_confirmButton addTarget:self
                           action:@selector(confirmButtonEvent)
                 forControlEvents:UIControlEventTouchUpInside];
    }
    return _confirmButton;
}

- (UIScrollView *)scrollView {
    if (!_scrollView) {
        _scrollView = [[UIScrollView alloc] init];
        _scrollView.showsHorizontalScrollIndicator = NO;
    }
    return _scrollView;
}

- (UILabel *)titleLabel {
    if (!_titleLabel) {
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.font = [UIFont systemFontOfSize:18];
        _titleLabel.textColor = ColorFromRGB(0x262626);
    }
    return _titleLabel;
}

- (UILabel *)nameLabel {
    if (!_nameLabel) {
        _nameLabel = [[UILabel alloc] init];
        _nameLabel.textColor = ColorFromRGB(0x262626);
        _nameLabel.font = [UIFont systemFontOfSize:18];
        _nameLabel.lineBreakMode = NSLineBreakByTruncatingMiddle;
        [self.scrollView addSubview:_nameLabel];
    }
    return _nameLabel;
}

- (UILabel *)forwardContentLabel {
    if (!_forwardContentLabel) {
        _forwardContentLabel = [[UILabel alloc] init];
        _forwardContentLabel.textColor = ColorFromRGB(0x262626);
        _forwardContentLabel.font = [UIFont systemFontOfSize:17];
    }
    return _forwardContentLabel;
}

@end
