//
//  JPushData.h
//  JuggleIM
//
//  Created by Fei Li on 2024/9/18.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JPushData : NSObject

/// Push title, used for display in the notification bar
@property (nonatomic, copy) NSString *title;

/// Push content, used for display in the notification bar
@property (nonatomic, copy) NSString *content;

/// Extra field, can carry custom JSON
@property (nonatomic, copy) NSString *extra;

@end

NS_ASSUME_NONNULL_END
