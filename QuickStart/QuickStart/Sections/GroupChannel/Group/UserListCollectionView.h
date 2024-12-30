//
//  UserListCollectionView.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/27.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@protocol UserListCollectionViewDelegate <NSObject>

- (void)addButtonDidClick;
- (void)deleteButtonDidClick;
- (void)userDidClick:(NSString *)userId;

@end

@interface UserListCollectionView : UICollectionView

@property (nonatomic, weak) id<UserListCollectionViewDelegate> userListCollectionViewDelegate;
@property (nonatomic, strong) NSString *groupId;
@property (nonatomic, assign) BOOL isAllowAdd;
@property (nonatomic, assign) BOOL isAllowDelete;

- (instancetype)initWithFrame:(CGRect)frame;
- (void)reloadData:(NSArray *)userList;
@end

NS_ASSUME_NONNULL_END
