//
//  JReadNtfMessage.h
//  JetIM
//
//  Created by Nathan on 2024/3/4.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JReadNtfMessage : JMessageContent
@property (nonatomic, copy) NSArray <NSString *> *messageIds;
@end

NS_ASSUME_NONNULL_END
