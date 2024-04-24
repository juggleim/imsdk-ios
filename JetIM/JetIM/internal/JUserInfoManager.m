//
//  JUserInfoManager.m
//  JetIM
//
//  Created by Nathan on 2024/3/12.
//

#import "JUserInfoManager.h"

@interface JUserInfoManager ()
@property (nonatomic, strong) JetIMCore *core;
@end

@implementation JUserInfoManager

- (instancetype)initWithCore:(JetIMCore *)core {
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

@end
