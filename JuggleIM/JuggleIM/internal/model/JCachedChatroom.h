//
//  JCachedChatroom.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/26.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

typedef NS_ENUM(NSUInteger, JChatroomStatus) {
    JChatroomStatusUnknown,
    JChatroomStatusJoining,
    JChatroomStatusJoined,
    JChatroomStatusFailed,
    JChatroomStatusQuit
};

@interface JCachedChatroom : NSObject
@property (nonatomic, assign) JChatroomStatus status;
@property (nonatomic, assign) long long syncTime;
@property (nonatomic, assign) long long attrSyncTime;
@property (nonatomic, copy) NSDictionary <NSString *, NSString *> *attributes;
@end

NS_ASSUME_NONNULL_END
