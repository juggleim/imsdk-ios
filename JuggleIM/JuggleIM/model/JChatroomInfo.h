//
//  JChatroomInfo.h
// JuggleIM
//
//  Created by Nathan on 2024/7/8.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JChatroomMember.h>
#import <JuggleIM/JuggleIMConst.h>

NS_ASSUME_NONNULL_BEGIN


@interface JChatroomInfo : NSObject

/// Chatroom id
@property (nonatomic, copy) NSString *chatroomId;

/// Member order
@property (nonatomic, assign) JChatroomMemberOrder memberOrder;

/// Number of chatroom members
@property (nonatomic, assign) int totalMemberCount;

/// Member list, containing only the first 100 entries (according to the memberOrder sort rule)
@property (nonatomic, copy) NSArray <JChatroomMember *> *memberList;

@end

NS_ASSUME_NONNULL_END
