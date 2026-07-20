//
//  JMessageOptions.h
// JuggleIM
//
//  Created by Zheng Kai on 2024/6/18.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JMessageMentionInfo.h>
#import <JuggleIM/JPushData.h>

@interface JMessageOptions : NSObject

/// mention info (nil for non-mention messages)
@property (nonatomic, strong) JMessageMentionInfo *mentionInfo;

/// Referenced message
@property (nonatomic, copy) NSString *referredMsgId;

/// Push info
@property (nonatomic, strong) JPushData *pushData;

/// Message lifetime, in milliseconds. Example: 86400000 = 24 * 60 * 60 * 1000; this message will be automatically deleted after 1 day.
/// The default value is 0, which means no automatic destruction.
@property (nonatomic, assign) long long lifeTime;

/// Message lifetime after being read, in milliseconds. Usually less than lifeTime.
/// The default value is 0, which means no automatic destruction after read.
@property (nonatomic, assign) long long lifeTimeAfterRead;
@end
