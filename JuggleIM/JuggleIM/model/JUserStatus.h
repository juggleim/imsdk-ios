//
//  JUserStatus.h
//  JuggleIM
//
//  Created by Fei Li on 2026/5/11.
//

#import <Foundation/Foundation.h>

NS_ASSUME_NONNULL_BEGIN

/*!
 在线类型
 */
typedef NS_ENUM(NSUInteger, JUserStatusType) {
    /*!
     未知
     */
    JUserStatusTypeUnknown = 0,
    /*!
     在线
     */
    JUserStatusTypeOnline = 1,
    /*!
     离线
     */
    JUserStatusTypeOffline = 2
};

@interface JUserStatus : NSObject

@property (nonatomic, copy) NSString *userId;
@property (nonatomic, assign) JUserStatusType statusType;

@end

NS_ASSUME_NONNULL_END
