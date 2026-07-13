//
//  JMomentMedia.h
//  JuggleIM
//
//  Created by Fei Li on 2025/11/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JMomentMediaType) {
    /*
     Image
     */
    JMomentMediaTypeImage,
    
    /*
     Video
     */
    JMomentMediaTypeVideo
};

@interface JMomentMedia : NSObject

/// Remote address
@property (nonatomic, copy) NSString *url;
/// Remote address of the thumbnail
@property (nonatomic, copy) NSString *snapshotUrl;
/// Type
@property (nonatomic, assign) JMomentMediaType type;
/// Height
@property (nonatomic, assign) int height;
/// Width
@property (nonatomic, assign) int width;
/// Duration (for videos)
@property (nonatomic, assign) int duration;

- (NSDictionary *)toDictionary;

+ (JMomentMedia *)mediaWith:(NSDictionary *)dictionary;

@end

NS_ASSUME_NONNULL_END
