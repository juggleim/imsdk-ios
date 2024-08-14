//
//  JConversationMentionInfo.m
// JuggleIM
//
//  Created by 郑开 on 2024/6/18.
//

#import "JConversationMentionInfo.h"



#define jConversationMentionInfoMsgs                @"mentionMsgs"
#define jConversationMentionMessageSenderId         @"senderId"
#define jConversationMentionMessageMsgId            @"msgId"
#define jConversationMentionMessageMsgTime          @"msgTime"


@implementation JConversationMentionMessage


- (BOOL)isEqual:(id)object{
    if ([object isKindOfClass:self.class]) {
        JConversationMentionMessage * it = (JConversationMentionMessage *)object;
        return [self.msgId isEqualToString:it.msgId];
    }else{
        return [super isEqual:object];
    }
}


@end


@implementation JConversationMentionInfo

- (NSString *)encodeToJson {
    NSMutableDictionary *dic = [NSMutableDictionary dictionary];
    if (self.mentionMsgList.count > 0) {
        NSMutableArray *msgs = [NSMutableArray array];
        for (JConversationMentionMessage * mentionMessage in self.mentionMsgList) {
            NSMutableDictionary *msg = [NSMutableDictionary dictionary];
            [msg setObject:mentionMessage.senderId ? : @"" forKey:jConversationMentionMessageSenderId];
            [msg setObject:mentionMessage.msgId ? : @"" forKey:jConversationMentionMessageMsgId];
            [msg setObject:@(mentionMessage.msgTime) forKey:jConversationMentionMessageMsgTime];
            [msgs addObject:msg];
        }
        [dic setObject:msgs forKey:jConversationMentionInfoMsgs];
    }
    NSData *d = [NSJSONSerialization dataWithJSONObject:dic options:kNilOptions error:nil];
    NSString *s = [[NSString alloc] initWithData:d encoding:NSUTF8StringEncoding];
    return s;
}

+ (instancetype)decodeFromJson:(NSString *)json {
    if (json.length == 0) {
        return nil;
    }
    NSData *data = [json dataUsingEncoding:NSUTF8StringEncoding];
    NSDictionary *dic = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    JConversationMentionInfo *info = [[JConversationMentionInfo alloc] init];

    NSArray *userJson = dic[jConversationMentionInfoMsgs];
    if (userJson.count > 0) {
        NSMutableArray <JConversationMentionMessage *> * msgs = [NSMutableArray array];
        for (NSDictionary * msg in userJson) {
            JConversationMentionMessage * mentionMessage = [[JConversationMentionMessage alloc] init];
            mentionMessage.senderId = msg[jConversationMentionMessageSenderId]?:@"";
            mentionMessage.msgId = msg[jConversationMentionMessageMsgId]?:@"";
            NSNumber * msgTime = msg[jConversationMentionMessageMsgTime];
            if(msgTime){
                mentionMessage.msgTime = msgTime.longLongValue;
            }else{
                mentionMessage.msgTime = 0;
            }
            [msgs addObject:mentionMessage];
        }
        info.mentionMsgList = msgs;
    }
    return info;
}

@end
