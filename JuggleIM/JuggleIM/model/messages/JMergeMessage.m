//
//  JMergeMessage.m
// JuggleIM
//
//  Created by Nathan on 2024/3/19.
//

#import "JMergeMessage.h"

#define jMergeType @"jg:merge"
#define jMergeTitle @"title"
#define jMessageIdList @"messageIdList"
#define jMessageConversationId      @"conversationId"
#define jMessageConversationType    @"conversationType"
#define jMessageContainerMsgId      @"containerMsgId"
#define jMergeExtra @"extra"
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
                 conversation:(JConversation *)conversation
                MessageIdList:(NSArray <NSString *> *)messageIdList
                  previewList:(NSArray <JMergeMessagePreviewUnit *> *)previewList{
    if (self = [super init]) {
        self.title = title;
        if (messageIdList.count > 100) {
            messageIdList = [messageIdList subarrayWithRange:NSMakeRange(0, 99)];
        }
        self.messageIdList = messageIdList;
        if (previewList.count > 10) {
            previewList = [previewList subarrayWithRange:NSMakeRange(0, 9)];
        }
        self.previewList = previewList;
        self.conversation = conversation;
    }
    return self;
}

+ (NSString *)contentType {
    return jMergeType;
}

-(NSData *)encode{
    NSMutableDictionary *mergeDic = [NSMutableDictionary dictionary];
    [mergeDic setObject:self.title?:@"" forKey:jMergeTitle];
    [mergeDic setObject:self.extra?:@"" forKey:jMergeExtra];
    [mergeDic setObject:self.conversation.conversationId?:@"" forKey:jMessageConversationId];
    [mergeDic setObject:@(self.conversation.conversationType) forKey:jMessageConversationType];
    [mergeDic setObject:self.containerMsgId?:@"" forKey:jMessageContainerMsgId];
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
    NSData *data = [NSJSONSerialization dataWithJSONObject:mergeDic options:kNilOptions error:nil];
    return data;
}

- (void)decode:(NSData *)data{
    NSDictionary * json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    self.title = json[jMergeTitle];
    self.messageIdList = json[jMessageIdList];
    self.extra = json[jMergeExtra];
    self.containerMsgId = json[jMessageContainerMsgId];
    JConversation * conversation = [[JConversation alloc] init];
    conversation.conversationId = json[jMessageConversationId];
    NSNumber * type = json[jMessageConversationType];
    if([type isKindOfClass:[NSNumber class]]){
        conversation.conversationType = [type integerValue];
    }
    self.conversation = conversation;
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
