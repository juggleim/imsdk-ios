//
//  BaseSelectTableViewCell.m
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

#import "BaseSelectTableViewCell.h"
#import "OCConst.h"

@interface BaseSelectTableViewCell ()
@property (nonatomic, strong) UIImageView *rightImageView;
@property (nonatomic, strong) NSDictionary *cellSubViews;
@end

@implementation BaseSelectTableViewCell

- (instancetype)initWithSelected:(BOOL)isSelected {
    self = [super init];
    if (self) {
        [self initialize:isSelected];
    }
    return self;
}

- (void)initialize:(BOOL)isSelected {
    self.leftLabel = [[UILabel alloc] init];
    self.leftLabel.font = [UIFont systemFontOfSize:17.f];
    self.leftLabel.textColor = ColorFromRGB(0x262626);
    self.leftLabel.translatesAutoresizingMaskIntoConstraints = NO;

    self.rightImageView = [[UIImageView alloc] init];
    self.rightImageView.layer.cornerRadius = 5.0f;
    self.rightImageView.layer.masksToBounds = YES;
    self.rightImageView.translatesAutoresizingMaskIntoConstraints = NO;
    self.rightImageView.contentMode = UIViewContentModeScaleAspectFill;
    if (isSelected) {
        self.rightImageView.image = [UIImage imageNamed:@"iconSelect"];
    } else {
        self.rightImageView.image = nil;
    }

    [self.contentView addSubview:self.leftLabel];
    [self.contentView addSubview:self.rightImageView];

    self.cellSubViews = NSDictionaryOfVariableBindings(_leftLabel, _rightImageView);
    [self setLayout];
}

- (void)setLayout {
    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_leftLabel(21)]"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];
    [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_leftLabel
                                                                 attribute:NSLayoutAttributeCenterY
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.contentView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                multiplier:1
                                                                  constant:0]];

    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_rightImageView(20)]"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];

    [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_rightImageView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.contentView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                multiplier:1
                                                                  constant:0]];

    [self.contentView
        addConstraints:[NSLayoutConstraint
                           constraintsWithVisualFormat:@"H:|-10-[_leftLabel]-(>=10)-[_rightImageView(20)]-20-|"
                                               options:0
                                               metrics:nil
                                                 views:self.cellSubViews]];
}
@end
