#import "NormalAlertView.h"
#import <Masonry/Masonry.h>
#import "OCConst.h"

typedef enum : NSUInteger {
    AlertViewCancel,
    AlertViewConfirm,
} AlertViewActionTag;

#define AWidth 320
#define AHeight 134

@interface NormalAlertView ()

@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UILabel *messageLabel;
@property (nonatomic, strong) UILabel *infoLabel;
@property (nonatomic, strong) UIButton *cancelButton;
@property (nonatomic, strong) UIButton *downgradeButton;
@property (nonatomic, strong) UIView *horizontalLine;
@property (nonatomic, strong) UIView *verticalLine;
@property (nonatomic, strong) NSString *title;
@property (nonatomic, strong) NSString *message;
@property (nonatomic, strong) NSString *info;
@property (nonatomic, strong) NSString *highlightText;
@property (nonatomic, strong) NSString *leftTitle;
@property (nonatomic, strong) NSString *rightTitle;
@property (nonatomic, copy) ButtonBlock cancel;
@property (nonatomic, copy) ButtonBlock confirm;

@end

@implementation NormalAlertView

+ (void)showAlertWithTitle:(NSString *)title
                   message:(NSString *)message
             describeTitle:(NSString *)describeTitle
              confirmTitle:(NSString *)confirmTitle
                   confirm:(ButtonBlock)confirm {
    [self showAlertWithTitle:title
                     message:message
               highlightText:@""
               describeTitle:describeTitle
                   leftTitle:@""
                  rightTitle:confirmTitle
                      cancel:nil
                     confirm:confirm];
}

+ (void)showAlertWithMessage:(NSString *)Message
               highlightText:(NSString *)highlightText
                   leftTitle:(NSString *)leftTitle
                  rightTitle:(NSString *)rightTitle
                      cancel:(ButtonBlock)cancel
                     confirm:(ButtonBlock)confirm {
    [self showAlertWithTitle:@""
                     message:Message
               highlightText:highlightText
               describeTitle:@""
                   leftTitle:leftTitle
                  rightTitle:rightTitle
                      cancel:cancel
                     confirm:confirm];
}

+ (void)showAlertWithTitle:(NSString *)title
                   message:(NSString *)message
             highlightText:(NSString *)highlightText
             describeTitle:(NSString *)describeTitle
                 leftTitle:(NSString *)leftTitle
                rightTitle:(NSString *)rightTitle
                    cancel:(ButtonBlock)cancel
                   confirm:(ButtonBlock)confirm {
    NormalAlertView *alertView = [[NormalAlertView alloc] initWithFrame:[UIScreen mainScreen].bounds];
    alertView.backgroundColor = [ColorFromRGB(0x000000) colorWithAlphaComponent:0.5];
    alertView.title = title;
    alertView.message = message;
    alertView.info = describeTitle;
    alertView.highlightText = highlightText;
    alertView.leftTitle = leftTitle;
    alertView.rightTitle = rightTitle;
    alertView.cancel = cancel;
    alertView.confirm = confirm;
    [alertView addSubviews];
    [alertView showAlertView];
}

- (void)showAlertView {
    [[UIApplication sharedApplication].keyWindow addSubview:self];
}

- (void)dismissAlertView {
    [self removeFromSuperview];
}

- (void)addSubviews {
    CGRect rect = CGRectMake(([UIScreen mainScreen].bounds.size.width - AWidth) / 2,
                             ([UIScreen mainScreen].bounds.size.height - AHeight) / 2, AWidth, AHeight);
    UIView *contentView = [[UIView alloc] initWithFrame:rect];
    contentView.backgroundColor = ColorFromRGB(0xffffff);
    contentView.layer.cornerRadius = 8;
    contentView.layer.masksToBounds = YES;
    [self addSubview:contentView];
    [contentView addSubview:self.titleLabel];
    [contentView addSubview:self.messageLabel];
    [contentView addSubview:self.infoLabel];
    [contentView addSubview:self.cancelButton];
    [contentView addSubview:self.downgradeButton];
    [contentView addSubview:self.horizontalLine];
    [contentView addSubview:self.verticalLine];
    CGSize size = contentView.bounds.size;
    if (self.title.length > 0) {
        [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(contentView.mas_top).offset(15);
            make.left.equalTo(contentView.mas_left).offset(13);
            make.right.equalTo(contentView.mas_right).offset(-13);
            make.height.offset(20);
        }];
    } else {
        self.titleLabel.hidden = YES;
        [self.titleLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.width.height.left.offset(0);
        }];
    }
    [self.messageLabel mas_makeConstraints:^(MASConstraintMaker *make) {
        make.top.equalTo(self.titleLabel.mas_bottom).offset(20);
        make.left.equalTo(contentView.mas_left).offset(13);
        make.right.equalTo(contentView.mas_right).offset(-13);
    }];
    if (self.info.length > 0) {
        [self.infoLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.messageLabel.mas_bottom).offset(10);
            make.left.equalTo(contentView.mas_left).offset(13);
            make.right.equalTo(contentView.mas_right).offset(-13);
        }];
    } else {
        self.infoLabel.hidden = YES;
        [self.infoLabel mas_makeConstraints:^(MASConstraintMaker *make) {
            make.top.equalTo(self.messageLabel.mas_bottom).offset(10);
            make.width.height.left.offset(0);
        }];
    }
    [self.horizontalLine mas_makeConstraints:^(MASConstraintMaker *make) {
        make.left.equalTo(contentView.mas_left).offset(0);
        make.right.equalTo(contentView.mas_right).offset(0);
        make.bottom.equalTo(contentView.mas_bottom).offset(-44);
        make.height.equalTo(@0.5);
    }];
    if (self.leftTitle.length > 0) {
        [self.cancelButton mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(contentView.mas_left).offset(0);
            make.bottom.equalTo(contentView.mas_bottom).offset(0);
            make.height.equalTo(@43);
            make.width.equalTo(@(size.width / 2));
        }];
        [self.downgradeButton mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(self.cancelButton.mas_right).offset(0);
            make.bottom.equalTo(contentView.mas_bottom).offset(0);
            make.height.equalTo(@43);
            make.width.equalTo(@(size.width / 2));
        }];

        [self.verticalLine mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(self.cancelButton.mas_right).offset(0);
            make.bottom.equalTo(contentView.mas_bottom).offset(0);
            make.width.equalTo(@0.5);
            make.height.equalTo(@43);
        }];
    } else {
        [self.downgradeButton mas_makeConstraints:^(MASConstraintMaker *make) {
            make.left.equalTo(contentView);
            make.bottom.equalTo(contentView.mas_bottom).offset(0);
            make.height.equalTo(@43);
            make.width.equalTo(contentView);
        }];
    }
    [contentView updateConstraintsIfNeeded];
    [contentView layoutIfNeeded];
    rect.size.height = CGRectGetMaxY(self.infoLabel.frame) + 44 + 10;
    rect.origin.y = ([UIScreen mainScreen].bounds.size.height - rect.size.height) / 2;
    contentView.frame = rect;
}

