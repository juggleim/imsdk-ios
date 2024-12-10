//
//  MultiAudioCallUserCollectionLayout.h
//  QuickStart
//
//  Created by Fei Li on 2024/12/10.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface MultiAudioCallUserCollectionLayout : UICollectionViewFlowLayout

@property(nonatomic, assign) CGFloat itemMargin;
@property(nonatomic, assign) CGFloat buttomPadding;

- (instancetype)initWithItemMargin:(CGFloat)itemMargin buttomPadding:(CGFloat)buttomPadding;

@end

NS_ASSUME_NONNULL_END
