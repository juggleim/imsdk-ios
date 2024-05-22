//
//  JLogCommandMessage.h
//  JetIM
//
//  Created by Nathan on 2024/5/21.
//

#import <JetIM/JetIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JLogCommandMessage : JMessageContent
@property (nonatomic, assign) long long startTime;
@property (nonatomic, assign) long long endTime;
@end

NS_ASSUME_NONNULL_END
