//
//  JConcreteMessage.h
//  JetIM
//
//  Created by Nathan on 2023/12/10.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JConcreteMessage : JMessage
@property (nonatomic, assign) long long msgIndex;
@property (nonatomic, copy) NSString *clientUid;
@property (nonatomic, assign) int flags;
@property (nonatomic, assign) BOOL existed;
@end

NS_ASSUME_NONNULL_END
