//
//  JMessage+internal.h
//  Juggle
//
//  Created by Nathan on 2023/12/7.
//

#import <Juggle/Juggle.h>

NS_ASSUME_NONNULL_BEGIN

@interface JMessage (internal)
@property (nonatomic, assign) int64_t msgIndex;
@end

NS_ASSUME_NONNULL_END
