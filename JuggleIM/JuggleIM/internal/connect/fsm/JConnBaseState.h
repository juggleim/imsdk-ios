//
//  JConnBaseState.h
//  JuggleIM
//
//  Created by Fei Li on 2024/11/12.
//

#import "JState.h"
#import "JConnectionManager.h"

NS_ASSUME_NONNULL_BEGIN

@interface JConnBaseState : JState
@property (nonatomic, weak) JConnectionManager *connectionManager;
@end

NS_ASSUME_NONNULL_END
