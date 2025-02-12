//
//  BaseSettingTableViewCell.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/23.
//

#import "BaseSettingTableViewCell.h"
#import "OCConst.h"
#import <SDWebImage/UIImageView+WebCache.h>

@interface BaseSettingTableViewCell ()
@property (nonatomic, strong) NSDictionary *cellSubViews;
@property(nonatomic, strong) NSArray *leftLabelConstraints;
@property (nonatomic, strong) NSArray *rightLabelConstraints;
@end

@implementation BaseSettingTableViewCell

- (void)awakeFromNib {
    [super awakeFromNib];
    
}

- (void)setSelected:(BOOL)selected animated:(BOOL)animated {
    [super setSelected:selected animated:animated];
}

- (instancetype)initWithFrame:(CGRect)frame {
    self = [super initWithFrame:frame];
    if (self) {
        self.selectionStyle = UITableViewCellSelectionStyleNone;
        [self initialize];
    }
    return self;
}

- (instancetype)initWithCoder:(NSCoder *)coder
{
    self = [super initWithCoder:coder];
    if (self) {
        [self initialize];
    }
    return self;
}

- (void)initialize {
    self.leftLabel = [[UILabel alloc] init];
    self.leftLabel.font = [UIFont systemFontOfSize:16.f];
    self.leftLabel.textColor = ColorFromRGB(0x000000);
    self.leftLabel.translatesAutoresizingMaskIntoConstraints = NO;

    self.rightLabel = [[UILabel alloc] init];
    self.rightLabel.font = [UIFont systemFontOfSize:14.f];
    self.rightLabel.textColor = ColorFromRGB(0x999999);
    self.rightLabel.translatesAutoresizingMaskIntoConstraints = NO;
    
    self.rightArrow = [[UIImageView alloc] init];
    self.rightArrow.image = [UIImage imageNamed:@"iconChevronRight"];
    self.rightArrow.translatesAutoresizingMaskIntoConstraints = NO;

    self.switchButton = [[UISwitch alloc] init];
    [self.switchButton addTarget:self action:@selector(onClickSwitch:) forControlEvents:UIControlEventValueChanged];
    self.switchButton.on = self.switchButtonStatus;
    self.switchButton.translatesAutoresizingMaskIntoConstraints = NO;

    self.bottomLine = [[UIView alloc] init];
    self.bottomLine.backgroundColor = ColorFromRGB(0xdfdfdf);
    self.bottomLine.translatesAutoresizingMaskIntoConstraints = NO;

    [self.contentView addSubview:self.leftLabel];
    [self.contentView addSubview:self.rightLabel];
    [self.contentView addSubview:self.rightArrow];
    [self.contentView addSubview:self.switchButton];
    [self.contentView addSubview:self.bottomLine];

    self.cellSubViews =
        NSDictionaryOfVariableBindings(_leftLabel, _rightLabel, _rightArrow, _switchButton, _bottomLine);
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
    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_rightLabel(21)]"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];
    [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_rightLabel
                                                                 attribute:NSLayoutAttributeCenterY
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.contentView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                multiplier:1
                                                                  constant:0]];
    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_rightArrow(13)]"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];
    [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_rightArrow
                                                                 attribute:NSLayoutAttributeCenterY
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.contentView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                multiplier:1
                                                                  constant:0]];

    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"H:|-10-[_bottomLine]|"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];
    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[_bottomLine(0.5)]|"
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];

    [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:_switchButton
                                                                 attribute:NSLayoutAttributeCenterY
                                                                 relatedBy:NSLayoutRelationEqual
                                                                    toItem:self.contentView
                                                                 attribute:NSLayoutAttributeCenterY
                                                                multiplier:1
                                                                  constant:0]];
}

- (void)setCellStyle:(BaseSettingCellStyle)style {
    NSString *constraints;
    switch (style) {
    case DefaultStyle: {
        self.switchButton.hidden = YES;
        self.rightLabel.hidden = YES;
        constraints = @"H:|-10-[_leftLabel]-(>=10)-[_rightArrow(8)]-10-|";
    } break;
            
    case RightLabelStyle: {
        self.switchButton.hidden = YES;
        constraints = @"H:|-10-[_leftLabel]-(>=5)-[_rightLabel]-8-[_rightArrow(16)]-12-|";
    } break;

    case SwitchStyle: {
        self.rightArrow.hidden = YES;
        self.switchButton.hidden = NO;
        constraints = @"H:|-10-[_leftLabel]-(>=10)-[_switchButton(50)]-10-|";
    } break;
    default:
        break;
    }
    [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:constraints
                                                                             options:0
                                                                             metrics:nil
                                                                               views:self.cellSubViews]];
}

