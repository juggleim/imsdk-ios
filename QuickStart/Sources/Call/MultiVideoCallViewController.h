//
//  MultiVideoCallViewController.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

#import "BaseCallViewController.h"

NS_ASSUME_NONNULL_BEGIN

@interface MultiVideoCallViewController : BaseCallViewController

/*!
 通话呼入时，最上方显示的邀请者头像View
 */
@property(nonatomic, strong) UIImageView *inviterPortraitView;

/*!
 通话呼入时，最上方显示的邀请者名字Label
 */
@property(nonatomic, strong) UILabel *inviterNameLabel;

/*!
 全屏的用户的名称Label
 */
@property(nonatomic, strong) UILabel *mainNameLabel;

/*!
 通话呼入时，用户列表说明的Label
 */
@property(nonatomic, strong) UILabel *userCollectionTitleLabel;

/*!
 用户视频列表的CollectionView
 */
@property(nonatomic, strong) UICollectionView *userCollectionView;

/*!
 用户视频列表CollectionView所使用的Layout

 @discussion
 默认使用的是RCVideoMultiCallUserCollectionLayout，您也可以自定义Layout。
 */
@property(nonatomic, strong) UICollectionViewLayout *userCollectionViewLayout;

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;
@end

NS_ASSUME_NONNULL_END
