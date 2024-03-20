//
//  JMergeMessage.m
//  JetIM
//
//  Created by Nathan on 2024/3/19.
//

#import "JMergeMessage.h"

#define jMergeType @"jg:merge"
#define jMergeTitle @"title"
#define jMessageIdList @"messageIdList"
#define jPreviewList @"previewList"
#define jPreviewContent @"content"
#define jPreviewUserId @"userId"
#define jPreviewUserName @"name"
#define jPreviewPortrait @"portrait"
#define jMergeDigest @"[Merge]"

@interface JMergeMessage ()
@property (nonatomic, copy) NSString *title;
@property (nonatomic, copy) NSArray <NSString *> *messageIdList;
@property (nonatomic, copy) NSArray <JMergeMessagePreviewUnit *> *previewList;
@end

@implementation JMergeMessagePreviewUnit
@end

@implementation JMergeMessage


- (instancetype)initWithTitle:(NSString *)title
                MessageIdList:(NSArray<NSString *> *)messageIdList
                  previewList:(NSArray<JMergeMessagePreviewUnit *> *)previewList {
    JMergeMessage *message = [[JMergeMessage alloc] init];
    message.title = title;
    if (messageIdList.count > 100) {
        messageIdList = [messageIdList subarrayWithRange:NSMakeRange(0, 99)];
    }
    message.messageIdList = messageIdList;
    if (previewList.count > 10) {
        previewList = [previewList subarrayWithRange:NSMakeRange(0, 9)];
    }
    message.previewList = previewList;
    return message;
}

+ (NSString *)contentType {
    return jMergeType;
}

- (NSDictionary *)encodeToJson {
    NSMutableDictionary *mergeDic = [NSMutableDictionary dictionary];
    [mergeDic setObject:self.title?:@"" forKey:jMergeTitle];
    if (self.messageIdList.count > 0) {
        [mergeDic setObject:self.messageIdList forKey:jMessageIdList];
    }
    NSMutableArray *previewListJson = [NSMutableArray array];
    for (JMergeMessagePreviewUnit *unit in self.previewList) {
        NSMutableDictionary *unitJson = [NSMutableDictionary dictionary];
        [unitJson setObject:unit.previewContent?:@"" forKey:jPreviewContent];
        [unitJson setObject:unit.sender.userId?:@"" forKey:jPreviewUserId];
        [unitJson setObject:unit.sender.userName?:@"" forKey:jPreviewUserName];
        [unitJson setObject:unit.sender.portrait?:@"" forKey:jPreviewPortrait];
        [previewListJson addObject:unitJson];
    }
    [mergeDic setObject:previewListJson forKey:jPreviewList];
    return mergeDic;
}

- (void)decodeWithJson:(NSDictionary *)json {
    self.title = json[jMergeTitle];
    self.messageIdList = json[jMessageIdList];
    NSArray *previewListJson = json[jPreviewList];
    NSMutableArray *previewList = [NSMutableArray array];
    if ([previewListJson isKindOfClass:[NSArray class]]) {
        for (NSDictionary *unitJson in previewListJson) {
            if ([unitJson isKindOfClass:[NSDictionary class]]) {
                JMergeMessagePreviewUnit *unit = [[JMergeMessagePreviewUnit alloc] init];
                unit.previewContent = unitJson[jPreviewContent];
                JUserInfo *userInfo = [[JUserInfo alloc] init];
                userInfo.userId = unitJson[jPreviewUserId];
                userInfo.userName = unitJson[jPreviewUserName];
                userInfo.portrait = unitJson[jPreviewPortrait];
                unit.sender = userInfo;
                [previewList addObject:unit];
            }
        }
    }
    self.previewList = previewList;
}

- (NSString *)conversationDigest {
    return jMergeDigest;
}

@end
