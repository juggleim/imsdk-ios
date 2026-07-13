//
//  JGetFavoriteMessageOption.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/13.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetFavoriteMessageOption : NSObject
/// Query start position. Pass empty for the first time; later calls can use the next offset from the success callback
@property (nonatomic, copy) NSString *offset;
/// Query count
@property (nonatomic, assign) int count;
@end

NS_ASSUME_NONNULL_END
