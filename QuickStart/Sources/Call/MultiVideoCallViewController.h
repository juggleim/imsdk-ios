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
 Avatar view of the inviter shown at the top during incoming calls
 */
@property(nonatomic, strong) UIImageView *inviterPortraitView;

/*!
 Name label of the inviter shown at the top during incoming calls
 */
@property(nonatomic, strong) UILabel *inviterNameLabel;

/*!
 Full-screen user name label
 */
@property(nonatomic, strong) UILabel *mainNameLabel;

/*!
 User list description label during incoming calls
 */
@property(nonatomic, strong) UILabel *userCollectionTitleLabel;

/*!
 User video list collection view
 */
@property(nonatomic, strong) UICollectionView *userCollectionView;

/*!
 Layout used by the user video list collection view

 @discussion
 RCVideoMultiCallUserCollectionLayout is used by default, and you can customize the layout.
 */
@property(nonatomic, strong) UICollectionViewLayout *userCollectionViewLayout;

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;
@end

NS_ASSUME_NONNULL_END
