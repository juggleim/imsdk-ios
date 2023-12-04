//
//  JImageMessage.h
//  Juggle
//
//  Created by Nathan on 2023/12/3.
//

#import <Juggle/Juggle.h>

NS_ASSUME_NONNULL_BEGIN

@interface JImageMessage : JMessageContent
@property (nonatomic, copy) NSString *imageURL;
@property (nonatomic, copy) NSString *thumbnailURL;
@end

NS_ASSUME_NONNULL_END
