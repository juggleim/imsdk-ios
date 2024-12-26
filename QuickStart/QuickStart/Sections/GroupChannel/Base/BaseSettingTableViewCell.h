//
//  BaseSettingTableViewCell.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/23.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol BaseSettingTableViewCellDelegate <NSObject>
@optional
- (void)onClickSwitchButton:(id)sender;
@end

typedef NS_ENUM(NSUInteger, BaseSettingCellStyle) {
    /*!
     Display：leftLabel,rightArrow
     */
    DefaultStyle,
    
    /*!
     Display：leftLabel,rightLabel,rightArrow
     */
    RightLabelStyle,

    /*!
     Display：leftLabel,switchButton
     */
    SwitchStyle
};

@interface BaseSettingTableViewCell : UITableViewCell
@property (nonatomic, strong) UILabel *leftLabel;
@property (nonatomic, strong) UISwitch *switchButton;
@property (nonatomic, strong) UILabel *rightLabel;
@property (nonatomic, strong) UIImageView *rightArrow;
@property (nonatomic, strong) UIView *bottomLine;
@property (nonatomic, assign) BOOL switchButtonStatus;
@property (weak, nonatomic) id<BaseSettingTableViewCellDelegate> delegate;

- (void)setCellStyle:(BaseSettingCellStyle)style;
@end

NS_ASSUME_NONNULL_END
