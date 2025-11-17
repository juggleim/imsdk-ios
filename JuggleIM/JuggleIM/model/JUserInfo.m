//
//  JUserInfo.m
// JuggleIM
//
//  Created by Nathan on 2024/3/9.
//

#import "JUserInfo.h"

@implementation JUserInfo
- (NSDictionary *)toDictionary {
    NSMutableDictionary *dict = [NSMutableDictionary dictionary];
    dict[@"userId"] = self.userId;
    dict[@"userName"] = self.userName ?: [NSNull null];
    dict[@"portrait"] = self.portrait ?: [NSNull null];
    dict[@"extraDic"] = self.extraDic ?: [NSNull null];
    dict[@"type"] = @(self.type);
    dict[@"updatedTime"] = @(self.updatedTime);
    return dict.copy;
}
@end
