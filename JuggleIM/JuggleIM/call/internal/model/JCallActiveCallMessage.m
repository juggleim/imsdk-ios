//
//  JCallActiveCallMessage.m
//  JuggleIM
//
//  Created by Fei Li on 2025/9/1.
//

#import "JCallActiveCallMessage.h"

#define jActivedCall @"jg:activedcall"
#define jUserId @"user_id"
#define jUserName @"nickname"
#define jUserPortrait @"user_portrait"

@implementation JCallActiveCallMessage

+ (NSString *)contentType {
    return jActivedCall;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCmd;
}

- (void)decode:(NSData *)data {
    NSDictionary *json = [NSJSONSerialization JSONObjectWithData:data options:kNilOptions error:nil];
    JCallInfo *callInfo = [JCallInfo new];
    callInfo.callId = json[@"room_id"];
    id roomType = json[@"room_type"];
    if ([roomType isKindOfClass:[NSNumber class]]) {
        int t = [roomType intValue];
        callInfo.isMultiCall = t == 1;
    }
    
    NSDictionary *ownerDic = json[@"owner"];
    callInfo.owner = [self decodeUserInfo:ownerDic];
    callInfo.mediaType = JCallMediaTypeVoice;
    id mediaType = json[@"rtc_media_type"];
    if ([mediaType isKindOfClass:[NSNumber class]]) {
        callInfo.mediaType = [(NSNumber *)mediaType intValue];
    }
    NSArray *members = json[@"members"];
    NSMutableArray *arr = [NSMutableArray array];
    for (NSDictionary *memberDic in members) {
        JUserInfo *userInfo = [self decodeUserInfo:memberDic];
        JCallMember *callMember = [JCallMember new];
        callMember.userInfo = userInfo;
        [arr addObject:callMember];
    }
    callInfo.members = arr;
    self.callInfo = callInfo;
    id isFinished = json[@"finished"];
    if ([isFinished isKindOfClass:[NSNumber class]]) {
        self.isFinished = [(NSNumber *)isFinished boolValue];
    }
}

- (JUserInfo *)decodeUserInfo:(NSDictionary *)userInfoDic {
    JUserInfo *userInfo = [[JUserInfo alloc] init];
    userInfo.userId = userInfoDic[jUserId];
    userInfo.userName = userInfoDic[jUserName];
    userInfo.portrait = userInfoDic[jUserPortrait];
    return userInfo;
}

@end
