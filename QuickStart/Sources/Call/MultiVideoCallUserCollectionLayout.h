//
//  MultiVideoCallUserCollectionLayout.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/10.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface MultiVideoCallUserCollectionLayout : UICollectionViewLayout

@property(nonatomic, assign) CGFloat itemMargin;

- (instancetype)initWithItemMargin:(CGFloat)itemMargin;

@end

NS_ASSUME_NONNULL_END
