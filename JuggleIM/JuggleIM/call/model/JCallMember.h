//
//  JCallMember.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JCallConst.h>
#import <JuggleIM/JUserInfo.h>

@interface JCallMember : NSObject
@property (nonatomic, strong) JUserInfo *userInfo;
@property (nonatomic, assign) JCallStatus callStatus;
@property (nonatomic, assign) long long startTime;
@property (nonatomic, assign) long long connectTime;
@property (nonatomic, assign) long long finishTime;
@property (nonatomic, strong) JUserInfo *inviter;
@end
