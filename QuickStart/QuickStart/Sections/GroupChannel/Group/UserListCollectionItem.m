//
//  UserListCollectionItem.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/27.
//

#import "UserListCollectionItem.h"
#import "OCConst.h"
#import <JuggleIM/JuggleIM.h>
#import <SDWebImage/UIImageView+WebCache.h>
#import "PortraitUtil.h"

#define ivAvaWidth 40

@implementation UserListCollectionItem
- (instancetype)initWithFrame:(CGRect)frame {
    if (self = [super initWithFrame:frame]) {
        _ivAva = [[UIImageView alloc] initWithFrame:CGRectZero];
        _ivAva.clipsToBounds = YES;
        _ivAva.layer.cornerRadius = ivAvaWidth/2;
        [_ivAva setBackgroundColor:[UIColor clearColor]];
        [self.contentView addSubview:_ivAva];

        _titleLabel = [UILabel new];
        [_titleLabel setTextColor:ColorFromRGB(0x999999)];
        [_titleLabel setFont:[UIFont systemFontOfSize:13]];
        [_titleLabel setTextAlignment:NSTextAlignmentCenter];
        [self.contentView addSubview:_titleLabel];

        _btnImg = [[UIButton alloc] initWithFrame:CGRectZero];
        [_btnImg setHidden:YES];
        [_btnImg setImage:[UIImage imageNamed:@"deleteMemberTip"]
                 forState:UIControlStateNormal];
        [_btnImg addTarget:self action:@selector(deleteItem:) forControlEvents:UIControlEventTouchUpInside];
        [self.contentView addSubview:_btnImg];

        [_ivAva setTranslatesAutoresizingMaskIntoConstraints:NO];
        [_titleLabel setTranslatesAutoresizingMaskIntoConstraints:NO];
        [_btnImg setTranslatesAutoresizingMaskIntoConstraints:NO];

        [self.contentView
            addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_ivAva]|"
                                                                   options:kNilOptions
                                                                   metrics:nil
                                                                     views:NSDictionaryOfVariableBindings(_ivAva)]];

        [self.contentView addConstraints:[NSLayoutConstraint
                                             constraintsWithVisualFormat:@"H:|[_titleLabel]|"
                                                                 options:kNilOptions
                                                                 metrics:nil
                                                                   views:NSDictionaryOfVariableBindings(_titleLabel)]];
        [self.contentView
            addConstraints:[NSLayoutConstraint
                               constraintsWithVisualFormat:@"V:|[_ivAva(ivAvaWidth)]-9-[_titleLabel(==15)]"
                                                   options:kNilOptions
                            metrics:@{@"ivAvaWidth":@(ivAvaWidth)}
                                                     views:NSDictionaryOfVariableBindings(_titleLabel, _ivAva)]];

        [self.contentView
            addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|[_btnImg(25)]"
                                                                   options:kNilOptions
                                                                   metrics:nil
                                                                     views:NSDictionaryOfVariableBindings(_btnImg)]];
        [self.contentView
            addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:|[_btnImg(25)]"
                                                                   options:kNilOptions
                                                                   metrics:nil
                                                                     views:NSDictionaryOfVariableBindings(_btnImg)]];

        [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_titleLabel
                                                                     attribute:NSLayoutAttributeCenterX
                                                                     relatedBy:NSLayoutRelationEqual
                                                                        toItem:_ivAva
                                                                     attribute:NSLayoutAttributeCenterX
                                                                    multiplier:1.0f
                                                                      constant:0]];
    }
    return self;
}

- (void)deleteItem:(id)sender {
    if (self.delegate && [self.delegate respondsToSelector:@selector(deleteButtonDidClick:)]) {
        [self.delegate deleteButtonDidClick:self];
    }
}

- (void)setUserInfo:(JCUser *)user {
    if ([user.userId isEqualToString:JIM.shared.currentUserId]) {
        [self.btnImg setHidden:YES];
    }
    self.ivAva.image = nil;
    self.userId = user.userId;
    self.titleLabel.text = user.userName;
    if (user.portrait.length == 0) {
        UIImage *image = [PortraitUtil defaultPortraitImageWith:user.userId name:user.userName type:JConversationTypePrivate];
        self.ivAva.image = image;
        self.ivAva.contentMode = UIViewContentModeScaleAspectFit;
    } else {
        [self.ivAva sd_setImageWithURL:[NSURL URLWithString:user.portrait]
                      placeholderImage:[UIImage imageNamed:@"iconPerson"]];
    }
}
@end
