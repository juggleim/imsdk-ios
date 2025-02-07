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
@property (nonatomic, strong) UIImageView *leftImageView;
@property (nonatomic, strong) UIImageView *rightImageView;

@property (nonatomic, assign) BOOL switchButtonStatus;
@property (weak, nonatomic) id<BaseSettingTableViewCellDelegate> delegate;

- (void)setCellStyle:(BaseSettingCellStyle)style;

/*!
 设置imageView方法
 @param imageView   cell中左边或者右边的imageView
 @param imageStr    image的name或者url
 @param imageSize   image的尺寸大小
 @param LeftOrRight 为了设置约束，需要告诉方法是设置左边还是右边的imageView
                    0:表示设置左边的imageView。
                    1:表示设置右边的imageView。
 */
- (void)setImageView:(UIImageView *)imageView
            ImageStr:(NSString *)imageStr
           imageSize:(CGSize)imageSize
         LeftOrRight:(NSInteger)LeftOrRight;
@end

NS_ASSUME_NONNULL_END
