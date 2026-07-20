//
//  JGetMomentCommentOption.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/13.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JuggleIMConst.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetMomentCommentOption : NSObject
/// Moment id
@property (nonatomic, copy) NSString *momentId;
/// Starting timestamp for querying comments; 0 means the current time
@property (nonatomic, assign) long long startTime;
/// Fetch count，maximum 50
@property (nonatomic, assign) int count;
/// Fetch direction
@property (nonatomic, assign) JPullDirection direction;

- (NSDictionary *)toDictionary;
@end

NS_ASSUME_NONNULL_END
