//
//  JVideoMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/1/25.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JVideoMessage : JMediaMessageContent

+ (instancetype)videoWithData:(NSData *)videoFileData;

/// Local path of the video cover image
@property (nonatomic, copy) NSString *snapshotLocalPath;
/// Remote address of the video cover image
@property (nonatomic, copy) NSString *snapshotUrl;
/// Video height
@property (nonatomic, assign) int height;
/// Video width
@property (nonatomic, assign) int width;
/// Video size, in bytes
@property (nonatomic, assign) long long size;
/// Video duration, in seconds
@property (nonatomic, assign) int duration;
/// Extra field
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
