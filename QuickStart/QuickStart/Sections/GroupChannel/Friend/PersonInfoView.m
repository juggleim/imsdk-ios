//
//  PersonInfoView.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/7.
//

#import "PersonInfoView.h"
#import "OCConst.h"
#import "Masonry/Masonry.h"
#import <SDWebImage/UIImageView+WebCache.h>

@interface PersonInfoView ()
@property (nonatomic, strong) UIView *infoBgView;
@property (nonatomic, strong) UIImageView *portraitImgView;
@property (nonatomic, strong) UILabel *remarksLabel;
@property (nonatomic, strong) UILabel *nameLabel;

@property (nonatomic, strong) JCUser *user;
@end

@implementation PersonInfoView

- (instancetype)initWithFrame:(CGRect)frame
{
    self = [super initWithFrame:frame];
    if (self) {
        [self addSubviews];
    }
    return self;
}

- (void)setUserInfo:(JCUser *)userInfo {
    self.user = userInfo;
    if ([userInfo.userId isEqualToString:JIM.shared.currentUserId]) {
        [self updateSubviewsWithHaveRemarks:NO];
        self.nameLabel.text = userInfo.userName;
    } else {
//        if (userInfo.alias.length > 0) {
//            [self updateSubviewsWithHaveRemarks:YES];
//            self.remarksLabel.text = userInfo.alias;
//            self.nameLabel.text = [NSString stringWithFormat:RCDLocalizedString(@"someone_nickname"), userInfo.name];
//        } else {
            [self updateSubviewsWithHaveRemarks:NO];
            self.nameLabel.text = userInfo.userName;
//        }
    }
    if (!userInfo.portrait || userInfo.portrait.length <= 0) {
        self.portraitImgView.image = [UIImage imageNamed:@"iconPerson"];
    } else {
        [self.portraitImgView sd_setImageWithURL:[NSURL URLWithString:userInfo.portrait]
                                placeholderImage:[UIImage imageNamed:@"iconPerson"]];
    }
//    [self updateInfoViewLayout];
}

- (void)addSubviews {
    [self addSubview:self.infoBgView];
    [self.infoBgView addSubview:self.portraitImgView];
    [self.infoBgView addSubview:self.remarksLabel];
    [self.infoBgView addSubview:self.nameLabel];

    [self.infoBgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.bottom.leading.trailing.equalTo(self);
    }];

    [self.portraitImgView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(self).offset(10);
        make.centerY.equalTo(self);
        make.width.height.offset(65);
    }];

    [self.remarksLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.infoBgView).offset(14);
        make.leading.equalTo(self.portraitImgView.mas_trailing).offset(10);
        make.height.offset(16);
    }];

    [self.nameLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.remarksLabel.mas_bottom).offset(3);
        make.leading.equalTo(self.remarksLabel);
        make.trailing.equalTo(self.infoBgView);
        make.height.offset(16);
    }];
}

- (void)updateSubviewsWithHaveRemarks:(BOOL)haveRemarks {
    self.remarksLabel.hidden = !haveRemarks;
    if (haveRemarks == YES) {
        self.nameLabel.font = [UIFont systemFontOfSize:14];
        self.nameLabel.textColor = ColorFromRGB(0x999999);
        [self.remarksLabel mas_remakeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.infoBgView).offset(14);
            make.leading.equalTo(self.portraitImgView.mas_trailing).offset(10);
            make.height.offset(18);
        }];

        [self.nameLabel mas_remakeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.remarksLabel.mas_bottom).offset(4);
            make.leading.equalTo(self.remarksLabel);
            make.trailing.equalTo(self.infoBgView);
            make.height.offset(16);
        }];
    } else {
        self.nameLabel.font = [UIFont systemFontOfSize:16];
        self.nameLabel.textColor = ColorFromRGB(0x000000);

        [self.nameLabel mas_remakeConstraints:^(MASConstraintMaker *make) {
            make.centerY.equalTo(self.infoBgView);
            make.leading.equalTo(self.portraitImgView.mas_trailing).offset(10);
            make.height.offset(18);
        }];
    }
}

#pragma mark - Getter && Setter
- (UIView *)infoBgView {
    if (!_infoBgView) {
        _infoBgView = [[UIView alloc] init];
        _infoBgView.backgroundColor = ColorFromRGB(0xffffff);
    }
    return _infoBgView;
}

- (UIImageView *)portraitImgView {
    if (!_portraitImgView) {
        _portraitImgView = [[UIImageView alloc] init];
        _portraitImgView.contentMode = UIViewContentModeScaleAspectFill;
        _portraitImgView.layer.masksToBounds = YES;
        _portraitImgView.layer.cornerRadius = 32;
        _portraitImgView.clipsToBounds = YES;
    }
    return _portraitImgView;
}

- (UILabel *)remarksLabel {
    if (!_remarksLabel) {
        _remarksLabel = [[UILabel alloc] init];
        _remarksLabel.font = [UIFont systemFontOfSize:16];
        _remarksLabel.textColor = ColorFromRGB(0x000000);
        _remarksLabel.hidden = YES;
    }
    return _remarksLabel;
}

- (UILabel *)nameLabel {
    if (!_nameLabel) {
        _nameLabel = [[UILabel alloc] init];
        _nameLabel.font = [UIFont systemFontOfSize:14];
        _nameLabel.textColor = ColorFromRGB(0x000000);
    }
    return _nameLabel;
}
@end
