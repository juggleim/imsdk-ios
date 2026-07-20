//
//  JGroupMessageMemberReadDetail.h
//  JuggleIM
//
//  Created by Fei Li on 2025/10/30.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JUserInfo.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupMessageMemberReadDetail : NSObject
/// User info
@property (nonatomic, strong) JUserInfo *userInfo;
/// Read time
@property (nonatomic, assign) long long readTime;

@end

NS_ASSUME_NONNULL_END
