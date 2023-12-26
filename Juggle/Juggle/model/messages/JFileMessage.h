//
//  JFileMessage.h
//  Juggle
//
//  Created by Nathan on 2023/12/26.
//

#import <Juggle/Juggle.h>

NS_ASSUME_NONNULL_BEGIN

@interface JFileMessage : JMessageContent
@property (nonatomic, copy) NSString *name;
@property (nonatomic, copy) NSString *url;
@property (nonatomic, assign) long long size;
@property (nonatomic, copy) NSString *type;
@end

NS_ASSUME_NONNULL_END
