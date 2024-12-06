//
//  JCallMediaDelegate.h
//  JuggleIM
//
//  Created by Fei Li on 2024/12/3.
//

#ifndef JCallMediaDelegate_h
#define JCallMediaDelegate_h

@protocol JCallMediaDelegate <NSObject>

- (UIView *)viewForUserId:(NSString *)userId;

- (void)usersDidJoin:(NSArray <NSString *> *)userIdList;

@end

#endif /* JCallMediaDelegate_h */
