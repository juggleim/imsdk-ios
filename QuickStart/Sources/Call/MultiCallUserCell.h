//
//  MultiCallUserCell.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/10.
//

#import <UIKit/UIKit.h>
#import <JuggleIM/JCallMember.h>

NS_ASSUME_NONNULL_BEGIN

@interface MultiCallUserCell : UICollectionViewCell

/*!
 用户的头像View（视频时会使用此View作为用户的视频View）
 */
@property(nonatomic, strong) UIImageView *headerImageView;

/*!
 用户名字的Label
 */
@property(nonatomic, strong) UILabel *nameLabel;

/*!
 用户状态的Label
 */
@property(nonatomic, strong) UIImageView *statusView;

/*!
 设置用户通话信息和通话状态

 @param model      用户通话信息的Model
 @param callStatus 用户通话状态
 */
- (void)setModel:(JCallMember *)model status:(JCallStatus)callStatus;

@end

NS_ASSUME_NONNULL_END
