//
//  JGetMessageOptions.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/16.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGetMessageOptions : NSObject
/// Message timestamp; when 0 or unset, defaults to the current time
@property (nonatomic, assign) long long startTime;
/// Fetch count; defaults to 100, and values over 100 return 100
@property (nonatomic, assign) int count;
/// List of message types to fetch. Example for getting a message type: [JTextMessage contentType]. If empty, all message types are fetched
@property (nonatomic, copy) NSArray <NSString *> *contentTypes;
@end

NS_ASSUME_NONNULL_END
