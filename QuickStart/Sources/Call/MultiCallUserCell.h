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
 User avatar view (used as the video view during calls)
 */
@property(nonatomic, strong) UIImageView *headerImageView;

/*!
 User name label
 */
@property(nonatomic, strong) UILabel *nameLabel;

/*!
 User status label
 */
@property(nonatomic, strong) UIImageView *statusView;

/*!
 Set user call info and call status

 @param model      user call info model
 @param callStatus user call status
 */
- (void)setModel:(JCallMember *)model status:(JCallStatus)callStatus;

@end

NS_ASSUME_NONNULL_END
