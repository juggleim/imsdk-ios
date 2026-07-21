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
 Set imageView helper
 @param imageView   left or right imageView in the cell
 @param imageStr    image name or URL
 @param imageSize   image size
 @param LeftOrRight tells the helper whether to configure the left or right imageView
                    0: configure the left imageView.
                    1: configure the right imageView.
 */
- (void)setImageView:(UIImageView *)imageView
            ImageStr:(NSString *)imageStr
           imageSize:(CGSize)imageSize
         LeftOrRight:(NSInteger)LeftOrRight;
@end

NS_ASSUME_NONNULL_END
