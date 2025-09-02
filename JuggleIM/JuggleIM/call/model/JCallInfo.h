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

/// 通话 id
@property (nonatomic, copy) NSString *callId;
/// 是否多人通话，NO 表示一对一通话
@property (nonatomic, assign) BOOL isMultiCall;
/// 媒体类型（语音/视频）
@property (nonatomic, assign) JCallMediaType mediaType;
/// 通话的发起人 id
@property (nonatomic, strong) JUserInfo *owner;
/// 通话参与者
@property (nonatomic, copy) NSArray <JCallMember *> *members;
/// 扩展字段
@property (nonatomic, copy) NSString *extra;

@end
