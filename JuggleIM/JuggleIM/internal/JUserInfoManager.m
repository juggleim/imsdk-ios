//
//  JUserInfoManager.m
// JuggleIM
//
//  Created by Nathan on 2024/3/12.
//

#import "JUserInfoManager.h"

@interface JUserInfoManager ()
@property (nonatomic, strong) JIMCore *core;
@end

@implementation JUserInfoManager

- (instancetype)initWithCore:(JIMCore *)core {
    if (self = [super init]) {
        self.core = core;
    }
    return self;
}

- (JUserInfo *)getUserInfo:(NSString *)userId {
    return [self.core.dbManager getUserInfo:userId];
}

- (JGroupInfo *)getGroupInfo:(NSString *)groupId {
    return [self.core.dbManager getGroupInfo:groupId];
}

- (JGroupMember *)getGroupMember:(NSString *)groupId userId:(NSString *)userId {
    return [self.core.dbManager getGroupMember:groupId userId:userId];
}

@end
