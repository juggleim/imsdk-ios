//
//  UserListCollectionView.m
//  QuickStart
//
//  Created by Fei Li on 2024/12/27.
//

#import "UserListCollectionView.h"
#import "OCConst.h"
#import "UserListCollectionItem.h"
#import "QuickStart-Swift.h"

@interface UserListCollectionView() <UICollectionViewDelegate, UICollectionViewDataSource>
@property (nonatomic, strong) NSArray *userList;
@end

@implementation UserListCollectionView
- (instancetype)initWithFrame:(CGRect)frame {
    UICollectionViewFlowLayout *flowLayout = [[UICollectionViewFlowLayout alloc] init];
    flowLayout.scrollDirection = UICollectionViewScrollDirectionVertical;
    self = [super initWithFrame:frame collectionViewLayout:flowLayout];
    if (self) {
        self.delegate = self;
        self.dataSource = self;
        self.scrollEnabled = NO;
        self.backgroundColor = ColorFromRGB(0xffffff);
        [self registerClass:[UserListCollectionItem class] forCellWithReuseIdentifier:@"UserListCollectionItem"];
        [self registerClass:[UserListCollectionItem class]
            forCellWithReuseIdentifier:@"UserListCollectionItemForSigns"];
    }
    return self;
}

#pragma mark - Api
- (void)reloadData:(NSArray *)userList {
    self.userList = userList;
    [self reloadData];
}

#pragma mark - UICollectionViewDelegateFlowLayout
- (CGSize)collectionView:(UICollectionView *)collectionView
                  layout:(UICollectionViewLayout *)collectionViewLayout
  sizeForItemAtIndexPath:(NSIndexPath *)indexPath {
    float width = 40;
    float height = width + 15 + 9;
    return CGSizeMake(width, height);
}

- (CGFloat)collectionView:(UICollectionView *)collectionView
                                 layout:(UICollectionViewLayout *)collectionViewLayout
    minimumLineSpacingForSectionAtIndex:(NSInteger)section {
    return 12;
}

- (UIEdgeInsets)collectionView:(UICollectionView *)collectionView
                        layout:(UICollectionViewLayout *)collectionViewLayout
        insetForSectionAtIndex:(NSInteger)section {
    UICollectionViewFlowLayout *flowLayout = (UICollectionViewFlowLayout *)collectionViewLayout;
    flowLayout.minimumInteritemSpacing = 20;
    flowLayout.minimumLineSpacing = 12;
    return UIEdgeInsetsMake(15, 10, 10, 10);
}

#pragma mark - UICollectionViewDataSource
- (NSInteger)collectionView:(UICollectionView *)collectionView numberOfItemsInSection:(NSInteger)section {
    NSInteger row = self.userList.count;
    if (self.isAllowAdd) {
        row += 1;
        if (self.isAllowDelete) {
            row += 1;
        }
    }
    return row;
}

- (UICollectionViewCell *)collectionView:(UICollectionView *)collectionView
                  cellForItemAtIndexPath:(NSIndexPath *)indexPath {
    UICollectionViewCell *cell;
    if (indexPath.row < self.userList.count) {
        UserListCollectionItem *settingCell =
            [collectionView dequeueReusableCellWithReuseIdentifier:@"UserListCollectionItem" forIndexPath:indexPath];
        settingCell.groupId = self.groupId;
        [settingCell setUserInfo:self.userList[indexPath.row]];

        settingCell.ivAva.contentMode = UIViewContentModeScaleAspectFill;
        cell = settingCell;
    } else {
        UIImage *image = [UIImage imageNamed:@"addMember"];
        if (self.isAllowDelete) {
            if (indexPath.row == self.userList.count + 1) {
                image = [UIImage imageNamed:@"deleteMember"];
            }
        }
        UserListCollectionItem *cellForSigns =
            [collectionView dequeueReusableCellWithReuseIdentifier:@"UserListCollectionItemForSigns"
                                                      forIndexPath:indexPath];
        cellForSigns.ivAva.image = nil;
        cellForSigns.ivAva.image = image;
        cellForSigns.titleLabel.text = @"";
        cell = cellForSigns;
    }
    return cell;
}

- (void)collectionView:(UICollectionView *)collectionView didSelectItemAtIndexPath:(NSIndexPath *)indexPath {
    if (indexPath.row < self.userList.count) {
        JCUser *user = self.userList[indexPath.row];
        if (self.userListCollectionViewDelegate &&
            [self.userListCollectionViewDelegate respondsToSelector:@selector(userDidClick:)]) {
            [self.userListCollectionViewDelegate userDidClick:user.userId];
        }
    } else if (self.isAllowAdd && indexPath.row == self.userList.count) {
        if (self.userListCollectionViewDelegate &&
            [self.userListCollectionViewDelegate respondsToSelector:@selector(addButtonDidClick)]) {
            [self.userListCollectionViewDelegate addButtonDidClick];
        }
    } else if (self.isAllowDelete && indexPath.row == self.userList.count + 1) {
        if (self.userListCollectionViewDelegate &&
            [self.userListCollectionViewDelegate respondsToSelector:@selector(deleteButtonDidClick)]) {
            [self.userListCollectionViewDelegate deleteButtonDidClick];
        }
    }
}
@end
