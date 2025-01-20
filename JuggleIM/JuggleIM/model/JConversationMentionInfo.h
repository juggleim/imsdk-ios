//
//  JConversationMentionInfo.h
// JuggleIM
//
//  Created by 郑开 on 2024/6/18.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JMessageMentionInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JConversationMentionMessage : NSObject
@property (nonatomic, copy) NSString *senderId;
@property (nonatomic, copy) NSString *msgId;
@property (nonatomic, assign) long long msgTime;
@property (nonatomic, assign) JMentionType type;
@end

@interface JConversationMentionInfo : NSObject
@property (nonatomic, copy) NSArray <JConversationMentionMessage *> * mentionMsgList;
- (NSString *)encodeToJson;
+ (instancetype)decodeFromJson:(NSString *)json;
@end

NS_ASSUME_NONNULL_END
