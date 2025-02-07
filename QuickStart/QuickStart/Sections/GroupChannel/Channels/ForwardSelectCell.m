//
//  ForwardSelectCell.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/20.
//

#import "ForwardSelectCell.h"
#import <Masonry/Masonry.h>
#import <SDWebImage/UIImageView+WebCache.h>
#import "OCConst.h"

@interface ForwardSelectCell ()
@property (nonatomic, strong) UIImageView *headerImageView;
@property (nonatomic, strong) UILabel *titleLabel;
@end

@implementation ForwardSelectCell

- (instancetype)initWithStyle:(UITableViewCellStyle)style reuseIdentifier:(NSString *)reuseIdentifier {
    if (self = [super initWithStyle:style reuseIdentifier:reuseIdentifier]) {
        [self setupViews];
    }
    return self;
}

- (void)setupViews {
    self.backgroundColor = ColorFromRGB(0xffffff);
    [self.contentView addSubview:self.headerImageView];
    [self.contentView addSubview:self.titleLabel];

    [self.headerImageView mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(self.contentView).offset(12);
        make.centerY.equalTo(self.contentView).offset(7.5);
        make.height.width.offset(40);
    }];

    [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.leading.equalTo(self.headerImageView.mas_trailing).offset(9);
        make.centerY.equalTo(self.contentView);
        make.trailing.equalTo(self.contentView).offset(-12);
        make.height.offset(24);
    }];
}

- (void)setModel:(JConversation *)model {
    if (model.conversationType == JConversationTypeGroup) {
        JGroupInfo *group = [JIM.shared.userInfoManager getGroupInfo:model.conversationId];
        if (group) {
            [self.headerImageView sd_setImageWithURL:[NSURL URLWithString:group.portrait]
                                    placeholderImage:[UIImage imageNamed:@"iconMembersCustom"]];
            self.titleLabel.text = group.groupName;
        } else {
            [self.headerImageView setImage:[UIImage imageNamed:@"iconMembersCustom"]];
            self.titleLabel.text = @"";
        }
    } else if (model.conversationType == JConversationTypePrivate) {
        JUserInfo *userInfo = [JIM.shared.userInfoManager getUserInfo:model.conversationId];
        if (userInfo) {
            [self.headerImageView sd_setImageWithURL:[NSURL URLWithString:userInfo.portrait]
                                    placeholderImage:[UIImage imageNamed:@"iconPerson"]];
            self.titleLabel.text = userInfo.userName;
        } else {
            [self.headerImageView setImage:[UIImage imageNamed:@"iconPerson"]];
            self.titleLabel.text = @"";
        }
    }
}

- (UIImageView *)headerImageView {
    if (!_headerImageView) {
        _headerImageView = [[UIImageView alloc] init];
        _headerImageView.layer.masksToBounds = YES;
        _headerImageView.layer.cornerRadius = 20;
    }
    return _headerImageView;
}

- (UILabel *)titleLabel {
    if (!_titleLabel) {
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.backgroundColor = [UIColor clearColor];
        _titleLabel.font = [UIFont systemFontOfSize:16];
        _titleLabel.textAlignment = NSTextAlignmentLeft;
        _titleLabel.textColor = ColorFromRGB(0x262626);
    }
    return _titleLabel;
}

@end
