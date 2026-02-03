//
//  JStreamAppendMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2026/1/29.
//

#import "JStreamAppendMessage.h"

#define jSAStreamId @"stream_id"
#define jSAContent @"content"
#define jSASeq @"seq"
#define jSAIsFinished @"is_finished"

@implementation JStreamAppendMessage

+ (NSString *)contentType {
    return @"jg:streamappend";
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    id streamId = json[jSAStreamId];
    if ([streamId isKindOfClass:[NSString class]]) {
        self.streamId = streamId;
    } else {
        self.streamId = @"";
    }
    id content = json[jSAContent];
    if ([content isKindOfClass:[NSString class]]) {
        self.content = content;
    } else {
        self.content = @"";
    }
    id isFinished = json[jSAIsFinished];
    if ([isFinished isKindOfClass:[NSNumber class]]) {
        self.isFinished = [(NSNumber *)isFinished boolValue];
    }
    id seq = json[jSASeq];
    if ([seq isKindOfClass:[NSNumber class]]) {
        self.seq = [(NSNumber *)seq intValue];
    }
}

+ (JMessageFlag)flags {
    return JMessageFlagNone;
}

@end
