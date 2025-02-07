//
//  UserListCollectionItem.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/27.
//

#import <UIKit/UIKit.h>
#import "QuickStart-Swift.h"

NS_ASSUME_NONNULL_BEGIN

@protocol UserListCollectionItemDelegate;

@interface UserListCollectionItem : UICollectionViewCell
@property (nonatomic, strong) UIImageView *ivAva;
@property (nonatomic, strong) UILabel *titleLabel;
@property (nonatomic, strong) UIButton *btnImg;
@property (nonatomic, copy) NSString *userId;
@property (nonatomic, copy) NSString *groupId;
@property (nonatomic, weak) id<UserListCollectionItemDelegate> delegate;

- (void)setUserInfo:(JCUser *)user;
@end

@protocol UserListCollectionItemDelegate <NSObject>

- (void)deleteButtonDidClick:(UserListCollectionItem *)item;

@end

NS_ASSUME_NONNULL_END
