//
//  JFriend.h
//  JuggleIM
//
//  Created by Fei Li on 2026/1/23.
//

#import <Foundation/Foundation.h>

@interface JFriendInfo : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) BOOL isFriend;
// 好友备注
@property (nonatomic, copy) NSString *alias;
@property (nonatomic, assign) long long updatedTime;

@end

