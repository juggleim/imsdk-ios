//
//  JCallProtocol.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/21.
//

#import <JuggleIM/JCallSession.h>

@protocol JCallReceiveDelegate <NSObject>

/// 接听到通话
/// - Parameter callSession: 通话实例
- (void)callDidReceive:(id<JCallSession>)callSession;

@end

@protocol JCallProtocol <NSObject>

/// 提供 Zego 初始化必需的参数
/// - Parameters:
///   - appId: Zego AppId
///   - appSign: Zego AppSign
- (void)initZegoEngineWith:(int)appId
                   appSign:(NSString *)appSign;

- (id<JCallSession>)startSingleCall:(NSString *)userId
                         delegate:(id<JCallSessionDelegate>)delegate;

- (void)addReceiveDelegate:(id<JCallReceiveDelegate>)receiveDelegate;

@end
