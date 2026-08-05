//
//  MultiAudioCallViewController.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/9.
//

#import "BaseCallViewController.h"

NS_ASSUME_NONNULL_BEGIN

@interface MultiAudioCallViewController : BaseCallViewController
/*!
 Avatar view of the inviter shown at the top during incoming calls
 */
@property(nonatomic, strong) UIImageView *inviterPortraitView;

/*!
 Name label of the inviter shown at the top during incoming calls
 */
@property(nonatomic, strong) UILabel *inviterNameLabel;

/*!
 User list description label during incoming calls
 */
@property(nonatomic, strong) UILabel *userCollectionTitleLabel;

/*!
 user avatar list collection view
 */
@property(nonatomic, strong) UICollectionView *userCollectionView;

/*!
 layout used by the user avatar list collection view
 */
@property(nonatomic, strong) UICollectionViewLayout *userCollectionViewLayout;

- (instancetype)initWithOutgoingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;

- (instancetype)initWithIncomingCall:(id<JCallSession>)callSession
                             groupId:(NSString *)groupId;
@end

NS_ASSUME_NONNULL_END
