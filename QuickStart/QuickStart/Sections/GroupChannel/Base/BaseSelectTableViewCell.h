//
//  BaseSelectTableViewCell.h
//  QuickStart
//
//  Created by Fei Li on 2025/1/2.
//

#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface BaseSelectTableViewCell : UITableViewCell

- (instancetype)init NS_UNAVAILABLE;
- (instancetype)initWithSelected:(BOOL)isSelected;

@property (nonatomic, strong) UILabel *leftLabel;
@end

NS_ASSUME_NONNULL_END
