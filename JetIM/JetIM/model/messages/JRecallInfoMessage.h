//
//  JRecallInfoMessage.h
//  JetIM
//
//  Created by Nathan on 2024/2/3.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JRecallInfoMessage : JMessageContent
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