- (void)setImageView:(UIImageView *)imageView
            ImageStr:(NSString *)imageStr
           imageSize:(CGSize)imageSize
         LeftOrRight:(NSInteger)LeftOrRight {
    self.switchButton.hidden = YES;
    if (imageView == nil) {
        imageView = [[UIImageView alloc] init];
    }
    if (imageStr != nil) {
        if ([imageStr hasPrefix:@"http"] || [imageStr hasPrefix:@"file:"]) {
            [imageView sd_setImageWithURL:[NSURL URLWithString:imageStr] placeholderImage:nil];
        } else {
            imageView.image = [UIImage imageNamed:imageStr];
        }
        imageView.translatesAutoresizingMaskIntoConstraints = NO;
        [self.contentView addSubview:imageView];
        self.cellSubViews =
            NSDictionaryOfVariableBindings(_leftLabel, _rightLabel, _rightArrow, _switchButton, imageView);

        if (LeftOrRight == 0) {
            if (self.leftLabelConstraints != nil) {
                [self.contentView removeConstraints:self.leftLabelConstraints];
            }
            self.leftImageView = imageView;
            self.leftLabelConstraints = [NSLayoutConstraint
                constraintsWithVisualFormat:@"H:|-10-[imageView(width)]-8-[_leftLabel]-10-[_rightArrow(16)]-12-|"
                                    options:0
                                    metrics:@{
                                        @"width" : @(imageSize.width)
                                    }
                                      views:self.cellSubViews];
            [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[imageView(height)]"
                                                                                     options:0
                                                                                     metrics:@{
                                                                                         @"height" : @(imageSize.height)
                                                                                     }
                                                                                       views:self.cellSubViews]];

            [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:imageView
                                                                         attribute:NSLayoutAttributeCenterY
                                                                         relatedBy:NSLayoutRelationEqual
                                                                            toItem:self.contentView
                                                                         attribute:NSLayoutAttributeCenterY
                                                                        multiplier:1
                                                                          constant:0]];

            [self.contentView addConstraints:self.leftLabelConstraints];
        }
        if (LeftOrRight == 1) {
            if (self.rightLabelConstraints != nil) {
                [self.contentView removeConstraints:self.rightLabelConstraints];
            }
            self.rightImageView = imageView;
            self.rightLabelConstraints = [NSLayoutConstraint
                constraintsWithVisualFormat:@"H:|-10-[_leftLabel]-(>=10)-[imageView(width)]-13-[_rightArrow(16)]-12-|"
                                    options:0
                                    metrics:@{
                                        @"width" : @(imageSize.width)
                                    }
                                      views:self.cellSubViews];
            [self.contentView addConstraints:[NSLayoutConstraint constraintsWithVisualFormat:@"V:[imageView(height)]"
                                                                                     options:0
                                                                                     metrics:@{
                                                                                         @"height" : @(imageSize.height)
                                                                                     }
                                                                                       views:self.cellSubViews]];

            [self.contentView addConstraint:[NSLayoutConstraint constraintWithItem:imageView
                                                                         attribute:NSLayoutAttributeCenterY
                                                                         relatedBy:NSLayoutRelationEqual
                                                                            toItem:self.contentView
                                                                         attribute:NSLayoutAttributeCenterY
                                                                        multiplier:1
                                                                          constant:0]];
            [self.contentView addConstraints:self.rightLabelConstraints];
            self.rightImageView.layer.masksToBounds = YES;
            self.rightImageView.layer.cornerRadius = 65/2.f;
        }
        [self setNeedsUpdateConstraints];
        [self updateConstraintsIfNeeded];
        [self layoutIfNeeded];
    }
}

- (void)onClickSwitch:(id)sender {
    if ([self.delegate respondsToSelector:@selector(onClickSwitchButton:)]) {
        [self.delegate onClickSwitchButton:sender];
    }
}

- (void)setSwitchButtonStatus:(BOOL)switchButtonStatus {
    self.switchButton.on = switchButtonStatus;
    _switchButtonStatus = switchButtonStatus;
}

@end
