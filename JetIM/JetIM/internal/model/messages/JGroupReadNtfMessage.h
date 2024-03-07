//
//  JGroupReadNtfMessage.h
//  JetIM
//
//  Created by Nathan on 2024/3/6.
//

#import <JetIM/JetIM.h>
#import <JGroupMessageReadInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupReadNtfMessage : JMessageContent
@property (nonatomic, strong) NSDictionary <NSString *, JGroupMessageReadInfo *> *msgs;
@end

NS_ASSUME_NONNULL_END
