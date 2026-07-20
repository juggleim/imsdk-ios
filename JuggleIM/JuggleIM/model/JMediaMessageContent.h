//
//  JMediaMessageContent.h
// JuggleIM
//
//  Created by Nathan on 2024/5/9.
//

#import <JuggleIM/JMessageContent.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMediaMessageContent : JMessageContent
/// Local path
@property (nonatomic, copy, nullable) NSString *localPath;
/// Remote address
@property (nonatomic, copy, nullable) NSString *url;

@end

NS_ASSUME_NONNULL_END
