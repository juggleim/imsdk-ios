//
//  JChatroomManager.h
//  JuggleIM
//
//  Created by Fei Li on 2024/8/13.
//

#import <Foundation/Foundation.h>
#import "JChatroomProtocol.h"
#import "JIMCore.h"

NS_ASSUME_NONNULL_BEGIN

@interface JChatroomManager : NSObject <JChatroomProtocol>
- (instancetype)initWithCore:(JIMCore *)core;
@end

NS_ASSUME_NONNULL_END
