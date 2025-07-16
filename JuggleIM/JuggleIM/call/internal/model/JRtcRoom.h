//
//  JRtcRoom.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/24.
//

#import <Foundation/Foundation.h>
#import "JUserInfo.h"
#import "JCallMember.h"

NS_ASSUME_NONNULL_BEGIN

@interface JRtcRoom : NSObject
@property (nonatomic, copy) NSString *roomId;
@property (nonatomic, strong) JUserInfo *owner;
@property (nonatomic, assign) BOOL isMultiCall;
@property (nonatomic, copy) NSString *deviceId;
@property (nonatomic, assign) JCallStatus callStatus;
@property (nonatomic, copy) NSArray <JCallMember *> *members;
@property (nonatomic, assign) JCallMediaType mediaType;
@property (nonatomic, copy) NSString *extra;
@end

NS_ASSUME_NONNULL_END
