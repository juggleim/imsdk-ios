//
//  JCallMediaRoomConfig.h
//  JuggleIM
//
//  Created by Fei Li on 2024/11/1.
//

#import <Foundation/Foundation.h>

@interface JCallMediaRoomConfig : NSObject
@property (nonatomic, assign) BOOL isUserStatusNotify;
@property (nonatomic, copy) NSString *token;
@property (nonatomic, copy) NSString *url;
@end
