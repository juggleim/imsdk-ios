//
//  JSnapshotPackedVideoMessage.h
// JuggleIM
//
//  Created by Nathan on 2024/5/14.
//

#import <JuggleIM/JuggleIM.h>
#import <UIKit/UIKit.h>

NS_ASSUME_NONNULL_BEGIN

@interface JSnapshotPackedVideoMessage : JMediaMessageContent
/// Remote address of the video cover image
@property (nonatomic, strong) UIImage *snapshotImage;
/// Video height
@property (nonatomic, assign) int height;
/// Video width
@property (nonatomic, assign) int width;
/// Video size, in KB
@property (nonatomic, assign) long long size;
/// Video duration, in seconds
@property (nonatomic, assign) int duration;
/// Video file name
@property (nonatomic, copy) NSString *name;
/// Extra field
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
