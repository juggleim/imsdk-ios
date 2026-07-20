//
//  JGroupMessageReadInfoDetail.h
//  JuggleIM
//
//  Created by Fei Li on 2025/10/30.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JGroupMessageMemberReadDetail.h>

NS_ASSUME_NONNULL_BEGIN

@interface JGroupMessageReadInfoDetail : NSObject

/// Group message read count
@property (nonatomic, assign) int readCount;
/// Total group member count
@property (nonatomic, assign) int memberCount;
/// List of users who have read
@property (nonatomic, copy) NSArray <JGroupMessageMemberReadDetail *> *readMembers;
/// List of users who have not read
@property (nonatomic, copy) NSArray <JGroupMessageMemberReadDetail *> *unreadMembers;

@end

NS_ASSUME_NONNULL_END
