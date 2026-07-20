//
//  JCallInfo.h
//  JuggleIM
//
//  Created by Fei Li on 2025/8/29.
//

#import <Foundation/Foundation.h>
#import <JuggleIM/JCallConst.h>
#import <JuggleIM/JCallMember.h>

@interface JCallInfo : NSObject

/// Call ID.
@property (nonatomic, copy) NSString *callId;
/// Whether this is a multi-party call. NO means a one-to-one call.
@property (nonatomic, assign) BOOL isMultiCall;
/// Media type (audio/video).
@property (nonatomic, assign) JCallMediaType mediaType;
/// User info of the call initiator.
@property (nonatomic, strong) JUserInfo *owner;
/// Call participants.
@property (nonatomic, copy) NSArray <JCallMember *> *members;
/// Extra field.
@property (nonatomic, copy) NSString *extra;

@end
