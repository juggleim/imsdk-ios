//
//  JConcreteMessage.h
//  Juggle
//
//  Created by Nathan on 2023/12/10.
//

#import <Juggle/Juggle.h>

NS_ASSUME_NONNULL_BEGIN

@interface JConcreteMessage : JMessage
@property (nonatomic, assign) int64_t msgIndex;
@end

NS_ASSUME_NONNULL_END
