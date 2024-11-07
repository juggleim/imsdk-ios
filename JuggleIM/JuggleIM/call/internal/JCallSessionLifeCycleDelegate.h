//
//  JCallSessionLifeCycleDelegate.h
//  JuggleIM
//
//  Created by Fei Li on 2024/10/23.
//

@class JCallSessionImpl;

@protocol JCallSessionLifeCycleDelegate <NSObject>
- (void)sessionDidfinish:(JCallSessionImpl *)session;
- (void)callDidReceive:(JCallSessionImpl *)session;
- (BOOL)callDidAccept:(JCallSessionImpl *)session;
@end



