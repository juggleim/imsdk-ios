//
//  JMessageReferredInfo.h
//  JetIM
//
//  Created by 郑开 on 2024/6/18.
//

#import <Foundation/Foundation.h>
#import <JetIM/JMessageContent.h>

@interface JMessageReferredInfo : NSObject

@property (nonatomic, copy) NSString * messageId;

@property (nonatomic, copy) NSString * senderId;

@property (nonatomic, strong) JMessageContent * content;

@end

