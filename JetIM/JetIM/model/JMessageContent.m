//
//  JMessageContent.m
//  JetIM
//
//  Created by Nathan on 2023/11/28.
//

#import "JMessageContent.h"

#define kUnknownType @"jg:unknown"

@implementation JMessageContent

+ (NSString *)contentType {
    return kUnknownType;
}

+ (JMessageFlag)flags {
    return JMessageFlagIsCountable|JMessageFlagIsSave;
}

/// 数据序列化方法
-(NSData *)jmessageContentEncode{
    return nil;
}

/// 数据反序列话方法
/// - Parameter date: 数据内容
-(void)jmessageContentDecode:(NSData *)data{
    
}

- (NSString *)conversationDigest {
    return @"";
}

/// 本地模糊搜索内容
- (NSString *)searchContent{
    return @"";
}
@end
