//
//  JMsgModifyMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/1/8.
//

#define jModifyCmdType @"jg:modify"
#define jModifyMsgTime @"msg_time"
#define jModifyMsgType @"msg_type"
#define jModifyMsgId @"msg_id"
#define jModifyMsgContent @"msg_content"

#import "JMsgModifyMessage.h"
#import "JContentTypeCenter.h"

@implementation JMsgModifyMessage
+ (NSString *)contentType {
    return jModifyCmdType;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.originalMessageId = json[jModifyMsgId]?:@"";
    id time = json[jModifyMsgTime];
    if ([time isKindOfClass:[NSNumber class]]) {
        self.originalMessageTime = [(NSNumber *)time longLongValue];
    }
    self.messageType = json[jModifyMsgType]?:@"";
    NSString *contentString = json[jModifyMsgContent];
    NSData *contentData = [[NSData alloc] initWithBase64EncodedString:contentString options:NSDataBase64DecodingIgnoreUnknownCharacters];
    self.messageContent = [[JContentTypeCenter shared] contentWithData:contentData contentType:self.messageType];
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}


@end
