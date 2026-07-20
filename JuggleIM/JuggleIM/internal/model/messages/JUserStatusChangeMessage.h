//
//  JUserStatusChangeMessage.h
//  JuggleIM
//
//  Created by Fei Li on 2026/5/11.
//

#import <JuggleIM/JuggleIM.h>

NS_ASSUME_NONNULL_BEGIN

@interface JUserStatusChangeMessage : JMessageContent

@property (nonatomic, assign) BOOL isOnline;

@end

NS_ASSUME_NONNULL_END