- (void)buttonAction:(UIButton *)button {
    if (button.tag == AlertViewCancel) {
        if (self.cancel) {
            self.cancel();
        }
    } else {
        if (self.confirm) {
            self.confirm();
        }
    }
    [self dismissAlertView];
}

- (UILabel *)titleLabel {
    if (!_titleLabel) {
        _titleLabel = [[UILabel alloc] init];
        _titleLabel.font = [UIFont boldSystemFontOfSize:18];
        _titleLabel.textAlignment = NSTextAlignmentCenter;
        _titleLabel.text = self.title;
        _titleLabel.numberOfLines = 1;
        _titleLabel.lineBreakMode = NSLineBreakByTruncatingMiddle;
        _titleLabel.textColor = ColorFromRGB(0x000000);
    }
    return _titleLabel;
}

- (UILabel *)messageLabel {
    if (!_messageLabel) {
        _messageLabel = [[UILabel alloc] init];
        _messageLabel.font = [UIFont systemFontOfSize:17];
        _messageLabel.textAlignment = NSTextAlignmentCenter;
        _messageLabel.text = self.message;
        _messageLabel.numberOfLines = 0;
        _messageLabel.lineBreakMode = NSLineBreakByTruncatingMiddle;
        _messageLabel.textColor = ColorFromRGB(0x262626);
        if (self.highlightText.length > 0) {
            NSRange range = [self.message rangeOfString:self.highlightText];
            if (range.location != NSNotFound) {
                NSMutableAttributedString *attributedString =
                    [[NSMutableAttributedString alloc] initWithString:self.message];
                [attributedString addAttribute:NSForegroundColorAttributeName value:ColorFromRGB(0x3a91f3) range:range];
                _messageLabel.attributedText = attributedString;
            }
        }
    }
    return _messageLabel;
}

- (UILabel *)infoLabel {
    if (!_infoLabel) {
        _infoLabel = [[UILabel alloc] init];
        _infoLabel.font = [UIFont systemFontOfSize:16];
        _infoLabel.textAlignment = NSTextAlignmentCenter;
        _infoLabel.text = self.info;
        _infoLabel.numberOfLines = 0;
        _infoLabel.lineBreakMode = NSLineBreakByTruncatingMiddle;
        _infoLabel.textColor = ColorFromRGB(0x939393);
    }
    return _infoLabel;
}

- (UIButton *)cancelButton {
    if (!_cancelButton) {
        _cancelButton = [[UIButton alloc] init];
        _cancelButton.backgroundColor = [UIColor clearColor];
        [_cancelButton.titleLabel setFont:[UIFont systemFontOfSize:18]];
        [_cancelButton setTitleColor:ColorFromRGB(0x262626) forState:UIControlStateNormal];
        [_cancelButton setTitle:self.leftTitle forState:UIControlStateNormal];
        _cancelButton.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
        _cancelButton.tag = AlertViewCancel;
        [_cancelButton addTarget:self action:@selector(buttonAction:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _cancelButton;
}

- (UIButton *)downgradeButton {
    if (!_downgradeButton) {
        _downgradeButton = [[UIButton alloc] init];
        _downgradeButton.backgroundColor = [UIColor clearColor];
        [_downgradeButton.titleLabel setFont:[UIFont systemFontOfSize:18]];
        [_downgradeButton setTitleColor:ColorFromRGB(0x3a91f3) forState:UIControlStateNormal];
        [_downgradeButton setTitle:self.rightTitle forState:UIControlStateNormal];
        _downgradeButton.contentHorizontalAlignment = UIControlContentHorizontalAlignmentCenter;
        _downgradeButton.tag = AlertViewConfirm;
        [_downgradeButton addTarget:self action:@selector(buttonAction:) forControlEvents:UIControlEventTouchUpInside];
    }
    return _downgradeButton;
}

- (UIView *)horizontalLine {
    if (!_horizontalLine) {
        _horizontalLine = [[UIView alloc] init];
        _horizontalLine.backgroundColor = ColorFromRGB(0xE5E5E5);
    }
    return _horizontalLine;
}

- (UIView *)verticalLine {
    if (!_verticalLine) {
        _verticalLine = [[UIView alloc] init];
        _verticalLine.backgroundColor = ColorFromRGB(0xE5E5E5);
    }
    return _verticalLine;
}
@end
