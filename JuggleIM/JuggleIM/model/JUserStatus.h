//
//  JUserStatus.h
//  JuggleIM
//
//  Created by Fei Li on 2026/5/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 Online status type
 */
typedef NS_ENUM(NSUInteger, JUserStatusType) {
    /*!
     Unknown
     */
    JUserStatusTypeUnknown = 0,
    /*!
     Online
     */
    JUserStatusTypeOnline = 1,
    /*!
     Offline
     */
    JUserStatusTypeOffline = 2
};

@interface JUserStatus : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) JUserStatusType statusType;

@end

NS_ASSUME_NONNULL_END
