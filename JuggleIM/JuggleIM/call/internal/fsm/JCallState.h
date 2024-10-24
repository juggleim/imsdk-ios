//
//  JCallState.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/22.
//

#import "JState.h"
#import "JCallSessionImpl.h"

NS_ASSUME_NONNULL_BEGIN

@interface JCallState : JState
@property (nonatomic, weak) JCallSessionImpl *callSessionImpl;
@end

NS_ASSUME_NONNULL_END
